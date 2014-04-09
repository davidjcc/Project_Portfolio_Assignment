#include "Game.h"
#include "key_codes.h"

// tell the compiler to always link into a Winmain, even if we are using a console app
#pragma comment (linker, "/ENTRY:WinMainCRTStartup") 

/*
The following code tells the linker whether to create a console or not. If we are debugging logic,
we can easily switch to console output and print debug messages to the console.
*/

// comment this out for OGL usage.
//#define _CONSOLEONLY_

#ifdef _CONSOLEONLY_
	#pragma comment (linker, "/SUBSYSTEM:CONSOLE") // Show console output in debug mode 
#else 
	#pragma comment (linker, "/SUBSYSTEM:WINDOWS") 
#endif 

Game g_Game;

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
	switch(uMsg){
		//KEY DOWN
		case WM_KEYDOWN:
			switch(wParam)	
			{
				case VK_ESCAPE:
					PostQuitMessage(0);
					break;
				case VK_B:
					if(g_Game.player->getBBRender())
						g_Game.player->setBBRender(false);
					else
						g_Game.player->setBBRender(true);	
					break;
				case VK_SPACE:
					g_Game.player->setJumping(true);
					break;
				case VK_SHIFT:
						g_Game.player->setSprinting(true);
					break;
				case VK_L:
					if(g_Game.drawLight)
						g_Game.drawLight = false;
					else
						g_Game.drawLight = true;
					break;
				case VK_W:
					g_Game.player->setVel(WALK_SPD, 0, 0);
					g_Game.player->setWalking(true);
					break;
				case VK_S:
					g_Game.player->rotate(Vector(0, 0, 1));
					g_Game.player->setVel(-WALK_SPD, 0, 0);
					g_Game.player->setWalking(true);
					break;
				case VK_A:
					g_Game.player->setVel(0, 0, -WALK_SPD);
					g_Game.player->setWalking(true);
					break;
				case VK_D:
					g_Game.player->setVel(0 ,0, WALK_SPD);
					g_Game.player->setWalking(true);
					break;
				case VK_F:
					break;
				default:
					break;
			}
			break;
		case WM_KEYUP:
			switch(wParam)	
			{
				case VK_W:
					g_Game.player->stopMoving();
					break;
				case VK_S:
					g_Game.player->stopMoving();
					break;
				case VK_A:
					g_Game.player->stopMoving();
					break;
				case VK_D:
					g_Game.player->stopMoving();
					break;
				case VK_SPACE:
					g_Game.player->setJumping(false);
					break;
				case VK_SHIFT:
					g_Game.player->setSprinting(false);
					break;
				default:
					break;
			}
			break;
		case WM_DESTROY:
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
		case WM_SIZE:
			g_Game.Resize(LOWORD(lParam), HIWORD(lParam));
		break;
		case WM_MOUSEMOVE:
			// Track the mouse position
			g_Game.mouseX = LOWORD(lParam);
			g_Game.mouseY = HIWORD(lParam);
			if(g_Game.picking == false)
				g_Game.CameraPos();
		break;
		case WM_MOUSEWHEEL:
			if(GET_WHEEL_DELTA_WPARAM(wParam) < 0)  //chk dir of scroll wheel
				g_Game.camRad *= 1.1f;
			else
				g_Game.camRad /= 1.1f;
		break;
		case WM_LBUTTONDOWN:
		break;
		case WM_LBUTTONUP:
		break;
		case WM_RBUTTONDOWN: // zoom out
			//DebugOut("Right mouse button clicked");
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/**
This is the starting point for the application
*/

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int iCmdShow){
	MSG msg;
	bool done = false;

	// dont create a screen if we are in "console output" mode
#ifndef _CONSOLEONLY_
	g_Game.CreateScreen("Lighting example", SCRN_W, SCRN_H, 32, false);
	g_Game.InitOpenGL();
#endif

	g_Game.Initialise();

	// ===========================================================================
	//            THE MAIN GAME LOOP
	// ===========================================================================
	while(!done){
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
			if(msg.message == WM_QUIT){
				done = true;
			}else{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}else{
				// the loop that updates the game..
				g_Game.Update();
				// dont perform graphical updates if we are in "console" mode
#ifndef _CONSOLEONLY_
				g_Game.Render();
				g_Game.Swap();
#endif
		}
	}

// don't need to release any screens if we are using a console.
#ifndef _CONSOLEONLY_
	g_Game.ReleaseScreen();
#endif
	
	g_Game.Shutdown();

	return msg.wParam;
}
