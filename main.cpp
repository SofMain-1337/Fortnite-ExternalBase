//SIMPLE ASS CODE FOR YALL TO UNDERSTAND EVERYTHING! IF YOU CANT UNDERSTAND YOU A GIGA PASTER

//PLEASE DONT SELL THIS SOURCE PLEASEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE

//CAUSE I KNOW YOU A PASTER AND DONT KNOW ANYTHING JUST WANNA CHANGE MENU AND NAME AND MAKE THIS YOURS WITHOUT HAVING ANY KNOWLEDGE I AM LETTING YOU KNOW THAT IF YOU CHANGE DRIVER YOU WILL HAVE TO CHANGE SOME THINGS IN HERE AND IN game.hpp and in driver.hpp SO JUST KEEP THAT IN MIND!
#include "includes.hpp"
#include <iostream>
#include <thread>


void cr3_loop()
{
	//UPDATED CR3 CAUSE U WOULDNT KNOW HOW TO UPDATE IT CAUSE YALL GIGA PASTERS UwU -SOFMAIN //KINDA SHIT AND LAGGY BUT WORKS AND ITS OKAY TO USE
	for (;;)
	{
		mem::fetch_cr3();
		std::this_thread::sleep_for(std::chrono::milliseconds(000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001)); //100
	}
}

void main()
{
	printf("Waiting for Fortnite...");

	while (game_wnd == 0)
	{
		Sleep(1);
		mem::process_id = mem::find_process(L"FortniteClient-Win64-Shipping.exe"); //searching for process
		game_wnd = get_process_wnd(mem::process_id); //found the process
		Sleep(1);
	}

	



	system("cls");
	printf("SOFMAIN FN BASE");

	if (!input::init())
	{
		printf("The input was not initialized :("); //input wasnt initialized please go to definition and check or check ur shitty pc !
		Sleep(3000);
		exit(0);
	}

	if (!gui::init())
	{
		printf("The gui was not initialized :("); //init/gui wasnt initialized please go to definition and check or check ur shitty pc !
		Sleep(3000);
		exit(0);
	}
	if (!mem::find_driver())
	{
		printf("The driver was not initialized :("); //driver wasnt initialized please go to definition and check or check ur shitty pc OR YOU JUST FCKED THE COMMS IDK JUST DOUBLE CHECK EVERYTHING FOR THIS ONE!
		Sleep(3000);
		exit(0);
	}

	//DONT TOUCH HERE PASTER! TOUCH ONLY IF YOU WANNA CHANGE DRIVER OR IF YOU KNOW WHAT YOU ARE DOING! -SOFMAIN
	virtualaddy = mem::find_image(); //finding img
	cr3 = mem::fetch_cr3(); //loading cr3
	std::thread([&]() { for (;;) { cr3_loop(); } }).detach(); //for UPDATED CR3 ( Shitty method btw laggy af )
	if (!mem::driver_handle) //base_address //driver_handle
	{
		printf("The driver couldn't get the base address"); //Check ur driver base in the code if u get this error! Mine doesnt have a base_address so its driver_handle
		Sleep(3000);
		exit(0);
	}
	//here is for menu overlay blah blah
	create_overlay(); 
	directx_init();
	render_loop();

}


