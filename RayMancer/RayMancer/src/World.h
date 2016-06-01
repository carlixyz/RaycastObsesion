
#ifndef WORLD_H
#define WORLD_H

//#include <cstdint>
#include "AppManager.h"
#include "Player.h"
#include <vector>
#include <iostream>


#define MAP_WIDTH  24
#define MAP_HEIGHT 24
#define MAP_AREA MAP_WIDTH*MAP_HEIGHT
#define BLOCK_SIZE 64

//struct Entity;	// foward decl

struct World
{
	int width = MAP_WIDTH;
	int height = MAP_HEIGHT;
	std::vector<int> data;

	bool Init();
	bool Init(std::string fileName);
	void Update(AppManager &mainApp);
	void Deinit();
	
	void RasterMap(PixelBuffer &dst);

	inline int GetBlock(int x, int y)
	{
		if (y >= MAP_HEIGHT || x >= MAP_WIDTH)	// Avoid unallowed readings inside vector
			return 0;

		return (data[(int)(x + y*MAP_WIDTH)]);
	};

	// MINIMAP
	bool	mapShow = false;
	int		mapScale = 8;						// default 32
	float	mapFactor = 1;
	PixelBuffer mapBuffer;						// Map Pixels PreBuffer
	PixelBuffer mapResized;						// Main Window PreBuffer
	void SetMiniMapScale(float scale);	
	void DrawPlayerMap(Player &player);


	//Ray cast(Entity &player, float angle, float range);
};


#endif