#include <ntifs.h>
#include <windef.h>
#include <ntimage.h>
#include <cstdint>
#include "defines.h"
#include <stdlib.h>
#include <intrin.h>

UNICODE_STRING name, link;

typedef struct _SYSTEM_BIGPOOL_ENTRY {
	PVOID VirtualAddress;
	ULONG_PTR NonPaged : 1;
	ULONG_PTR SizeInBytes;
	UCHAR Tag[4];
} SYSTEM_BIGPOOL_ENTRY, * PSYSTEM_BIGPOOL_ENTRY;

typedef struct _SYSTEM_BIGPOOL_INFORMATION {
	ULONG Count;
	SYSTEM_BIGPOOL_ENTRY AllocatedInfo[1]; // Flexible array member, adjust as needed
} SYSTEM_BIGPOOL_INFORMATION, * PSYSTEM_BIGPOOL_INFORMATION;

typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBigPoolInformation = 0x42,
} SYSTEM_INFORMATION_CLASS;

extern "C" NTSTATUS NTAPI IoCreateDriver(PUNICODE_STRING DriverName, PDRIVER_INITIALIZE InitializationFunction);
extern "C" PVOID NTAPI PsGetProcessSectionBaseAddress(PEPROCESS Process);
extern "C" NTSTATUS NTAPI ZwQuerySystemInformation(SYSTEM_INFORMATION_CLASS systemInformationClass, PVOID systemInformation, ULONG systemInformationLength, PULONG returnLength);

#define code_rw CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1545, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_ba CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1546, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_get_guarded_region CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1547, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_GetDirBase CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1548, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_security 0x83b5b69
#define win_1803 17134
#define win_1809 17763
#define win_1903 18362
#define win_1909 18363
#define win_2004 19041
#define win_20H2 19569
#define win_21H1 20180
#define win_22H2 19045

#define PAGE_OFFSET_SIZE 12
static const UINT64 PMASK = (~0xfull << 8) & 0xfffffffffull;

typedef struct _rw {
	INT32 security;
	INT32 process_id;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
	BOOLEAN write;
} rw, * prw;

typedef struct _ba {
	INT32 security;
	INT32 process_id;
	ULONGLONG* address;
} ba, * pba;

typedef struct _ga {
	INT32 security;
	ULONGLONG* address;
} ga, * pga;

typedef struct _MEMORY_OPERATION_DATA {
	uint32_t        pid;
	uintptr_t       cr3;
} MEMORY_OPERATION_DATA, * PMEMORY_OPERATION_DATA;


NTSTATUS read(PVOID target_address, PVOID buffer, SIZE_T size, SIZE_T* bytes_read) {
	MM_COPY_ADDRESS to_read = { 0 };
	to_read.PhysicalAddress.QuadPart = (LONGLONG)target_address;
	// return MmCopyMemory(buffer, to_read, size, MM_COPY_MEMORY_VIRTUAL, bytes_read); // read virtual (doesnt work)
	return MmCopyMemory(buffer, to_read, size, MM_COPY_MEMORY_PHYSICAL, bytes_read); // read physical
}

NTSTATUS write(PVOID target_address, PVOID buffer, SIZE_T size, SIZE_T* bytes_read)
{
	if (!target_address)
		return STATUS_UNSUCCESSFUL;

	PHYSICAL_ADDRESS AddrToWrite = { 0 };
	AddrToWrite.QuadPart = LONGLONG(target_address);

	PVOID pmapped_mem = MmMapIoSpaceEx(AddrToWrite, size, PAGE_READWRITE);

	if (!pmapped_mem)
		return STATUS_UNSUCCESSFUL;

	memcpy(pmapped_mem, buffer, size);

	*bytes_read = size;
	MmUnmapIoSpace(pmapped_mem, size);
	return STATUS_SUCCESS;
}

INT32 get_winver() {
	RTL_OSVERSIONINFOW ver = { 0 };
	RtlGetVersion(&ver);
	switch (ver.dwBuildNumber)
	{
	case win_1803:
		return 0x0278;
		break;
	case win_1809:
		return 0x0278;
		break;
	case win_1903:
		return 0x0280;
		break;
	case win_1909:
		return 0x0280;
		break;
	case win_2004:
		return 0x0388;
		break;
	case win_20H2:
		return 0x0388;
		break;
	case win_21H1:
		return 0x0388;
		break;
	case win_22H2:
		return 0x0388;
		break;
	default:
		return 0x0388;
	}
}

volatile uint64_t g_MmPfnDatabase = 0;
volatile uint64_t g_PXE_BASE = 0;
volatile uint64_t g_idx = 0;
uintptr_t dirBase = 0;

void initDefinesCR3() {

	KDDEBUGGER_DATA64 kdBlock = { 0 };
	CONTEXT context = { 0 };
	context.ContextFlags = CONTEXT_FULL;
	(RtlCaptureContext)(&context);

	PDUMP_HEADER dumpHeader = (PDUMP_HEADER)ExAllocatePool(NonPagedPool, DUMP_BLOCK_SIZE);
	if (dumpHeader) {
		(KeCapturePersistentThreadState)(&context, NULL, 0, 0, 0, 0, 0, dumpHeader);
		RtlCopyMemory(&kdBlock, (PUCHAR)dumpHeader + KDDEBUGGER_DATA_OFFSET, sizeof(kdBlock));

		ExFreePool(dumpHeader);

		g_MmPfnDatabase = *(ULONG64*)(kdBlock.MmPfnDatabase);

		// Calculate base addresses for page table entries
		ULONG64 g_PTE_BASE = kdBlock.PteBase;
		ULONG64 g_PDE_BASE = g_PTE_BASE + ((g_PTE_BASE & 0xffffffffffff) >> 9);
		ULONG64 g_PPE_BASE = g_PTE_BASE + ((g_PDE_BASE & 0xffffffffffff) >> 9);
		g_PXE_BASE = g_PTE_BASE + ((g_PPE_BASE & 0xffffffffffff) >> 9);
		g_idx = (g_PTE_BASE >> 39) - 0x1FFFE00;
	}
}

uintptr_t get_kernel_base() {
	const auto idtbase = *reinterpret_cast<uint64_t*>(__readgsqword(0x18) + 0x38);
	const auto descriptor_0 = *reinterpret_cast<uint64_t*>(idtbase);
	const auto descriptor_1 = *reinterpret_cast<uint64_t*>(idtbase + 8);
	const auto isr_base = ((descriptor_0 >> 32) & 0xFFFF0000) + (descriptor_0 & 0xFFFF) + (descriptor_1 << 32);
	auto align_base = isr_base & 0xFFFFFFFFFFFFF000;

	for (; ; align_base -= 0x1000) {
		for (auto* search_base = reinterpret_cast<uint8_t*>(align_base); search_base < reinterpret_cast<uint8_t*>(align_base) + 0xFF9; search_base++) {
			if (search_base[0] == 0x48 &&
				search_base[1] == 0x8D &&
				search_base[2] == 0x1D &&
				search_base[6] == 0xFF) {
				const auto relative_offset = *reinterpret_cast<int*>(&search_base[3]);
				const auto address = reinterpret_cast<uint64_t>(search_base + relative_offset + 7);
				if ((address & 0xFFF) == 0) {
					if (*reinterpret_cast<uint16_t*>(address) == 0x5A4D) {
						return address;
					}
				}
			}
		}
	}
}

intptr_t search_pattern(void* module_handle, const char* section, const char* signature_value) {
	static auto in_range = [](auto x, auto a, auto b) { return (x >= a && x <= b); };
	static auto get_bits = [](auto  x) { return (in_range((x & (~0x20)), 'A', 'F') ? ((x & (~0x20)) - 'A' + 0xa) : (in_range(x, '0', '9') ? x - '0' : 0)); };
	static auto get_byte = [](auto  x) { return (get_bits(x[0]) << 4 | get_bits(x[1])); };

	const auto dos_headers = reinterpret_cast<PIMAGE_DOS_HEADER>(module_handle);
	const auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uintptr_t>(module_handle) + dos_headers->e_lfanew);
	const auto section_headers = reinterpret_cast<PIMAGE_SECTION_HEADER>(nt_headers + 1);

	auto range_start = 0ui64;
	auto range_end = 0ui64;
	for (auto cur_section = section_headers; cur_section < section_headers + nt_headers->FileHeader.NumberOfSections; cur_section++) {
		if (strcmp(reinterpret_cast<const char*>(cur_section->Name), section) == 0) {
			range_start = reinterpret_cast<uintptr_t>(module_handle) + cur_section->VirtualAddress;
			range_end = range_start + cur_section->Misc.VirtualSize;
		}
	}

	if (range_start == 0)
		return 0u;

	auto first_match = 0ui64;
	auto pat = signature_value;
	for (uintptr_t cur = range_start; cur < range_end; cur++) {
		if (*pat == '\0') {
			return first_match;
		}
		if (*(uint8_t*)pat == '\?' || *reinterpret_cast<uint8_t*>(cur) == get_byte(pat)) {
			if (!first_match)
				first_match = cur;

			if (!pat[2])
				return first_match;

			if (*(uint16_t*)pat == 16191 || *(uint8_t*)pat != '\?') {
				pat += 3;
			}
			else {
				pat += 2;
			}
		}
		else {
			pat = signature_value;
			first_match = 0;
		}
	}
	return 0u;
}

#pragma warning(push)
#pragma warning(disable:4201)

typedef union {
	struct {
		uint64_t reserved1 : 3;
		uint64_t page_level_write_through : 1;
		uint64_t page_level_cache_disable : 1;
		uint64_t reserved2 : 7;
		uint64_t address_of_page_directory : 36;
		uint64_t reserved3 : 16;
	};
	uint64_t flags;
} cr3;
static_assert(sizeof(cr3) == 0x8);

typedef union {
	struct {
		uint64_t present : 1;
		uint64_t write : 1;
		uint64_t supervisor : 1;
		uint64_t page_level_write_through : 1;
		uint64_t page_level_cache_disable : 1;
		uint64_t accessed : 1;
		uint64_t dirty : 1;
		uint64_t large_page : 1;
		uint64_t global : 1;
		uint64_t ignored_1 : 2;
		uint64_t restart : 1;
		uint64_t page_frame_number : 36;
		uint64_t reserved1 : 4;
		uint64_t ignored_2 : 7;
		uint64_t protection_key : 4;
		uint64_t execute_disable : 1;
	};

	uint64_t flags;
} pt_entry_64;
static_assert(sizeof(pt_entry_64) == 0x8);
#pragma warning(pop)

static uint64_t pte_base = 0;
static uint64_t pde_base = 0;
static uint64_t ppe_base = 0;
static uint64_t pxe_base = 0;
static uint64_t self_mapidx = 0;
static uint64_t mm_pfn_database = 0;

uint64_t get_dirbase() {
	return __readcr3() & 0xFFFFFFFFFFFFF000;
}

void* phys_to_virt(uint64_t phys) {
	PHYSICAL_ADDRESS phys_addr = { .QuadPart = (int64_t)(phys) };
	return reinterpret_cast<void*>(MmGetVirtualForPhysical(phys_addr));
}

void init_pte_base() {
	cr3 system_cr3 = { .flags = get_dirbase() };
	uint64_t dirbase_phys = system_cr3.address_of_page_directory << 12;
	pt_entry_64* pt_entry = reinterpret_cast<pt_entry_64*>(phys_to_virt(dirbase_phys));
	for (uint64_t idx = 0; idx < 0x200; idx++) {
		if (pt_entry[idx].page_frame_number == system_cr3.address_of_page_directory) {
			pte_base = (idx + 0x1FFFE00ui64) << 39ui64;
			pde_base = (idx << 30ui64) + pte_base;
			ppe_base = (idx << 30ui64) + pte_base + (idx << 21ui64);
			pxe_base = (idx << 12ui64) + ppe_base;
			self_mapidx = idx;
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "PteBase 0x%llx\n", pte_base);
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "PdeBase 0x%llx\n", pde_base);
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "PpeBase 0x%llx\n", ppe_base);
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "PxeBase 0x%llx\n", pxe_base);
			DbgPrintEx(DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "idx 0x%llx\n", idx);

			break;
		}
	}
}

uintptr_t init_mmpfn_database() {
	auto search = search_pattern(reinterpret_cast<void*>(get_kernel_base()), ".text", "B9 ? ? ? ? 48 8B 05 ? ? ? ? 48 89 43 18") + 5;
	auto resolved_base = search + *reinterpret_cast<int32_t*>(search + 3) + 7;
	mm_pfn_database = *reinterpret_cast<uintptr_t*>(resolved_base);
	return mm_pfn_database;
}

// modified from https://github.com/Rythorndoran/enum_real_dirbase/tree/master
UINT64 get_process_cr3(PMEMORY_OPERATION_DATA x) {
	if (!pte_base) init_pte_base();
	if (!mm_pfn_database) init_mmpfn_database();
	auto mem_range_count = 0;
	auto mem_range = MmGetPhysicalMemoryRanges();

	auto cr3_ptebase = self_mapidx * 8 + pxe_base;

	for (mem_range_count = 0; mem_range_count < 200; ++mem_range_count)
	{
		if (mem_range[mem_range_count].BaseAddress.QuadPart == 0 && mem_range[mem_range_count].NumberOfBytes.QuadPart == 0)
			break;

		auto start_pfn = mem_range[mem_range_count].BaseAddress.QuadPart >> 12;
		auto end_pfn = start_pfn + (mem_range[mem_range_count].NumberOfBytes.QuadPart >> 12);

		for (auto i = start_pfn; i < end_pfn; ++i)
		{
			auto cur_mmpfn = reinterpret_cast<_MMPFN*>(mm_pfn_database + 0x30 * i);

			if (cur_mmpfn->flags)
			{
				if (cur_mmpfn->flags == 1)
					continue;

				if (cur_mmpfn->pte_address != cr3_ptebase)
					continue;

				auto decrypted_eprocess = ((cur_mmpfn->flags | 0xF000000000000000) >> 0xd) | 0xFFFF000000000000;
				dirBase = i << 12;
				PEPROCESS process;
				PsLookupProcessByProcessId((HANDLE)x->pid, &process);

				if (MmIsAddressValid(reinterpret_cast<void*>(decrypted_eprocess)) && reinterpret_cast<PEPROCESS>(decrypted_eprocess) == process)
				{
					if (dirBase) {
						RtlCopyMemory((void*)x->cr3, &dirBase, sizeof(dirBase));
						return STATUS_SUCCESS;
					}

					break;
				}
			}
		}
	}
}


UINT64 translate_linear(UINT64 directoryTableBase, UINT64 virtualAddress) {
	directoryTableBase &= ~0xf;

	UINT64 pageOffset = virtualAddress & ~(~0ul << PAGE_OFFSET_SIZE);
	UINT64 pte = ((virtualAddress >> 12) & (0x1ffll));
	UINT64 pt = ((virtualAddress >> 21) & (0x1ffll));
	UINT64 pd = ((virtualAddress >> 30) & (0x1ffll));
	UINT64 pdp = ((virtualAddress >> 39) & (0x1ffll));

	SIZE_T readsize = 0;
	UINT64 pdpe = 0;
	read(PVOID(directoryTableBase + 8 * pdp), &pdpe, sizeof(pdpe), &readsize);
	if (~pdpe & 1)
		return 0;

	UINT64 pde = 0;
	read(PVOID((pdpe & PMASK) + 8 * pd), &pde, sizeof(pde), &readsize);
	if (~pde & 1)
		return 0;

	if (pde & 0x80)
		return (pde & (~0ull << 42 >> 12)) + (virtualAddress & ~(~0ull << 30));

	UINT64 pteAddr = 0;
	read(PVOID((pde & PMASK) + 8 * pt), &pteAddr, sizeof(pteAddr), &readsize);
	if (~pteAddr & 1)
		return 0;

	if (pteAddr & 0x80)
		return (pteAddr & PMASK) + (virtualAddress & ~(~0ull << 21));

	virtualAddress = 0;
	read(PVOID((pteAddr & PMASK) + 8 * pte), &virtualAddress, sizeof(virtualAddress), &readsize);
	virtualAddress &= PMASK;

	if (!virtualAddress)
		return 0;

	return virtualAddress + pageOffset;
}

ULONG64 find_min(INT32 g, SIZE_T f) {
	INT32 h = (INT32)f;
	ULONG64 result = 0;

	result = (((g) < (h)) ? (g) : (h));

	return result;
}

NTSTATUS frw(prw x) {
	if (x->security != code_security)
		return STATUS_UNSUCCESSFUL;

	if (!x->process_id)
		return STATUS_UNSUCCESSFUL;

	PEPROCESS process = NULL;
	PsLookupProcessByProcessId((HANDLE)x->process_id, &process);
	if (!process)
		return STATUS_UNSUCCESSFUL;

	ULONGLONG process_base = dirBase;
	if (!process_base)
		return STATUS_UNSUCCESSFUL;
	ObDereferenceObject(process);

	SIZE_T this_offset = NULL;
	SIZE_T total_size = x->size;

	INT64 physical_address = translate_linear(process_base, (ULONG64)x->address + this_offset);
	if (!physical_address)
		return STATUS_UNSUCCESSFUL;

	ULONG64 final_size = find_min(PAGE_SIZE - (physical_address & 0xFFF), total_size);
	SIZE_T bytes_trough = NULL;

	if (x->write) {
		write(PVOID(physical_address), (PVOID)((ULONG64)x->buffer + this_offset), final_size, &bytes_trough);
	}
	else {
		read(PVOID(physical_address), (PVOID)((ULONG64)x->buffer + this_offset), final_size, &bytes_trough);
	}

	return STATUS_SUCCESS;
}

NTSTATUS fba(pba x) {
	ULONGLONG image_base = NULL;
	if (x->security != code_security)
		return STATUS_UNSUCCESSFUL;

	if (!x->process_id)
		return STATUS_UNSUCCESSFUL;

	PEPROCESS process = NULL;
	if (!NT_SUCCESS(PsLookupProcessByProcessId((HANDLE)x->process_id, &process))) {
		// Failed to lookup process by PID
		return STATUS_UNSUCCESSFUL;
	}

	image_base = (ULONGLONG)PsGetProcessSectionBaseAddress(process);

	if (!image_base) {
		// Invalid image base address
		ObDereferenceObject(process);
		return STATUS_UNSUCCESSFUL;
	}

	RtlCopyMemory(x->address, &image_base, sizeof(image_base));
	ObDereferenceObject(process);

	return STATUS_SUCCESS;
}

NTSTATUS fget_guarded_region(pga x) {
	if (x->security != code_security)
		return STATUS_UNSUCCESSFUL;

	ULONG infoLen = 0;
	NTSTATUS status = ZwQuerySystemInformation(SystemBigPoolInformation, &infoLen, 0, &infoLen);
	PSYSTEM_BIGPOOL_INFORMATION pPoolInfo = 0;

	while (status == STATUS_INFO_LENGTH_MISMATCH)
	{
		if (pPoolInfo)
			ExFreePool(pPoolInfo);

		pPoolInfo = (PSYSTEM_BIGPOOL_INFORMATION)ExAllocatePool(NonPagedPool, infoLen);
		status = ZwQuerySystemInformation(SystemBigPoolInformation, pPoolInfo, infoLen, &infoLen);
	}

	if (pPoolInfo)
	{
		for (unsigned int i = 0; i < pPoolInfo->Count; i++)
		{
			SYSTEM_BIGPOOL_ENTRY* Entry = &pPoolInfo->AllocatedInfo[i];
			PVOID VirtualAddress;
			VirtualAddress = (PVOID)((uintptr_t)Entry->VirtualAddress & ~1ull);
			SIZE_T SizeInBytes = Entry->SizeInBytes;
			BOOLEAN NonPaged = Entry->NonPaged;

			if (Entry->NonPaged && Entry->SizeInBytes == 0x200000) {
				UCHAR expectedTag[] = "TnoC";  // Tag should be a string, not a ulong
				if (memcmp(Entry->Tag, expectedTag, sizeof(expectedTag)) == 0) {
					RtlCopyMemory((void*)x->address, &Entry->VirtualAddress, sizeof(Entry->VirtualAddress));
					return STATUS_SUCCESS;
				}
			}

		}

		ExFreePool(pPoolInfo);
	}

	return STATUS_SUCCESS;
}

NTSTATUS io_controller(PDEVICE_OBJECT device_obj, PIRP irp) {
	UNREFERENCED_PARAMETER(device_obj);

	NTSTATUS status = { };
	ULONG bytes = { };
	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(irp);

	ULONG code = stack->Parameters.DeviceIoControl.IoControlCode;
	ULONG size = stack->Parameters.DeviceIoControl.InputBufferLength;

	if (code == code_rw) {
		if (size == sizeof(_rw)) {
			prw req = (prw)(irp->AssociatedIrp.SystemBuffer);

			status = frw(req);
			bytes = sizeof(_rw);
		}
		else
		{
			status = STATUS_INFO_LENGTH_MISMATCH;
			bytes = 0;
		}
	}
	else if (code == code_ba) {
		if (size == sizeof(_ba)) {
			pba req = (pba)(irp->AssociatedIrp.SystemBuffer);

			status = fba(req);
			bytes = sizeof(_ba);
		}
		else
		{
			status = STATUS_INFO_LENGTH_MISMATCH;
			bytes = 0;
		}
	}
	else if (code == code_get_guarded_region) {
		if (size == sizeof(_ga)) {
			pga req = (pga)(irp->AssociatedIrp.SystemBuffer);

			status = fget_guarded_region(req);
			bytes = sizeof(_ga);
		}
		else
		{
			status = STATUS_INFO_LENGTH_MISMATCH;
			bytes = 0;
		}
	}
	else if (code == code_GetDirBase) {
		PMEMORY_OPERATION_DATA req = (PMEMORY_OPERATION_DATA)(irp->AssociatedIrp.SystemBuffer);

		status = get_process_cr3(req);
	}

	irp->IoStatus.Status = status;
	irp->IoStatus.Information = bytes;
	IoCompleteRequest(irp, IO_NO_INCREMENT);

	return status;
}

NTSTATUS unsupported_dispatch(PDEVICE_OBJECT device_obj, PIRP irp) {
	UNREFERENCED_PARAMETER(device_obj);

	irp->IoStatus.Status = STATUS_NOT_SUPPORTED;
	IoCompleteRequest(irp, IO_NO_INCREMENT);

	return irp->IoStatus.Status;
}

NTSTATUS dispatch_handler(PDEVICE_OBJECT device_obj, PIRP irp) {
	UNREFERENCED_PARAMETER(device_obj);

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(irp);

	switch (stack->MajorFunction) {
	case IRP_MJ_CREATE:
		break;
	case IRP_MJ_CLOSE:
		break;
	default:
		break;
	}

	IoCompleteRequest(irp, IO_NO_INCREMENT);
	return irp->IoStatus.Status;
}

void unload_drv(PDRIVER_OBJECT drv_obj) {
	NTSTATUS status = { };

	status = IoDeleteSymbolicLink(&link);

	if (!NT_SUCCESS(status))
		return;

	IoDeleteDevice(drv_obj->DeviceObject);
}

NTSTATUS initialize_driver(PDRIVER_OBJECT drv_obj, PUNICODE_STRING path) {
	UNREFERENCED_PARAMETER(path);

	NTSTATUS status = STATUS_SUCCESS;
	PDEVICE_OBJECT device_obj = NULL;

	UNICODE_STRING name, link;
	RtlInitUnicodeString(&name, L"\\Device\\sofmainud1337"); // driver name
	RtlInitUnicodeString(&link, L"\\DosDevices\\sofmainud1337"); // driver name

	// Create the device
	status = IoCreateDevice(drv_obj, 0, &name, FILE_DEVICE_UNKNOWN, FILE_DEVICE_SECURE_OPEN, FALSE, &device_obj);
	if (!NT_SUCCESS(status)) {
		return status;
	}

	// Create a symbolic link
	status = IoCreateSymbolicLink(&link, &name);
	if (!NT_SUCCESS(status)) {
		IoDeleteDevice(device_obj);
		return status;
	}

	// Set up IRP dispatch functions
	for (int i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++) {
		drv_obj->MajorFunction[i] = &unsupported_dispatch;
	}

	drv_obj->MajorFunction[IRP_MJ_CREATE] = &dispatch_handler;
	drv_obj->MajorFunction[IRP_MJ_CLOSE] = &dispatch_handler;
	drv_obj->MajorFunction[IRP_MJ_DEVICE_CONTROL] = &io_controller;
	drv_obj->DriverUnload = &unload_drv;

	// Configure device flags
	device_obj->Flags |= DO_BUFFERED_IO;
	device_obj->Flags &= ~DO_DEVICE_INITIALIZING;

	return status;
}


NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);

	return IoCreateDriver(NULL, &initialize_driver);
}