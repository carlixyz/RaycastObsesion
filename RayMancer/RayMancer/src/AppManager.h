#ifndef APP_MAN_H
#define APP_MAN_H

#include "PixelToaster\PixelToaster.h"
using namespace PixelToaster;

#define SCREEN_W 640
#define SCREEN_H 480
#define SIZE SCREEN_W*SCREEN_H

#define HALF_SCR_W SCREEN_W/2
#define HALF_SCR_H SCREEN_H/2

#define DWORD unsigned int
//#include "Singleton.h"


struct PixelBuffer // PixelBuffer
{
	std::vector<TrueColorPixel> Data;
	int Width;
	int Height;
	int Area;

	PixelBuffer(){;}
	inline PixelBuffer(int width, int height)
	{
		Width = width;
		Height = height;
		Area = Width * Height;
		Data.resize(Area);
	}
	inline void Resize(int width, int height)
	{
		Width = width;
		Height = height;
		Area = Width * Height;
		Data.resize(Area);
	}
	inline void SetPixel(int x, int y, DWORD color)
	{
		if (x<0 || y<0 || x >= Width || y >= Height)
			return;
		Data[y * Width + x].integer = color;
	}
	inline void Cls(DWORD color = 0x00000000)	// Clear Screen to black
	{
		//memset(&v[0], 0, v.size() * sizeof v[0]);
		//for (int i = 0; i < Area; i++)
		//	Data[i].integer = color;
		std::fill(Data.begin(), Data.end(), (TrueColorPixel)color);
		//for (std::vector<int>::iterator it = v.begin(), end = v.end(); it != end; ++it) 
		//	*it = 0;

	}
	inline bool onArea(int x, int y)
	{
		return (x >= 0 && y >= 0 && x < Width && y < Height);
	}
	DWORD &PixelBuffer::operator[](int index)
	{
		return Data[index].integer;
	}
	PixelBuffer &PixelBuffer::operator=(const PixelBuffer& b)
	{
		Width = b.Width;
		Height = b.Height;
		Area = Width * Height;
		Data = b.Data;
		return *this;
	}
	
	inline int Clamp(int n, int lower, int upper)
	{
		return n + ((n < lower) * (lower - n)) + ((n > upper) * (upper - n));
	}
	inline void Draw(PixelBuffer &src, int xCoord = 0, int yCoord = 0)	// Draw Another buffer Data Inside 
	{
		//xCoord = Clamp(xCoord, 0, this->Width - src.Width);
		//yCoord = Clamp(yCoord, 0, this->Height - src.Height);

		//for (int x = 0, pixX = 0; x < src.Width; x++)
		//for (int y = 0, pixY = 0; y < src.Height; y++)
		//{
		//	pixY = Clamp((y + yCoord) * this->Width, 0, this->Area - (yCoord * this->Width + xCoord));
		//	pixX = Clamp((x + xCoord), 0, this->Width -10);
		//	Data[pixY + pixX].integer |= src[src.Width * y + x]; //Add
		//}

		for (int x = 0, pixX = 0; x < src.Width; x++)
		for (int y = 0, pixY = 0; y < src.Height; y++)
		{
			pixX = (x + xCoord);
			pixY = (y + yCoord) * this->Width;
			if (pixY + pixX >= this->Area)
				continue;

			//if ((y + yCoord) * this->Width + (x + xCoord) < this->Area)
			//	Data[(y + yCoord) * this->Width + (x + xCoord)].integer |= src[src.Width * y + x]; //Add
			Data[pixY + pixX].integer |= src[src.Width * y + x]; //Add
		}
	}
	inline void ReScale(float factor)											// Scaling Property
	{
		PixelBuffer refBuffer(Width, Height);									// TempCopy
		refBuffer.Data = this->Data;

		this->Resize((int)(Width * factor), (int)(Height * factor));				// Real Buffer Resize

		float scaleWidth = (float)this->Width / (float)refBuffer.Width;			// Scale proportion
		float scaleHeight = (float)this->Height / (float)refBuffer.Height;

		for (int cy = 0; cy < this->Height; cy++)
		{
			for (int cx = 0; cx < this->Width; cx++)
			{
				int pixel = (cy * (this->Width)) + (cx);
				int nearestMatch = (((int)(cy / scaleHeight) * (refBuffer.Width)) + ((int)(cx / scaleWidth)));
				Data[pixel] = refBuffer.Data[nearestMatch];
			}
		}
	}
};



class AppManager : public Listener //, public cSingleton<AppManager>
{
public:
	//friend class cSingleton<AppManager>;

	bool KeyPressed(Key key);

	bool KeyHit(Key key);

	void ForgetKeys();

	DWORD *OpenWindow(char* title, int width, int height);

	bool WindowClosed();

	void CloseWindow();

	void UpdateWindow();

	void UpdateWindow(PixelBuffer  & pBuffer);

	void UpdateWindowX2(PixelBuffer &pBuffer);

	Timer time;
	Display* window;
	PixelBuffer canvas;

	//EventHandler* ptwrap_events;

	AppManager()
	{
		// Protected constructor
		for (int i = 0; i < 256; i++)
		{
			keyPressed[i] = false;
			keyHit[i] = false;
		}
	}

protected:
	//AppManager()
	//{
	//	// Protected constructor
	//	for (int i = 0; i < 256; i++)
	//	{
	//		keyPressed[i] = false;
	//		keyHit[i] = false;
	//	}
	//}
	void onKeyDown(DisplayInterface& display, Key key)
	{
		int ikey = (int)key;
		// set state of pressed key
		keyPressed[ikey] = true;
		// mark as new key hit
		keyHit[ikey] = true;
		// note id of key for quick removal from keyhit list
		for (int i = 0; i<16; i++)
		if (newKeys[i] == -1)
		{
			newKeys[i] = ikey;
			break;
		}
	}
	void onKeyUp(DisplayInterface& display, Key key)
	{
		// clear state of released key
		keyPressed[(int)key] = false;
	}


private:
	bool keyPressed[256];
	bool keyHit[256];
	int newKeys[8];
};


#endif