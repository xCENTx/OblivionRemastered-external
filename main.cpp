
#include "menu.h"

static int LastTick = 0;
int mainthread();

int main()
{
	//  Initialize game data
	g_Oblivion = std::make_unique<TESOblivion>();

	//	Initialize Menu
	g_Menu = std::make_unique<Menu>();

	//	Initialize d3d window
	g_dxWindow = std::make_unique<DxWindow>();
	g_dxWindow->Init();

	//	Initialize Background Thread
	std::thread wcm(mainthread);

	//	Main Loop
	while (g_Menu->bRunning)
	{
		bool bTimer = GetTickCount64() - LastTick > 500;
		if (GetAsyncKeyState(VK_INSERT) & 0x8000 && bTimer)
		{
			std::this_thread::sleep_for(60ms);	//	wait for key input to pass in game thread

			g_Menu->bShowMenu ^= 1;
			g_Menu->UpdateOverlayViewState(g_Menu->bShowMenu);
			switch (g_Menu->bShowMenu)
			{
				case(true): 
					g_dxWindow->SetWindowFocus(g_dxWindow->GetWindowHandle());	//	focus overlay window
					SetCursorPos(
						int(g_Menu->GetImGuiMenuBounds().GetCenter().x),
						int(g_Menu->GetImGuiMenuBounds().GetCenter().y)
					);	//	set cursor to center of screen every time menu is shown
					break;
				case(false): 
					g_dxWindow->SetWindowFocus(g_memory.GetProcessInfo().hWnd); //	focus game window
					break;
				default: break;
			}

			LastTick = GetTickCount64();
		}

		g_dxWindow->CloneUpdate(g_memory.GetProcessInfo().hWnd);	//	refresh game window info
		g_dxWindow->Update(g_Menu->GetOverlay());					//	update overlay

		//	std::this_thread::sleep_for(1ms);
		std::this_thread::yield();
	}
	wcm.join();

	g_dxWindow->Shutdown();
	g_Oblivion->shutdown();

	return EXIT_SUCCESS;
}

int mainthread()
{
	while (g_Menu->bRunning)
	{
		g_Oblivion->update();						//	update game info
		
		//	std::this_thread::sleep_for(1ms);
		std::this_thread::yield();
	}

	return EXIT_SUCCESS;
}