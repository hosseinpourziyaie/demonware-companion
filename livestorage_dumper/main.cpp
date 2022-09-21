/************************************************************************************
_____________________________DEMONWARE COMPANION______________________________******
***********************************************************************************
**
** - Name        : LiveStorage Dump Tool
** - Description : starting point to application
**
** - Author      : Hosseinpourziyaie
** - Started on  : 2022-09-15     | Ended on : 2022-09-20
** - Note        : To successfully dump files they must have been fetched by game
**                 (snapshot should be taken after DW is connected to lobby service)
**
**
** [Copyright © Hosseinpourziyaie 2022] <hosseinpourziyaie@gmail.com>
**
************************************************************************************/
#include "stdinc.h"

void print_error(procces_fetch_error err, std::string extra_info = "")
{
	switch (err)
	{
	case COULDNT_FIND_PROCESS:
		std::cout << "[ERROR] " << "Couldnt find any supported game instance; supported games as of now: BO3, MWR, WWII" << std::endl;
		break;
	case COULDNT_OPEN_HANDLE:
		std::cout << "[ERROR] " << "Found supported process " << extra_info << " but couldnt open handle to it" << std::endl;
		break;
	case COULDNT_TARGET_MODULE:
		std::cout << "[ERROR] " << "Found supported process " << extra_info << " but couldnt target main module" << std::endl;
		break;
	default:
		break;
	}
	std::cout << std::endl << "[HELP] " << " If you are positive on having supported game process running in background, try running tool As Adminstrator" << std::endl;
}

void InitializeSupportedTitlesList()
{
	SupportedTitlesList.insert({ "h1_mp64_ship.exe", &ModernWarfareRemastered::StartUpExtraction });
	SupportedTitlesList.insert({ "s2_mp64_ship.exe", &WorldWarII::StartUpExtraction });
	SupportedTitlesList.insert({ "BlackOps3.exe", &BlackOps3::StartUpExtraction });
	SupportedTitlesList.insert({ "iw7_ship.exe", &InfiniteWarfare::StartUpExtraction });	
}

void SearchForSupportedGameInstance()
{
	ProcHex proc; // Initialize proccess handler class object

	for (auto const& pair : SupportedTitlesList) {
		if (proc.FindProcess(pair.first))
		{
			if (proc.OpenHandle())
			{
				if (proc.TargetModule(pair.first))
				{
					std::cout << "------------------------------------------------------------------------------------------------" << std::endl;
					std::cout << "[INFO] " << "found supported game: " << pair.first << " -- BaseAddr: 0x" << std::hex << std::uppercase << proc.moduleBase << " -- ImgSize: 0x" << proc.moduleSize << std::dec << "" << std::endl << std::endl;
					std::cout << "------------------------------------------------------------------------------------------------" << std::endl;

					pair.second(proc); return; // Callback to relative game's Handler function
				}
				else
				{
					print_error(COULDNT_TARGET_MODULE, pair.first); return; // Exception in Targeting Module
				}
			}
			else
			{
				print_error(COULDNT_OPEN_HANDLE, pair.first); return; // Exception in Opening Handle
			}
		}
	}

	print_error(COULDNT_FIND_PROCESS);  // Failure in Finding Target Process
}


int main()
{
	InitializeSupportedTitlesList();
	SearchForSupportedGameInstance();


	system("pause"); // press any key to Exit ...
}

