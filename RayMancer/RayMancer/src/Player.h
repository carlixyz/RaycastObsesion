#ifndef PLAYER_H
#define PLAYER_H

#include "AppManager.h"


//#define MOVE_SPEED 1 
#define MOVE_SPEED 10 
#define TURN_SPEED 0.05f

#define FOV 1.04719755128f			// RAD(60);

#define PI_TWO 1.57079632679f
#define PI     3.14159265358f 
#define TWO_PI 6.28318530718f 
const float radians = PI / 180;
const float degrees = 180 / PI;
#define RAD(ang) ang*radians
#define DEG(ang) ang*degrees

#define COL_RED   0xFF0000
#define COL_GREEN 0x00FF00


//#include "World.h"
struct World;	// Fowrd DECL 

struct Ray 
{
	float x;
	float y;
	float xStep;
	float yStep;

};



struct Player
{
	int posX = 128;			// Current x, y position of the player
	int posY = 128;			// 
	float Ang = RAD(60);			// current angle of rotation
	bool eyeFish = false;

	//float Dir;			// Input Enabler: The direction that the player is turning, -1 for left or 1 for right
	//float Move;			// Input Enabler: playing moving forward(speed = 1) or backwards (speed = -1).

	//float MoveSpeed;	// Move Multiplier: How far (in map units) does the player move each step/update
	//float RotSpeed;		// Rot Multiplier: How much does the player rotate each step/update (in radians)

	void Update(AppManager &mainApp);

	inline void WrapAngle(float &ang)    	//inline void wrap(float &angle)
	{
		ang -= TWO_PI * (ang > TWO_PI);     //ang = fmodf(ang, TWO_PI);
		ang += TWO_PI * (ang < 0);
	}
	inline int Clamp(int n, int lower, int upper)
	{
		return n + ((n < lower) * (lower - n)) + ((n > upper) * (upper - n));
	}
	inline float Distance(int x1, int y1, int x2, int y2)
	{
		int dX = x2 - x1;
		int dY = y2 - y1;
		return sqrtf((dX * dX) + (dY * dY));
	}
	inline bool DrawRect(int x1, int y1, int x2, int y2, PixelBuffer &dst, const DWORD& color = 0x00888888)
	{

		if (x1 < 0 || x1 > dst.Width - 1 || x2 < 0 || x2 > dst.Width - 1 || y1 < 0 || y1 > dst.Height - 1 || y2 < 0 || y2 > dst.Height - 1)
			return 0;

		int xEnd = (x1 > x2 ? x1 : x2);
		int yEnd = (y1 > y2 ? y1 : y2);

		int xStart = (x1 < x2 ? x1 : x2);
		int yStart = (y1 < y2 ? y1 : y2);

		for (int yIndex = yStart; yIndex < yEnd; yIndex++)
		for (int xIndex = xStart; xIndex < xEnd; xIndex++)
			dst.Data[xIndex + yIndex * dst.Width].integer += color;

		return 1;
	}
	void DrawLine(int x1, int y1, int x2, int y2, PixelBuffer &dst, const DWORD & color = 0x00888888, int brush = 2);
	inline bool VerLine(int sx, int sy1, int sy2, PixelBuffer &Dst, const DWORD& color = 0x00888888)
	{
		if (sy2 < sy1)
		{
			sy1 += sy2; sy2 = sy1 - sy2; sy1 -= sy2;
		} //swap y1 and y2
		if (sy2 < 0 || sy1 >= Dst.Height || sx < 0 || sx >= Dst.Width)
			return 0; //no single point of the line is on screen

		if (sy1 < 0)  sy1 = 0;					//clip top
		if (sy2 >= Dst.Width) sy2 = Dst.Height - 1;	//clip bottom

		for (int y = sy1; y < sy2; y++)
		{
			if (y < 0) continue;
			if (y >= Dst.Height) break;

			if (sx >= 0 && sx <= Dst.Width)
				Dst.Data[y*Dst.Width + sx].integer = color;
		}

		return 1;
	}

	void DrawWalls(World &map, PixelBuffer &dst);
	float FindHorizontalIntersection(World &map, float castAngle);
	float FindVerticalIntersection(World &map, float castAngle);
	bool Trace(World &map, Ray &ray);
};

#endif