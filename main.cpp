//SIMPLE ASS CODE FOR YALL TO UNDERSTAND EVERYTHING! IF YOU CANT UNDERSTAND YOU A GIGA PASTER

//PLEASE DONT SELL THIS SOURCE PLEASEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE

//CAUSE I KNOW YOU A PASTER AND DONT KNOW ANYTHING JUST WANNA CHANGE MENU AND NAME AND MAKE THIS YOURS WITHOUT HAVING ANY KNOWLEDGE I AM LETTING YOU KNOW THAT IF YOU CHANGE DRIVER YOU WILL HAVE TO CHANGE SOME THINGS IN HERE AND IN game.hpp and in driver.hpp SO JUST KEEP THAT IN MIND!
#include "includes.hpp"
#include <iostream>
#include <thread>


//NO THIS HIGHLIGHTS ARE NOT CHATGPT, I MADE THEM TO HELP YALL YNS TO UNDERSTAND WHAT EACH SHIT DOES, SO THAT YALL CAN LEARN AND NOT JUST PASTE.


// DONT USE ANY SHITTY CR3 LOOP FIX OR WHATEVER UR CHEAT WILL LAG SO BAD. IF UR DRIVER NEED CR3 LOOP THEN USE OTHER DRIVER OR FIX IT.
void main()
{
	printf("Waiting for Fortnite...");

	while (game_wnd == 0)
	{
		Sleep(1);
		sofmain1337::process_id = sofmain1337::find_process(L"FortniteClient-Win64-Shipping.exe"); //waiting for proccess
		game_wnd = get_process_wnd(sofmain1337::process_id); //found the process
		Sleep(1);
	}

	



	system("cls");
	printf("SOFMAIN FN BASE V4");

	if (!input::init())
	{
		printf("The input was not initialized :("); //input wasnt initialized please go to definition and check //INPUT MEANS THE MOUSE AIM I USE
		Sleep(3000);
		exit(0);
	}

	if (!gui::init())
	{
		printf("The gui was not initialized :("); //init/gui wasnt initialized please go to definition and check // GUI MEANS THE OVERLAY I USE
		Sleep(3000);
		exit(0);
	}
	if (!sofmain1337::find_driver())
	{
		printf("The driver was not initialized :("); //driver wasnt initialized please go to definition and check or check ur shitty pc OR YOU JUST FCKED THE COMMS IDK JUST DOUBLE CHECK EVERYTHING FOR THIS ONE!
		Sleep(3000);
		exit(0);
	}

	//DONT TOUCH HERE PASTER! TOUCH ONLY IF YOU WANNA CHANGE DRIVER OR IF YOU KNOW WHAT YOU ARE DOING! -SOFMAIN
	sofmainisud = sofmain1337::find_image(); //finding img / base
	auto cr3 = sofmain1337::fetch_cr3(); // CR3 SHIT

	if (!sofmain1337::driver_handle) //base_address //driver_handle
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


