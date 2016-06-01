
#include "Player.h"
//#include "Main.h"
#include <math.h>
#include "World.h"


void Player::Update(AppManager &mainApp)
{
	if (mainApp.KeyHit(Key::E))
		eyeFish = !eyeFish;

	if (mainApp.KeyPressed(Key::W) || mainApp.KeyPressed(Key::Up))
	{
		posX += cos(Ang) * MOVE_SPEED;
		posY += sin(Ang) * MOVE_SPEED;
	}
	if (mainApp.KeyPressed(Key::S) || mainApp.KeyPressed(Key::Down))
	{
		posX -= cos(Ang) * MOVE_SPEED;
		posY -= sin(Ang) * MOVE_SPEED;
	}

	if (mainApp.KeyPressed(Key::D))
	{
		posX -= sin(Ang) * MOVE_SPEED;
		posY += cos(Ang) * MOVE_SPEED;
	}
	if (mainApp.KeyPressed(Key::A))
	{
		posX += sin(Ang) * MOVE_SPEED;
		posY -= cos(Ang) * MOVE_SPEED;
	}

	if (mainApp.KeyPressed(Key::Left))
	{
		Ang -= TURN_SPEED;
		WrapAngle(Ang);
	}
	if (mainApp.KeyPressed(Key::Right))
	{
		Ang += TURN_SPEED;
		WrapAngle(Ang);
	}
}

void Player::DrawLine(int x1, int y1, int x2, int y2, PixelBuffer &Dst, const DWORD& color, int brush)
{
	x1 = Clamp(x1, 0, Dst.Width - (brush));
	y1 = Clamp(y1, 0, Dst.Height - (brush));
	x2 = Clamp(x2, 0, Dst.Width - (brush));
	y2 = Clamp(y2, 0, Dst.Height - (brush));

	//if (x1 < 0 || x1 > Dst.Width - (1 + brush) || x2 < 0 || x2 > Dst.Width - (1 + brush) ||
	//	y1 < 0 || y1 > Dst.Height - (1 + brush) || y2 < 0 || y2 > Dst.Height - (1 + brush)) return 0;


	int deltax = std::abs(x2 - x1);									//The difference between the x's
	int deltay = std::abs(y2 - y1);									//The difference between the y's
	int x = x1;														//Start x off at the first pixel
	int y = y1;														//Start y off at the first pixel
	int xinc1, xinc2, yinc1, yinc2, den, num, numadd, numpixels, curpixel;

	if (x2 >= x1)													//The x-values are increasing
	{
		xinc1 = 1;
		xinc2 = 1;
	}
	else															//The x-values are decreasing
	{
		xinc1 = -1;
		xinc2 = -1;
	}
	if (y2 >= y1)													//The y-values are increasing
	{
		yinc1 = 1;
		yinc2 = 1;
	}
	else															//The y-values are decreasing
	{
		yinc1 = -1;
		yinc2 = -1;
	}
	if (deltax >= deltay)						//There is at least one x-value for every y-value
	{
		xinc1 = 0;								//Don't change the x when numerator >= denominator
		yinc2 = 0;								//Don't change the y for every iteration
		den = deltax;
		num = deltax >> 2;
		numadd = deltay;
		numpixels = deltax;						//There are more x-values than y-values
	}
	else										//There is at least one y-value for every x-value
	{
		xinc2 = 0;								//Don't change the x for every iteration
		yinc1 = 0;								//Don't change the y when numerator >= denominator
		den = deltay;
		num = deltay >> 2;
		numadd = deltax;
		numpixels = deltay;						//There are more y-values than x-values
	}
	for (curpixel = 0; curpixel <= numpixels; curpixel++)
	{
		//pset(x % Dst.Width, y % Dst.Height, color);				//Draw the current pixel
		//Dst.Data[x + (y * Dst.Width)] = Pixel(1, 1, 1);
		Dst.Data[x + (y * Dst.Width)].integer += 0x00FF0000;

		for (int i = 0; i < brush; i++)
		{
			Dst.Data[(x + i) + (y * Dst.Width)].integer += color;		// Brush Broad Trough x (Kinda)
			Dst.Data[x + ((y + i) * Dst.Width)].integer += color;		// Brush Broad Trough y
		}


		num += numadd;							//Increase the numerator by the top of the fraction
		if (num >= den)							//Check if numerator >= denominator
		{
			num -= den;							//Calculate the new numerator value
			x += xinc1;							//Change the x as appropriate
			y += yinc1;							//Change the y as appropriate
		}
		x += xinc2;								//Change the x as appropriate
		y += yinc2;								//Change the y as appropriate
	}
}

void Player::DrawWalls(World &map, PixelBuffer &dst)
{
	int column = 0;												// Screen pixels columns index
	float degreesPerColumn = FOV / dst.Width;					// How many degradees had every column
	float start = Ang - (FOV*0.5f);								// The very first column index from the left

	while (column < dst.Width)									// Traversing trough horizontal resolution
	{
		float castAngle = start + degreesPerColumn * column;	// current angle ray casted
		float alpha = castAngle - Ang;							// angle difference for Eyefish FIX
		WrapAngle(castAngle);									// clamp around 0 and Pi * 2 (-360 ~ +360)

		float hDist = -1, vDist = -1;						// first htere was nothing... and then something

		hDist = FindHorizontalIntersection(map, castAngle);		// check in Horizontal & Vertical axis
		vDist = FindVerticalIntersection(map, castAngle);

		float distToWall = (vDist < hDist ? vDist : hDist);
		DWORD color = (vDist < hDist ? COL_GREEN : COL_RED);

		if (!eyeFish)
			distToWall *= cos(alpha);								// EyeFish Disabler	

		int wallSize = (BLOCK_SIZE / distToWall) * dst.Height;	// Getting real wall height
		int y = (dst.Height>>1) - (wallSize>>1);

		// DrawRect(column, y, column + 1, y + wallSize, dst, color);
		VerLine(column, y, y + wallSize, dst, color);
		column++;
	}


}

float Player::FindHorizontalIntersection(World &map, float castAngle) // RED COLOR
{
	struct Ray hRay;

	if (castAngle < PI)
	{
		hRay.y = floor(posY / BLOCK_SIZE) * BLOCK_SIZE + BLOCK_SIZE;
		hRay.x = posX + (fabs(posY - hRay.y) / tan(castAngle));
		hRay.yStep = BLOCK_SIZE;
		hRay.xStep = BLOCK_SIZE / tan(castAngle);
	}
	else
	{
		hRay.y = floor(posY / BLOCK_SIZE) * BLOCK_SIZE ;
		hRay.x = posX + (fabs(posY - hRay.y) / tan(-castAngle)) ;
		hRay.yStep = -BLOCK_SIZE;
		hRay.xStep = BLOCK_SIZE / tan(-castAngle);
		hRay.y--;
	}

	if (Trace(map, hRay) != false) 
		return Distance(posX, posY, hRay.x, hRay.y);
	else 
		return 10000000000;
	
}
float Player::FindVerticalIntersection(World &map, float castAngle) // GREEN COLOR
{
	struct Ray vRay;

	if (castAngle >= PI_TWO && castAngle < 3 * PI_TWO) 
	{
		vRay.x = floor(posX / BLOCK_SIZE) * BLOCK_SIZE;
		vRay.y = posY + (fabs(vRay.x - posX) * tan(-castAngle));
		vRay.xStep = -BLOCK_SIZE;
		vRay.yStep = BLOCK_SIZE * tan(-castAngle);
		vRay.x--;
	}
	else
	{
		vRay.x = floor(posX / BLOCK_SIZE) * BLOCK_SIZE + BLOCK_SIZE;
		vRay.y = posY + (fabs(vRay.x - posX) * tan(castAngle));
		vRay.xStep = BLOCK_SIZE;
		vRay.yStep = BLOCK_SIZE * tan(castAngle);
	}

	if (Trace(map, vRay) != false)
		return Distance(posX, posY, vRay.x, vRay.y);
	else
		return 10000000000;
}

bool Player::Trace(World &map, Ray &ray)
{
	while (ray.x <= BLOCK_SIZE * MAP_WIDTH && ray.x >= 0
		&& ray.y <= BLOCK_SIZE * MAP_HEIGHT && ray.y >= 0)
	{

		int gX = ray.x / BLOCK_SIZE;
		int gY = ray.y / BLOCK_SIZE;
					
		if (map.GetBlock(gX, gY) > 0)        //if (map[gY][gX] > 0)
			return true;

		ray.x += ray.xStep;
		ray.y += ray.yStep;
	}

	return false;
}

//#include	<iostream>
/*
void Player::DrawInMap(PixelBuffer & Dst)
{
	//Pixel ZoneColor(0, .75f, .75f);
	DWORD ZoneColor = 0x008888FF;
	DWORD DirColor = 0x00007777;
	float Pzone = .2f;		// A little offset 2 draw a small area around Player
	float FrontLength = 2;		// A little offset 2 draw a small area around Player
	float FrontX = (Player.posX + (cos(Player.Rot) * FrontLength)) * Map.Scale;
	float FrontY = (Player.posY + (sin(Player.Rot) * FrontLength)) * Map.Scale;

	//std::cout << " RADIANS: " << Player.Rot << " - DEGREES: " << DEG(Player.Rot) << "\n";

	float LeftViewX = (Player.posX + (cos(Player.Rot + RAD(-30))) * FrontLength * 2) * Map.Scale;
	float LeftViewY = (Player.posY + (sin(Player.Rot + RAD(-30))) * FrontLength * 2) * Map.Scale;
	float RightViewX = (Player.posX + (cos(Player.Rot + RAD(+30))) * FrontLength * 2) * Map.Scale;
	float RightViewY = (Player.posY + (sin(Player.Rot + RAD(+30))) * FrontLength * 2) * Map.Scale;
	gfx::drawLine(ZoneColor, Player.posX * Map.Scale, Player.posY * Map.Scale, LeftViewX, LeftViewY, Dst, 1);
	gfx::drawLine(ZoneColor, Player.posX * Map.Scale, Player.posY * Map.Scale, RightViewX, RightViewY, Dst, 1);


	gfx::drawRect(ZoneColor, (Player.posX - Pzone) * Map.Scale, (Player.posY - Pzone) * Map.Scale, (Player.posX + Pzone) * Map.Scale, (Player.posY + Pzone) * Map.Scale, Dst);
	gfx::drawLine(DirColor, Player.posX * Map.Scale, Player.posY * Map.Scale, FrontX, FrontY, Dst, 2);

}

void Player::UpdateInput(AppManager & App, float timeStep)
{

	Move = 0;
	Dir = 0;

	if (App.KeyPressed(Key::W) || App.KeyPressed(Key::Up))
		Move = 1;

	if (App.KeyPressed(Key::S) || App.KeyPressed(Key::Down))
		Move = -1;

	float moveStep = MoveSpeed * Move; // Player will move this far along the current direction vector

	if (App.KeyPressed(Key::D) || App.KeyPressed(Key::Right))
		Dir = 1;

	if (App.KeyPressed(Key::A) || App.KeyPressed(Key::Left))
		Dir = -1;

	// Add rotation if player is rotating (player.dir != 0)
	Rot += Dir * RotSpeed;

	// Calculate new player position with simple trigonometry
	float dX = cos(Rot) * moveStep;
	float dY = sin(Rot) * moveStep;

	// Set new position smoothly (slide along the only axis we are abled to move)
	if (!Map.isBlocked(posX + dX, posY)) // smoother 
		posX += dX;

	if (!Map.isBlocked(posX, posY + dY))
		posY += dY;

	// Calculate new player position with simple trigonometry	// It Hungs the move
	//float newX = posX + cos(Rot) * moveStep;
	//float newY = posY + sin(Rot) * moveStep;
	//if ( Map.isBlocked(newX, newY) )
	//	return;
	//posX = newX;
	//posY = newY;

	//speed modifiers
	MoveSpeed = timeStep * 5.0f; //the constant value is in squares/second
	RotSpeed = timeStep * 3.0f; //the constant value is in radians/second
}

void Player::Render(World &map, PixelBuffer &buffer)
{
	// Draw Columns
	for (int column = 0, resolution = (SCREEN_W >> 1); column < resolution; column++)
	{
		float xCoord = column / resolution - 0.5f;
		float angle = atan2(xCoord, 0.8f);
		//Ray ray = map.cast(player, player.direction + angle, this.range);
		//this.drawColumn(column, ray, angle, map);
	}
}

void Player::DrawColumn(World &map, PixelBuffer &buffer, Ray &ray, float angle, int column)
{
}

*/