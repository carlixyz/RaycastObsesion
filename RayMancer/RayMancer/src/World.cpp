
#include "World.h"
//#include <assert.h>
#include <cstdio>


bool World::Init()
{
	//width = MAP_WIDTH;
	//height = MAP_HEIGHT;
	data = {
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
		1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1,
		1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
	};

	mapBuffer.Resize(width * mapScale, height* mapScale);
	RasterMap(mapBuffer);
	mapResized = mapBuffer;
	return true;
}

bool World::Init(std::string fileName)
{
	return true;
}

void World::Deinit()
{
	data.clear();
}

void World::SetMiniMapScale(float scale)
{
	mapFactor += scale;
	mapResized = mapBuffer;
	mapResized.ReScale(mapFactor);
}

void World::Update(AppManager &mainApp)
{
	if (mainApp.KeyHit(Key::Tab))
		mapShow = !mapShow;

	if (mainApp.KeyPressed(Key::PageUp))
		SetMiniMapScale(+0.01f);

	if (mainApp.KeyPressed(Key::PageDown))
		SetMiniMapScale(-0.01f);

}

void World::RasterMap(PixelBuffer &dst)
{
	double scaleWidth = (double)dst.Width / (double)width;
	double scaleHeight = (double)dst.Height / (double)height;

	for (int cy = 0; cy < dst.Height; cy++)
	for (int cx = 0; cx < dst.Width; cx++)
	{
		int pixel = (cy * (dst.Width)) + (cx);
		int nearestMatch = (((int)(cy / scaleHeight) * (width)) + ((int)(cx / scaleWidth)));

		//dst[pixel] = (src.data[nearestMatch] != 0 ? 0x00FF00FF : 0x0000FF00);
		dst[pixel] = (data[nearestMatch] != 0 ? 0x008800FF : (900 * !(cx % mapScale && cy % mapScale)));
		//dst[pixel] -= ~120 * (cx % Scale && cy % Scale);	// 16
	}
}

void World::DrawPlayerMap(Player &player)
{
	mapResized = mapBuffer;
	float px = (player.posX * mapScale) /BLOCK_SIZE;
	float py = (player.posY * mapScale) / BLOCK_SIZE;
	/*float px = (player.posX / BLOCK_SIZE) * mapScale;
	float py = (player.posY  / BLOCK_SIZE)* mapScale;*/

	//Pixel ZoneColor(0, .75f, .75f);
	DWORD ZoneColor = 0x000088FF;
	DWORD DirColor = 0x0033BB66;
	float Pzone = .2f * mapScale;		// A little offset 2 draw a small area around Player
	float FrontLength = 2 * mapScale;		// The offset 2 draw the foward Vector  in front of Player view
	float FrontX = (px + (cos(player.Ang) * FrontLength)) ;
	float FrontY = (py + (sin(player.Ang) * FrontLength));
	//float FrontY = (py - (sin(player.Ang) * FrontLength));

	player.DrawRect((px - Pzone), (py - Pzone), (px + Pzone), (py + Pzone), mapResized, ZoneColor);
	player.DrawLine(px , py , FrontX, FrontY, mapResized, DirColor, 1);

	mapResized.ReScale(mapFactor);
}





//bool World::isBlocked(int x, int y)
//{
//	if (y < 0 || y >= height || x < 0 || x >= width)
//		return true;			//make sure that we cannot move outside level
//	// Return true if the map block is not 0, (if there is a blocking wall)
//	return (data[(int)(x + y*width)]);
//}

//Ray World::cast(Entity &player, float angle, float range)
//{
//
//	return Ray();
//}