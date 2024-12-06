#pragma once

#include <winternl.h>
#include <intrin.h>
#include <Windows.h>
#include <iostream>
#include <TlHelp32.h>
#include <stdint.h>

inline uintptr_t Base;

#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)

#define IOCTL_GET_DIRECTORY_BASE_ADDRESS CTL_CODE(FILE_DEVICE_UNKNOWN, 0xF7A3, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_READ_WRITE_OPERATION CTL_CODE(FILE_DEVICE_UNKNOWN, 0x9B1E, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_MOUSE_MOVEMENT CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1D7C, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define IOCTL_GET_BASE_ADDRESS CTL_CODE(FILE_DEVICE_UNKNOWN, 0xD835, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define SECURITY_FLAG 0x7F1E8B

#define MOUSE_LEFT_BUTTON_DOWN   0x0001  // Left Button changed to down.
#define MOUSE_LEFT_BUTTON_UP     0x0002  // Left Button changed to up.

inline static bool EAC = true;

typedef struct _ReadWrite
{
	INT32 Security;
	INT32 ProcessID;
	ULONGLONG Address;
	ULONGLONG Buffer;
	ULONGLONG Size;
	BOOLEAN Write;
	BOOLEAN EAC;
} RW, * PRW;

typedef struct _MU
{
	long x;
	long y;
	unsigned short button_flags;
};

typedef struct _DTB
{
	INT32 Security;
	INT32 ProcessID;
	bool* Operation;
} DTB, * DTBL;

typedef struct _BA
{
	INT32 Security;
	INT32 ProcessID;
	ULONGLONG* Address;
} BA, * PBA;

namespace Driver
{
	inline HANDLE DriverHandle;
	inline INT32 ProcessID;

	inline bool Init()
	{
		DriverHandle = CreateFileA(("\\\\.\\\{jeremyownsyou}"), GENERIC_READ | GENERIC_WRITE, 0, 0, 3, 0x00000080, 0);

		if (!DriverHandle || (DriverHandle == INVALID_HANDLE_VALUE))

			return false;

		return true;
	}

	inline void ReadPhysicalMemory(PVOID address, PVOID buffer, DWORD size)
	{
		_ReadWrite Arguments = { 0 };
		Arguments.Security = SECURITY_FLAG;
		Arguments.Address = (ULONGLONG)address;
		Arguments.Buffer = (ULONGLONG)buffer;
		Arguments.Size = size;
		Arguments.ProcessID = ProcessID;
		Arguments.Write = FALSE;
		if (EAC)
			Arguments.EAC = TRUE;
		else
			Arguments.EAC = FALSE;

		DeviceIoControl(DriverHandle, IOCTL_READ_WRITE_OPERATION, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);
	}

	inline void WritePhysicalMemory(PVOID address, PVOID buffer, DWORD size)
	{
		_ReadWrite Arguments = { 0 };
		Arguments.Security = SECURITY_FLAG;
		Arguments.Address = (ULONGLONG)address;
		Arguments.Buffer = (ULONGLONG)buffer;
		Arguments.Size = size;
		Arguments.ProcessID = ProcessID;
		Arguments.Write = TRUE;
		if (EAC)
			Arguments.EAC = TRUE;
		else
			Arguments.EAC = FALSE;

		DeviceIoControl(DriverHandle, IOCTL_READ_WRITE_OPERATION, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);
	}

	inline bool CR3()
	{
		bool Ret = false;
		_DTB arguments = { 0 };
		arguments.Security = SECURITY_FLAG;
		arguments.ProcessID = ProcessID;
		arguments.Operation = (bool*)&Ret;

		if (EAC)
		{
			DeviceIoControl(DriverHandle, IOCTL_GET_DIRECTORY_BASE_ADDRESS, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
		}
		else
		{
			return true;
		}

		return Ret;
	}

	inline uintptr_t GetBase()
	{
		uintptr_t image_address = { NULL };
		_BA Arguments = { NULL };

		Arguments.Security = SECURITY_FLAG;
		Arguments.ProcessID = ProcessID;
		Arguments.Address = (ULONGLONG*)&image_address;
		DeviceIoControl(DriverHandle, IOCTL_GET_BASE_ADDRESS, &Arguments, sizeof(Arguments), nullptr, NULL, NULL, NULL);

		return image_address;
	}

	inline INT32 FindProcess(LPCTSTR process_name)
	{

		PROCESSENTRY32 pt;
		HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		pt.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hsnap, &pt)) {
			do {
				if (!lstrcmpi(pt.szExeFile, process_name))
				{
					CloseHandle(hsnap);
					ProcessID = pt.th32ProcessID;
					return pt.th32ProcessID;
				}
			} while (Process32Next(hsnap, &pt));
		}

		CloseHandle(hsnap);
		return ProcessID;
	}
}

template <typename T>
inline T read(uint64_t Address)
{
	T Buffer{ };
	Driver::ReadPhysicalMemory((PVOID)Address, &Buffer, sizeof(T));
	return Buffer;
}

template <typename T>
inline T write(uint64_t Address, T Buffer)
{
	Driver::WritePhysicalMemory((PVOID)Address, &Buffer, sizeof(T));
	return Buffer;
}