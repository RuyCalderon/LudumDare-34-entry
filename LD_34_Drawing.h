#if !defined(LD_34_DRAWING_H)
#define LD_34_DRAWING_H

void DrawCircle(uint32_t Color, Circle2D Circle, WinScreenBuffer *Buffer)
{
	v2D_i BufferStart = v2D(0,0);
	v2D_i BufferEnd = v2D(Buffer->Dimensions.X,Buffer->Dimensions.Y);

	uint32_t * BufferAccess = (uint32_t *)Buffer->BufferMemory;
	//inverted so long axis matches long axis
	v2D_i DrawStart = v2D((int)(Circle.CenterPoint.X - Circle.Radius),
						  (int)(Circle.CenterPoint.Y - Circle.Radius));
	VectorClamp(&DrawStart, BufferStart, BufferEnd);

	v2D_i DrawEnd = v2D((int)(Circle.CenterPoint.X + Circle.Radius),
						(int)(Circle.CenterPoint.Y + Circle.Radius));
	VectorClamp(&DrawEnd, BufferStart, BufferEnd);

	int RadiusSquared = Circle.Radius * Circle.Radius;
	for(int Y = DrawStart.Y;
		Y < DrawEnd.Y;
		++Y)
	{
		int RelY = (int)Circle.CenterPoint.Y - Y;
		for(int X = DrawStart.X;
			X < DrawEnd.X;
			++X)
		{
			int RelX = (int)Circle.CenterPoint.X - X;
			if((RelX*RelX + RelY*RelY) <= RadiusSquared)
			{
				BufferAccess[Y * BufferEnd.X + X] = Color;
			}
		}
	}
}


void
DrawLine_LinearFallof(v2D_f Start, v2D_f End, int width, uint32_t Color, WinScreenBuffer *Buffer)
{
	v2D_i BufferStart = v2D(0,0);
	v2D_i BufferEnd = v2D(Buffer->Dimensions.X,Buffer->Dimensions.Y);

	uint32_t * BufferAccess = (uint32_t *)Buffer->BufferMemory;

	v2D_f Distance = End - Start;
	v2D_f GrowthDirection = VectorNormalize(Distance);
	int TotalDistance = (int)VectorToScalar(End - Start);
	for(int T = 0;
		T < TotalDistance;
		++T)
	{
		int XCoordinate = (int)Start.X + (int)(GrowthDirection.X * (float)T);
		int YCoordinate = (int)Start.Y + (int)(GrowthDirection.Y * (float)T);
		int EffectiveWidth = (int)((float)width * (float)(TotalDistance - T) / (float)TotalDistance);

		for(int Y = YCoordinate - (EffectiveWidth / 2);
			Y < (YCoordinate + 1) + (EffectiveWidth / 2);
			++Y)
		{
			for(int X = XCoordinate - (EffectiveWidth / 2);
				X < (XCoordinate + 1) + (EffectiveWidth / 2);
				++X)
			{
				int DrawX = ExclusiveClamp(BufferStart.X, BufferEnd.X - 1, X);
				int DrawY = ExclusiveClamp(BufferStart.Y, BufferEnd.Y - 1, Y);

				if(DrawX != CLAMP_FAILURE_VALUE && DrawY != CLAMP_FAILURE_VALUE)
				{
					BufferAccess[DrawY * Buffer->Dimensions.X + DrawX] = Color;
				}
			}
		}
	}
}

void
DrawLine_SquareFallof(v2D_f Start, v2D_f End, int width, uint32_t Color, WinScreenBuffer *Buffer)
{
	v2D_i BufferStart = v2D(0,0);
	v2D_i BufferEnd = v2D(Buffer->Dimensions.X,Buffer->Dimensions.Y);

	uint32_t * BufferAccess = (uint32_t *)Buffer->BufferMemory;

	v2D_f Distance = End - Start;
	v2D_f GrowthDirection = VectorNormalize(Distance);
	int TotalDistance = (int)VectorToScalar(End - Start);
	
	for(int T = 0;
		T < TotalDistance;
		++T)
	{
		int XCoordinate = (int)Start.X + (int)(GrowthDirection.X * (float)T);
		int YCoordinate = (int)Start.Y + (int)(GrowthDirection.Y * (float)T);
		int EffectiveWidth = (int)((float)width * sqrt(((float)(TotalDistance - T) / (float)TotalDistance)));
		
		b32 FirstPlaced = false;
		for(int Y = YCoordinate - (EffectiveWidth / 2);
			Y < (YCoordinate + 1) + (EffectiveWidth / 2);
			++Y)
		{
			for(int X = XCoordinate - (EffectiveWidth / 2);
				X < (XCoordinate + 1) + (EffectiveWidth / 2);
				++X)
			{
				int DrawX = ExclusiveClamp(BufferStart.X, BufferEnd.X - 1, X);
				int DrawY = ExclusiveClamp(BufferStart.Y, BufferEnd.Y - 1, Y);
				if(DrawX != CLAMP_FAILURE_VALUE && DrawY != CLAMP_FAILURE_VALUE)
				{
					if(!FirstPlaced)
					{
						BufferAccess[DrawY * Buffer->Dimensions.X + DrawX] = (Color | 0xAFAFAF00); // brighten a smidge. Just on the tip. Just to see how it feels.
					}
					else
					{
						BufferAccess[DrawY * Buffer->Dimensions.X + DrawX] = Color;
					}
				}
			}
			FirstPlaced =true;
		}
	}
}

void DrawBox(uint32_t Color, Box2D Box, WinScreenBuffer *Buffer)
{
	v2D_i BufferStart = v2D(0,0);
	v2D_i BufferEnd = v2D(Buffer->Dimensions.X,Buffer->Dimensions.Y);

	uint32_t * BufferAccess = (uint32_t *)Buffer->BufferMemory;
	//inverted so long axis matches long axis
	v2D_i DrawStart = v2D((int)(Box.CenterPoint.X - Box.Dimensions.X / 2),
						  (int)(Box.CenterPoint.Y - Box.Dimensions.Y / 2));
	VectorClamp(&DrawStart, BufferStart, BufferEnd);

	v2D_i DrawEnd = v2D((int)(Box.CenterPoint.X + Box.Dimensions.X / 2),
						(int)(Box.CenterPoint.Y + Box.Dimensions.Y / 2));
	VectorClamp(&DrawEnd, BufferStart, BufferEnd);

	for(int Y = DrawStart.Y;
		Y < DrawEnd.Y;
		++Y)
	{
		for(int X = DrawStart.X;
			X < DrawEnd.X;
			++X)
		{
			BufferAccess[Y * BufferEnd.X + X] = Color;
		}
	}
}

#define PI 3.1415f
void DrawDayNightCycle(WinScreenBuffer *ScreenBuffer, RECT WindowDimensions, GameState *GameState, RenderingColors *Colors)
{
	uint32_t SunColor = BuildColor(Colors->SunColor);
	uint32_t MoonColor = BuildColor(Colors->MoonColor);

	v2D_f CycleCenter = v2D((float)WindowDimensions.right / 2.0f, (float)WindowDimensions.bottom / 2.0f);
	r32 Radius = CycleCenter.Y * 0.75f; 
	r32 HorizontalStretch = CycleCenter.X / CycleCenter.Y;
	//TODO(Ruy): matrices would be nice...
	r32 SunRadians = (float)(GameState->TimeOfDay - 270) / 360.0f * 2.0f * PI;
	r32 MoonRadians = (float)(GameState->TimeOfDay - 90) / 360.0f * 2.0f * PI;
	
	v2D_f SunLocation =  v2D((float)(Radius * cos(SunRadians) * HorizontalStretch), (float)(Radius * sin(SunRadians))) + CycleCenter;
	int SunRadius = 15;
	v2D_f MoonLocation =  v2D((float)(Radius * cos(MoonRadians) * HorizontalStretch * 0.9f), (float)(Radius * sin(MoonRadians) * 0.9f)) + CycleCenter;
	int MoonRadius = 3;

	Circle2D Sun = {};
	Sun.Radius = SunRadius;
	Sun.CenterPoint = SunLocation;

	Circle2D Moon = {};
	Moon.Radius = MoonRadius;
	Moon.CenterPoint = MoonLocation;
	DrawCircle(SunColor, Sun, ScreenBuffer);
	DrawCircle(MoonColor, Moon, ScreenBuffer);
}

void DrawGround(WinScreenBuffer *Buffer, GameState *State, RenderingColors *Colors)
{
	uint32_t GroundColor = BuildColor(VectorHadamard(Colors->GroundColor, Colors->SunColor * State->PercentDaylight));
	uint32_t GroundWaterColor = BuildColor(Colors->GroundWaterColor);

	Box2D GroundBox = Box(0.0f, (float)State->GroundHeight, (float)Buffer->Dimensions.X, (float)Buffer->Dimensions.Y);
	Box2D GroundWaterBox = Box(0.0f, (float)State->GroundWaterHeight, (float)Buffer->Dimensions.X, (float)Buffer->Dimensions.Y);

	DrawBox(GroundColor, GroundBox, Buffer);
	DrawBox(GroundWaterColor, GroundWaterBox, Buffer);
}

//TODO(Ruy): Fucking green twilight
void DrawBackground(RECT WindowDimensions, GameState *State, RenderingColors *Colors)
{
	Box2D Background = Box((float)WindowDimensions.left, (float)WindowDimensions.top, (float)WindowDimensions.right, (float)WindowDimensions.bottom);
	
	uint32_t Color = BuildColor(VectorHadamard(Colors->SkyColor, Colors->SunColor * State->PercentDaylight));
	DrawBox(Color, Background, &ScreenBuffer);
}

void DrawRootTree(RootNode *NodeContainer, int NumNodes, uint32_t Color, WinScreenBuffer *ScreenBuffer)
{
	for(int i = 0;
		i < NumNodes;
		++i)
	{
		RootNode *CurrentNode = &NodeContainer[i];
		v2D_f StartLocation = CurrentNode->Start;
		v2D_f EndLocation = CurrentNode->End;
		DrawLine_LinearFallof(StartLocation, EndLocation, (int)CurrentNode->Width, Color, ScreenBuffer);
	}
}

void DrawLeaf(Leaf *CurrentLeaf, v2D_f LeafPosition, uint32_t Color, WinScreenBuffer *ScreenBuffer)
{
	Box2D Leaf = Box(LeafPosition, CurrentLeaf->Dimensions);
	DrawBox(Color, Leaf, ScreenBuffer);
	Leaf.CenterPoint = LeafPosition - v2D(2.0f,2.0f);
	DrawBox(Color | 0x6FFF6F00, Leaf, ScreenBuffer);
}

void DrawPlantTree(PlantNode *NodeContainer, int NumNodes, uint32_t Color, WinScreenBuffer *ScreenBuffer)
{
	uint32_t LeafColor = BuildColor(0.34f, 0.64f, 0.33f, 1.0f);
	for(int i = 0;
		i < NumNodes;
		++i)
	{
		PlantNode *CurrentNode = &NodeContainer[i];
		v2D_f StartLocation = CurrentNode->Start;
		v2D_f EndLocation = CurrentNode->End;
		DrawLine_SquareFallof(StartLocation, EndLocation, (int)CurrentNode->Width, Color, ScreenBuffer);
		
		v2D_f NodeCenter = (EndLocation + StartLocation ) / 2.0f;
		//TODO(Ruy): COntinue implementing leaves
		
		for(int j = 0;
			j < CurrentNode->NumLeaves;
			++j)
		{
			Leaf *CurrentLeaf = &CurrentNode->LeafContainer[j];
			v2D_f LeafPosition = NodeCenter + CurrentLeaf->RelativePosition;
			DrawLeaf(CurrentLeaf, LeafPosition, LeafColor, ScreenBuffer);
		}
	}
}

void DrawPlayerPlant(Plant *PlayerPlant, WinScreenBuffer *ScreenBuffer)
{
	int SeedRadius = (int)sqrt(PlayerPlant->SeedResourcesRemaining / 100.0f);
	Circle2D Seed = {};
	Seed.Radius = SeedRadius;
	Seed.CenterPoint = PlayerPlant->Location;
	DrawCircle(BuildColor(0.8f,0.9f,0.75f, 1.0f), Seed, ScreenBuffer);

	if(PlayerPlant->Planted)
	{
		DrawPlantTree(PlayerPlant->PlantNodeContainer, PlayerPlant->NumPlantNodes, BuildColor(0.6f, 0.4f, 0.3f, 1.0f), ScreenBuffer);
		DrawRootTree(PlayerPlant->RootNodeContainer, PlayerPlant->NumRootNodes, BuildColor(0.4f, 0.6f, 0.7f, 1.0f), ScreenBuffer);
	}
}

#endif
