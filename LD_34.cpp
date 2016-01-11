#include<windows.h>
#include <windowsx.h>
#include<iostream>

#include "stdint.h"
#define Assert(X) if(!(X)){*(int *)0 = 0;}
#define b32 uint32_t
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t
#define s8 int8_t
#define s16 int16_t
#define s32 int32_t
#define s64 int64_t
#define r32 float
#define r64 double

#define KILOBYTES (1024LL)
#define MEGABYTES (KILOBYTES * 1024LL)
#define GIGABYTES (MEGABYTES * 1024LL)
#define TERABYTES (GIGABYTES * 1024LL)
#include "Ext/RNGsuss.h"
#include "Ext/BasicVector.h"
#include "Ext/BasicStructs.h"
#include "math.h"

struct WinScreenBuffer
{
	v2D_i Dimensions;

	BITMAPINFO BufferInfo;
	void * BufferMemory;
};

static WinScreenBuffer ScreenBuffer;

static void
InitializeScreenBuffer(RECT BufferDimensions)
{
	if(ScreenBuffer.BufferMemory)
	{
		VirtualFree(ScreenBuffer.BufferMemory, 0, MEM_RELEASE);
	}

	ScreenBuffer.BufferInfo.bmiHeader.biSize = sizeof(ScreenBuffer.BufferInfo.bmiHeader);
	ScreenBuffer.BufferInfo.bmiHeader.biWidth = BufferDimensions.right - BufferDimensions.left;
	ScreenBuffer.BufferInfo.bmiHeader.biHeight = -BufferDimensions.bottom - BufferDimensions.top;
	ScreenBuffer.BufferInfo.bmiHeader.biPlanes = 1;
	ScreenBuffer.BufferInfo.bmiHeader.biBitCount = 32;
	ScreenBuffer.BufferInfo.bmiHeader.biCompression = BI_RGB;
	ScreenBuffer.BufferInfo.bmiHeader.biSizeImage = 0;
	ScreenBuffer.BufferInfo.bmiHeader.biXPelsPerMeter = 0;
	ScreenBuffer.BufferInfo.bmiHeader.biYPelsPerMeter = 0;
	ScreenBuffer.BufferInfo.bmiHeader.biClrUsed = 0;
	ScreenBuffer.BufferInfo.bmiHeader.biClrImportant = 0;

	ScreenBuffer.Dimensions.Width = ScreenBuffer.BufferInfo.bmiHeader.biWidth;
	ScreenBuffer.Dimensions.Height = -ScreenBuffer.BufferInfo.bmiHeader.biHeight;

	size_t SizeofBuffer = (ScreenBuffer.Dimensions.Width * ScreenBuffer.Dimensions.Height) * (ScreenBuffer.BufferInfo.bmiHeader.biBitCount / 8);
	ScreenBuffer.BufferMemory = VirtualAlloc(NULL, SizeofBuffer, MEM_COMMIT, PAGE_READWRITE);
}

static void
DrawScreenBuffer(HDC deviceContext, RECT WindowDimensions)
{
	StretchDIBits(deviceContext,
				  WindowDimensions.left, // dest
				  WindowDimensions.top,
				  WindowDimensions.right - WindowDimensions.left,
				  WindowDimensions.bottom - WindowDimensions.top,
				  WindowDimensions.left, // src
				  WindowDimensions.top,
				  WindowDimensions.right - WindowDimensions.left,
				  WindowDimensions.bottom - WindowDimensions.top,
				  ScreenBuffer.BufferMemory,
				  &ScreenBuffer.BufferInfo,
				  DIB_RGB_COLORS,
				  SRCCOPY);
}

#include "LD_34_HelperFunctions.h"
#include "LD_34_GameStructs.h"
#include "LD_34_GameFunctions.h"
#include "LD_34_Drawing.h"



static int
GetStringLength(char * TestStringOut)
{
	int ArrayCount = 0;
	while(TestStringOut[++ArrayCount] != '\0')
	{continue;}
	return ArrayCount;
}



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = 0;
    switch (uMsg)
    {
    	case WM_CREATE:
    	{
    		RECT BufferDimensions = {};
    		GetClientRect(hwnd,&BufferDimensions);
    		InitializeScreenBuffer(BufferDimensions);
    		break;
    	}
    	case WM_SIZE: 
    	{
    		RECT BufferDimensions = {};
    		GetClientRect(hwnd,&BufferDimensions);
    		InitializeScreenBuffer(BufferDimensions);
        	break;   
    	}
    	case WM_PAINT:
    	{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);


			RECT WindowDimensions = {};
			GetClientRect(hwnd,&WindowDimensions);
			DrawScreenBuffer(hdc, WindowDimensions);

			EndPaint(hwnd, &ps);
			break;
    	}
    	case WM_CLOSE:
    	{
    		PostQuitMessage(0);
    		break;
    	}
	    default:
	    {
	    	Result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	    }
	}
	return Result;
}


	

INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, INT nCmdShow)
{
	if(lpCmdLine && nCmdShow && hPrevInstance)
	{
		Assert(nCmdShow);	
	}

	const wchar_t CLASS_NAME[]  = L"MainWindowClass";

	double TimePerFrame = GetFrameTime(30);
	char OutputDebugBuffer[1024] = {};

	WNDCLASS WindowClass = {};

	WindowClass.style = CS_HREDRAW|CS_VREDRAW;
    WindowClass.lpfnWndProc   = WindowProc;
    WindowClass.hInstance     = hInstance;
    WindowClass.lpszClassName = (LPCSTR)CLASS_NAME;

    RegisterClass(&WindowClass);

    HWND hwnd = CreateWindowEx(0,                              // Optional window styles.
    						   (LPCSTR)CLASS_NAME,                     // Window class
    						   NULL,      // Window text
    						   WS_OVERLAPPEDWINDOW,            // Window style

    						   // Size and position
    						   CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

    						   NULL,       // Parent window    
    						   NULL,       // Menu
    						   hInstance,  // Instance handle
    						   NULL        // Additional application data
    						   );
    if(hwnd == NULL)
    {
        return 0;
    }
    else
    {
    	ShowWindow(hwnd,nCmdShow);
    }

	bool Running = true;





	HANDLE DebugConsoleHandle = {};
	DebugConsoleHandle = GetStdHandle( STD_OUTPUT_HANDLE );

	RECT WindowDimensions = {};
	GetClientRect(hwnd, &WindowDimensions);

	RandomState RandomGen = {};
	RandomGen.Reseed(13241251426LL);
	
	LARGE_INTEGER BeginFrameCount;
	LARGE_INTEGER EndFrameCount;
	uint64_t FrameCountDiff = 0;
	
	PlayerInputState InputState = {};
	GameState GamePersistentState = {};
	GamePersistentState.TimeOfDay = 0;
	GamePersistentState.TotalCycleTime = 360;
	GamePersistentState.MidDayTime = 180;
	GamePersistentState.MidNightTime = 0;

	GamePersistentState.GroundHeight = (WindowDimensions.bottom - WindowDimensions.top) / 2;
	GamePersistentState.GroundWaterHeight = (WindowDimensions.bottom - WindowDimensions.top) * 3 / 4;

	RenderingColors GameEntityColors = {};

	InitializeWeatherEvent(&RandomGen, &GamePersistentState.CurrentWeather);
	
	
	Plant PlayerPlant = {};
	PlayerPlant.Location = v2D((float)(WindowDimensions.right - WindowDimensions.left) / 2.0f, (float)GamePersistentState.GroundHeight);
	PlayerPlant.PlantResources = 1000.0f;
	PlayerPlant.SeedResourcesRemaining = 1000.0f;

	while(Running)
	{
		QueryPerformanceCounter(&BeginFrameCount);
		
		MSG Message = {};
		while(PeekMessage(&Message, hwnd, 0,0,PM_REMOVE))
		{
        	switch(Message.message)
        	{	
				//keyboard input
            	case WM_KEYDOWN:
            	{
					switch(Message.wParam)
					{
						case VK_ESCAPE:
						{
							
							break;
						}
						default:
						{
							break;
						}
					}
					
					break;
            	}
            	case WM_KEYUP:
            	{
            		switch(Message.wParam)
					{
						case VK_DOWN:
						{
							if(InputState.Down)
							{
								InputState.Up = 0;
								InputState.Down = 0;
							}
							else
							{
								InputState.Up = 0;
								InputState.Down = 1;
							}
							break;
						}
						case VK_UP:
						{
							if(InputState.Up)
							{
								InputState.Down = 0;
								InputState.Up = 0;
							}
							else
							{
								InputState.Down = 0;
								InputState.Up = 1;
							}
							break;
						}
						case VK_ESCAPE:
						{
							Running = false;
							break;
						}
						default:
						{
							break;
						}	
					}
					
					break;
            	}
            	default:
            	{
            		TranslateMessage(&Message);
        			DispatchMessage(&Message);
            		break;
            	}
        	}
		}

		//Update in case a resizing has occurred
		GetClientRect(hwnd,&WindowDimensions);

		memset((void *)ScreenBuffer.BufferMemory, 0, (WindowDimensions.right - WindowDimensions.left) * (WindowDimensions.bottom - WindowDimensions.top)); //zero buffer
		//uint32_t * BufferPixelAccess = (uint32_t *)ScreenBuffer.BufferMemory;





		//Game Code

		UpdateTimings(&GamePersistentState);
		CheckWeather(&RandomGen, &GamePersistentState);

		v4D_f SunColor = v4D(0.9f, 0.86f, 0.91f, 1.0f);
		v4D_f MoonColor = v4D(0.9f, 0.88f, 0.78f, 1.0f);
		v4D_f SkyColorProfile = v4D(0.74f, 0.64f,0.75f, 1.0f);
		v4D_f GroundColor = v4D(0.74f, 0.64f,0.75f, 1.0f);
		v4D_f GroundWaterColor = v4D(0.74f, 0.64f,0.75f, 1.0f);
		switch(GamePersistentState.CurrentWeather.WeatherType)
		{
			case WEATHER_CLEAR:
			{
				SkyColorProfile = v4D(0.74f, 0.85f,0.86f, 1.0f);
				GroundColor = v4D(0.61f, 0.419f,0.14f, 1.0f);
				GroundWaterColor = v4D(0.45f, 0.294f,0.078f, 1.0f);
				break;
			}
	 		case WEATHER_STORM:
	 		{
	 			SkyColorProfile.R = 0.23f;
	 			SkyColorProfile.G = 0.36f;
	 			SkyColorProfile.B = 0.44f;
	 			SkyColorProfile.A = 1.0f;

	 			GroundColor.R = 0.12f;
	 			GroundColor.G = 0.23f;
	 			GroundColor.B = 0.32f;
	 			GroundColor.A = 1.0f;

	 			GroundWaterColor.R = 0.1f;
	 			GroundWaterColor.G = 0.18f;
	 			GroundWaterColor.B = 0.24f;
	 			GroundWaterColor.A = 1.0f;

	 			if(RandomGen.NextNum()%10 == 1)
	 			{
		 			if(GamePersistentState.GroundWaterHeight > GamePersistentState.GroundHeight)
		 			{
		 				GamePersistentState.GroundWaterHeight -= 1;
		 			}
		 			else
		 			{
		 				GamePersistentState.GroundWaterHeight = GamePersistentState.GroundHeight + 1;
		 			}
	 			}
	 			GamePersistentState.PercentDaylight = GamePersistentState.PercentDaylight * GamePersistentState.PercentDaylight;

	 			break;
	 		}
	 		case WEATHER_DROUGHT:
	 		{
	 			SkyColorProfile.R = 0.90f;
	 			SkyColorProfile.G = 0.84f;
	 			SkyColorProfile.B = 0.98f;
	 			SkyColorProfile.A = 1.0f;

	 			GroundColor.R = 0.94f;
	 			GroundColor.G = 0.79f;
	 			GroundColor.B = 0.26f;
	 			GroundColor.A = 1.0f;

	 			GroundWaterColor.R = 0.70f;
	 			GroundWaterColor.G = 0.568f;
	 			GroundWaterColor.B = 0.104f;
	 			GroundWaterColor.A = 1.0f;

	 			if(RandomGen.NextNum()%10 == 1)
	 			{
		 			if(GamePersistentState.GroundWaterHeight < (WindowDimensions.bottom - WindowDimensions.top))
		 			{
		 				GamePersistentState.GroundWaterHeight += 1;
		 			}
		 			else
		 			{
		 				GamePersistentState.GroundWaterHeight = (WindowDimensions.bottom - WindowDimensions.top) - 1;
		 			}
		 		}

		 		GamePersistentState.PercentDaylight = sqrt(GamePersistentState.PercentDaylight);
	 			break;
	 		}
	 		default:
	 			break;		
	 	}

	 	GameEntityColors.SkyColor = SkyColorProfile;
	 	GameEntityColors.SunColor = SunColor;
	 	GameEntityColors.MoonColor = MoonColor;
	 	GameEntityColors.GroundColor = GroundColor;
	 	GameEntityColors.GroundWaterColor = GroundWaterColor;

		CheckRootSystemGroundwaterAccess(&PlayerPlant, &GamePersistentState);
		CheckPlantSunAccess(&PlayerPlant, &GamePersistentState);
		if(PlayerPlant.Planted && PlayerPlant.NumPlantNodes > 0)
		{
			GrowLeaves(&RandomGen, &PlayerPlant);
		}
		if(!InputState.Planting)
		{
			if(PlayerPlant.Planted)
			{
				AccumulateResources(&PlayerPlant, &GamePersistentState);
			}
			if(InputState.Down)
			{
				if(!PlayerPlant.Planted)
				{
					InputState.PlantingCounter = 15;
					InputState.Planting = true;
				}
				else
				{
					GrowDownwards(&RandomGen, &PlayerPlant, &GamePersistentState);
				}
			}
			if(InputState.Up)
			{
				if(PlayerPlant.Planted)
				{
					GrowUpwards(&RandomGen, &PlayerPlant, &GamePersistentState);
				}
			}

		}
		else
		{
			if(--InputState.PlantingCounter <= 0)
			{
				InputState.Planting = false;
				PlayerPlant.Planted = true;
			}
		}


		//Drawing Code
		DrawBackground(WindowDimensions, &GamePersistentState, &GameEntityColors);
		DrawDayNightCycle(&ScreenBuffer, WindowDimensions, &GamePersistentState, &GameEntityColors);
		DrawGround(&ScreenBuffer,&GamePersistentState, &GameEntityColors);
		//DrawWeather(&ScreenBuffer, &GamePersistentState);
		DrawPlayerPlant(&PlayerPlant, &ScreenBuffer);




		HDC WindowDeviceContext = GetDC(hwnd);
		DrawScreenBuffer(WindowDeviceContext, WindowDimensions);

		LARGE_INTEGER CurrentProcessorSpeedInCounts;
		QueryPerformanceFrequency(&CurrentProcessorSpeedInCounts);
		uint64_t EstimatedCountsPerFrame = (uint64_t)((double)(CurrentProcessorSpeedInCounts.QuadPart) * TimePerFrame);

		QueryPerformanceCounter(&EndFrameCount);
		FrameCountDiff = EndFrameCount.QuadPart - BeginFrameCount.QuadPart;
		while(FrameCountDiff < EstimatedCountsPerFrame)
		{
			//spinlocking
			QueryPerformanceCounter(&EndFrameCount);
			FrameCountDiff = EndFrameCount.QuadPart - BeginFrameCount.QuadPart;
		}
		float FrameTime = ((float)FrameCountDiff / (float)CurrentProcessorSpeedInCounts.QuadPart)*1000.0f;
		sprintf_s((LPSTR)OutputDebugBuffer,sizeof(OutputDebugBuffer), "Frame Time: %f ms \nDesired Frame Time: %f ms", FrameTime, (float)TimePerFrame*1000.0f);
		OutputDebugStringA(OutputDebugBuffer);
	}
	return 0;
}

