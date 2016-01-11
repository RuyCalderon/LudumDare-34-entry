#if !defined(LD_34_GAMESTRUCTS_H)
#define LD_34_GAMESTRUCTS_H

struct Surface_2D
{
	int Height;
};

struct Circle2D
{
	v2D_f CenterPoint;
	uint32_t Radius;
};

struct RenderingColors
{
	v4D_f SkyColor;
	v4D_f SunColor;
	v4D_f MoonColor;
	v4D_f GroundColor;
	v4D_f GroundWaterColor;
};

enum LIMITING_RESOURCE
{
	RESOURCE_WATER,
	RESOURCE_LIGHT,
	NUM_RESOURCES
};

#define MIN_WEATHER_FRAME_LENGTH 100
#define MAX_WEATHER_FRAME_LENGTH 1000
enum WEATHER
{
	WEATHER_CLEAR,
	WEATHER_STORM,
	WEATHER_DROUGHT,
	NUM_WEATHER_TYPES
};

struct WeatherEvent
{
	int FrameDuration;
	int WeatherType;
};

struct GameState
{
	int DaysPassed;
	int TimeOfDay;

	int TotalCycleTime;
	int MidDayTime;
	int MidNightTime;
	
	b32 DayTime;
	v2D_f SunLocation;
	r32 PercentDaylight;

	int GroundHeight;
	int GroundWaterHeight; 

	WeatherEvent CurrentWeather;
};

struct PlayerInputState
{
	b32 Up;
	b32 Down;
	b32 Planting;
	uint32_t PlantingCounter;
};

struct RootNode
{
	v2D_f Start;
	v2D_f End; // this is present for reference by offshoots
	
	v2D_f GrowthDirection;
	r32 Length;
	r32 Width;

	b32 InGroundwater;
	int NumOffshoots;
	r32 NewOffshootThreshold;
};

struct Leaf
{
	v2D_f Dimensions;
	v2D_f RelativePosition;
	b32 MaxSize;
};

struct PlantNode
{
	v2D_f *ParentEnd;
	v2D_f *ParentStart;
	r32 DistancePercent;

	v2D_f Start;
	v2D_f End; // this is present for reference by offshoots
	
	v2D_f GrowthDirection;
	r32 Length;
	r32 Width;

	r32 LinearDistanceInLight;
	int NumOffshoots;
	r32 NewOffshootThreshold;

	Leaf LeafContainer[20];
	int NumLeaves;
};

struct Plant
{
	b32 Planted;
	v2D_f Location;

	int LimitingResource;
	r32 PlantResources;
	r32 SeedResourcesRemaining;
	r32 MaxGrowthRate;

	int NumPlantNodes;
	int NumRootNodes;
	PlantNode PlantNodeContainer[100];
	RootNode RootNodeContainer[100];

	r32 CumulativeRootLengthInGroundWater;
	r32 CumulativeLeafSkyAccess;

	PlantNode *PlantStart;
	RootNode *RootStart;
};

#endif
