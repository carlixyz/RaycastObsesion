
#include <assert.h>
#include <cstdint>
#include "PixelToaster\PixelToaster.h"
//#include "main.h"
#include "AppManager.h"
#include "World.h"
#include "Player.h"


using namespace std;
using namespace PixelToaster;

DWORD *WinRef;

AppManager MainApp;
PixelBuffer backBuffer(SCREEN_W >> 1, SCREEN_H >> 1);						// Main Window PreBuffer
World mapTest;
Player player;
//#include	<iostream>


void main()
{
	//std::cout << backBuffer.Width << " " << backBuffer.Height << "\n";

	WinRef = MainApp.OpenWindow("RayMancer", SCREEN_W, SCREEN_H);
	assert(WinRef != NULL );
	
	mapTest.Init();    	//mapTest.RasterMap(mapBuffer);

	while (true)
	{
		backBuffer.Cls();

		if (MainApp.KeyPressed(Key::Q) || MainApp.WindowClosed() )
			break;
		player.Update(MainApp);
		mapTest.Update(MainApp);


		player.DrawWalls(mapTest, backBuffer);

		if (mapTest.mapShow)
		{
			mapTest.DrawPlayerMap(player);
			backBuffer.Draw(mapTest.mapResized, 10, 10);    //backBuffer.Draw(mapTest.mapBuffer, 10, 10);
		}


		MainApp.UpdateWindowX2(backBuffer);
	}

	//HKFree();
	mapTest.Deinit();
	MainApp.CloseWindow();

}


