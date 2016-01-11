#if !defined(LD_34_GAMEFUNCTIONS_H)
#define LD_34_GAMEFUNCTIONS_H

void 
UpdateTimings(GameState *State)
{
	if(State->TimeOfDay > State->TotalCycleTime)
	{
		State->TimeOfDay = 0;
		State->DaysPassed++;
	}
	else
	{
		State->TimeOfDay++;
	}

	if(State->TimeOfDay < 90 && State->TimeOfDay > 270)
	{
		State->DayTime = false;
	}
	else
	{
		State->DayTime = true;
	}

	State->PercentDaylight = (float)(State->MidDayTime - abs(State->MidDayTime - State->TimeOfDay)) / (float)State->MidDayTime;
}

void
InitializeLeaf(RandomState *Random, Leaf *NewLeaf, PlantNode *Node, v2D_f Dimensions)
{
	NewLeaf->Dimensions = Dimensions;
	r32 NodeRangeX = abs(Node->End.X - Node->Start.X);
	r32 NodeRangeY = Node->End.Y - Node->Start.Y;
	NewLeaf->RelativePosition = v2D(Random->GetRandomFloat(-(NodeRangeX / 1.5f), (NodeRangeX / 1.5f)), Random->GetRandomFloat(0.0f, (NodeRangeY * 1.5f)));
	NewLeaf->MaxSize = false;
}

void
InitializeRootNode(RootNode *Node, v2D_f Start, r32 Length, v2D_f GrowthDirection, r32 Width, int NumOffshoots, r32 NewOffshootThreshold)
{
	Node->Start = Start;
	Node->Length = Length;
	Node->GrowthDirection = GrowthDirection;
	Node->Width = Width;
	Node->NumOffshoots = NumOffshoots;
	Node->NewOffshootThreshold = NewOffshootThreshold;

	Node->End = Start + (GrowthDirection * Length);
}

void 
InitializePlantNode(PlantNode *Node, v2D_f *ParentEnd, v2D_f *ParentStart, r32 SproutDistancePercent, r32 Length,  v2D_f GrowthDirection, r32 Width, int NumOffshoots, r32 NewOffshootThreshold)
{
	Node->DistancePercent = SproutDistancePercent; 
	Node->ParentStart = ParentStart;
	Node->ParentEnd = ParentEnd;

	Node->Length = Length;
	Node->GrowthDirection = GrowthDirection;
	Node->Width = Width;
	Node->NumOffshoots = NumOffshoots;
	Node->NewOffshootThreshold = NewOffshootThreshold;

	Node->Start = (*ParentEnd - *ParentStart) * SproutDistancePercent;
	Node->End = Node->Start + (GrowthDirection * Length);
}


void 
CheckRootSystemGroundwaterAccess(Plant *PlayerPlant, GameState *State)
{
	PlayerPlant->CumulativeRootLengthInGroundWater = 0.0f;
	r32 GroundHeightToCheckFor = (float)State->GroundWaterHeight; // checking "above", windows bitmap rendering starts at top right corner of screen
	r32 CumulativeLinearRootLengthInGroundwater = 0.0f;
	for(int i = 0;
		i < PlayerPlant->NumRootNodes;
		++i)
	{
		RootNode *Root = &PlayerPlant->RootNodeContainer[i];
		if(Root->End.Y > GroundHeightToCheckFor && Root->Start.Y < GroundHeightToCheckFor)
		{
			Root->InGroundwater = true; //check to allow branching
			r32 LengthUnderwater = Linterp(Root->End.Y - Root->Start.Y, GroundHeightToCheckFor - Root->Start.Y, Root->Length, 0.0f);
			CumulativeLinearRootLengthInGroundwater += LengthUnderwater;
		}
		else if(Root->End.Y > GroundHeightToCheckFor && Root->Start.Y > GroundHeightToCheckFor)
		{
			Root->InGroundwater = true;
			CumulativeLinearRootLengthInGroundwater += Root->Length;
		}
		Assert(CumulativeLinearRootLengthInGroundwater >= 0.0f)
	}

	PlayerPlant->CumulativeRootLengthInGroundWater += CumulativeLinearRootLengthInGroundwater;
}


void 
CheckPlantSunAccess(Plant *PlayerPlant, GameState *State)
{
	PlayerPlant->CumulativeLeafSkyAccess = 0.0f;
	//Assume that all limbs lie on different planes in the "z" axis (which doesn't exist)
	r32 CumulativeLinearSunlightLength = 0.0f;
	if(State->DayTime)
	{
		v2D_f SunDirectionVector = State->SunLocation - PlayerPlant->Location; // to simulate the sun being arbitrarily far away use this vector for all occlusion calculations

		for(int i = PlayerPlant->NumPlantNodes - 1;
			i >= 0;
			--i)
		{
			PlantNode *Node = &PlayerPlant->PlantNodeContainer[i];
			v2D_f LimbVector = Node->End - Node->Start;

			//is there a better way to do this? My linear algebra is rusty
			v2D_f PerpendicularCompenent = LimbVector - (VectorNormalize(SunDirectionVector) * VectorDOT(SunDirectionVector, LimbVector));
			Node->LinearDistanceInLight = VectorLength(PerpendicularCompenent);
			CumulativeLinearSunlightLength += Node->LinearDistanceInLight;
		}
	}
	PlayerPlant->CumulativeLeafSkyAccess = CumulativeLinearSunlightLength;
}

void 
AccumulateResources(Plant *PlayerPlant, GameState *State)
{
	if(State->DayTime)
	{
		PlayerPlant->LimitingResource = ((PlayerPlant->CumulativeLeafSkyAccess * State->PercentDaylight) >= PlayerPlant->CumulativeRootLengthInGroundWater) ? RESOURCE_WATER : RESOURCE_LIGHT;
		if(PlayerPlant->LimitingResource == RESOURCE_WATER)
		{
			PlayerPlant->PlantResources += PlayerPlant->CumulativeRootLengthInGroundWater;
		}
		else if(PlayerPlant->LimitingResource == RESOURCE_LIGHT)
		{
			PlayerPlant->PlantResources += PlayerPlant->CumulativeLeafSkyAccess * State->PercentDaylight;
		}
	}
}


#define MAX_WIDTH 10.0f
#define STORAGE_THRESHOLD 1000.0f
#define GROWTH_RATE_ADJUSTMENT 10.0f
void 
GrowDownwards(RandomState *Random, Plant *PlayerPlant ,GameState *State)
{
	b32 TakeFromSeed = false;
	if(PlayerPlant->SeedResourcesRemaining > 0.0f)
	{
		TakeFromSeed = true;
	}
	else
	{
		PlayerPlant->SeedResourcesRemaining = 0.0f;
	}
	if(PlayerPlant->NumRootNodes == 0)
	{
		PlayerPlant->RootStart = &PlayerPlant->RootNodeContainer[0];
		InitializeRootNode(PlayerPlant->RootStart, PlayerPlant->Location, 1.0f, VectorNormalize(v2D(Random->GetRandomFloat(-0.5f,0.5f), Random->GetRandomFloat(0.0f,1.0f))), 1.0f, 0, Random->GetRandomFloat(10.0f,50.0f));
		PlayerPlant->NumRootNodes++;
	}
	else
	{
		int NumNodes = PlayerPlant->NumRootNodes;
		for(int i = 0;
			i < NumNodes;
			++i)
		{
			RootNode *Node = &PlayerPlant->RootNodeContainer[i];
			//Make growth dependent upon resources _and_ sunlight
			if(PlayerPlant->PlantResources >= 5.0f)
			{
				Node->Length += 0.1f * GROWTH_RATE_ADJUSTMENT;
				PlayerPlant->PlantResources -= 5.0f;
				if(TakeFromSeed)
				{
					PlayerPlant->SeedResourcesRemaining -= 5.0f;
				}
			}

			//storage threshold
			if(PlayerPlant->PlantResources >= 1000.0f && PlayerPlant->PlantResources > 5.0f *Node->Length)
			{
				Node->Width += 0.01f * ((MAX_WIDTH - Node->Width) / MAX_WIDTH) * GROWTH_RATE_ADJUSTMENT * State->PercentDaylight;
				PlayerPlant->PlantResources -= 5.0f * Node->Length;
				if(TakeFromSeed)
				{
					PlayerPlant->SeedResourcesRemaining -= 5.0f * Node->Length;
				}
			}

			Node->End = Node->Start + (Node->GrowthDirection * Node->Length);
			if(Node->Length > Node->NewOffshootThreshold * (float)(Node->NumOffshoots + 1) && Node->NumOffshoots < 4 && Node->InGroundwater&& PlayerPlant->NumRootNodes < 100)
			{
				Node->NumOffshoots++;
				RootNode *NewNode = &PlayerPlant->RootNodeContainer[PlayerPlant->NumRootNodes++];
				InitializeRootNode(NewNode, Node->End, 0.0f, VectorNormalize(v2D(Random->GetRandomFloat(-1.0f,1.0f), Random->GetRandomFloat(0.0f,1.0f))), 0.0f, 0, Random->GetRandomFloat(50.0f,100.0f));
			}
		}
	}
	if(PlayerPlant->PlantResources <= 0.0f || isnan(PlayerPlant->PlantResources))
	{
		PlayerPlant->PlantResources = 0.0f;
	}
}

//grow upwards works like the old version
void 
GrowUpwards(RandomState *Random, Plant *PlayerPlant, GameState *State)
{
	b32 TakeFromSeed = false;
	if(PlayerPlant->SeedResourcesRemaining > 0.0f)
	{
		TakeFromSeed = true;
	}
	else
	{
		PlayerPlant->SeedResourcesRemaining = 0.0f;
	}

	if(PlayerPlant->NumPlantNodes == 0)
	{
		PlayerPlant->PlantStart = &PlayerPlant->PlantNodeContainer[0];
		InitializePlantNode(PlayerPlant->PlantStart, &PlayerPlant->Location, &PlayerPlant->Location, 1.0f, 1.0f, VectorNormalize(v2D(Random->GetRandomFloat(-1.0f,1.0f), Random->GetRandomFloat(-2.0f,0.0f))), 1.0f, 0, Random->GetRandomFloat(35.0f,70.0f));
		PlayerPlant->NumPlantNodes++;
	}
	else
	{
		int NumNodes = PlayerPlant->NumPlantNodes;
		for(int i = 0;
			i < NumNodes;
			++i)
		{
			PlantNode *Node = &PlayerPlant->PlantNodeContainer[i];

			if(PlayerPlant->PlantResources >= 10.0f)
			{
				Node->Length += 0.1f * GROWTH_RATE_ADJUSTMENT * State->PercentDaylight;
				PlayerPlant->PlantResources -= 10.0f;
				if(TakeFromSeed)
				{
					PlayerPlant->SeedResourcesRemaining -= 10.0f;
				}

				for(int j = 0;
					j < Node->NumLeaves;
					++j)
				{
					Leaf *LeafToAlter = &Node->LeafContainer[j];
					LeafToAlter->RelativePosition += VectorNormalize(LeafToAlter->RelativePosition) * 0.1f;
				}
			}
			
			if(PlayerPlant->PlantResources >= 10.0f * Node->Length && PlayerPlant->PlantResources >= 1000.0f)
			{
				Node->Width += 0.01f * ((MAX_WIDTH - Node->Width) / MAX_WIDTH) * GROWTH_RATE_ADJUSTMENT * State->PercentDaylight;
				PlayerPlant->PlantResources -= 10.0f * Node->Length;
				if(TakeFromSeed)
				{
					PlayerPlant->SeedResourcesRemaining -= 10.0f *Node->Length;
				}
			}

			if(i == 0 && Node->NumLeaves == 0)
			{
				//first leaves are free
				Leaf *NewLeaf = &Node->LeafContainer[Node->NumLeaves++];
				InitializeLeaf(Random, NewLeaf, Node, v2D(1.0f,1.0f));
			}
			if(PlayerPlant->PlantResources >= 1500.0f && Node->LinearDistanceInLight > 35.0f && Node->NumLeaves < 20)
			{
				Leaf *NewLeaf = &Node->LeafContainer[Node->NumLeaves++];
				InitializeLeaf(Random, NewLeaf, Node, v2D(1.0f,1.0f));

				PlayerPlant->PlantResources -= 500.0f;
				if(TakeFromSeed)
				{
					//should be impossible
					PlayerPlant->SeedResourcesRemaining -= 500.0f;
				}
			}

			Node->Start = (*Node->ParentEnd - *Node->ParentStart) * (1.0f - Node->DistancePercent) + *Node->ParentStart;
			Node->End = Node->Start + (Node->GrowthDirection * Node->Length);

			if(Node->Length > Node->NewOffshootThreshold * (float)(Node->NumOffshoots + 1) && (i == 0 || Node->NumOffshoots < 2) && PlayerPlant->NumPlantNodes < 100)
			{
				Node->NumOffshoots++;
				PlantNode *NewNode = &PlayerPlant->PlantNodeContainer[PlayerPlant->NumPlantNodes++];
				InitializePlantNode(NewNode, &Node->Start, &Node->End, Random->GetRandomFloat(0.0f, 0.5f), 0.0f, VectorNormalize(v2D(Random->GetRandomFloat(-1.0f,1.0f), Random->GetRandomFloat(-0.5f,0.0f))), 0.0f, 0, Random->GetRandomFloat(50.0f,100.0f));
			}
		}
	}
	if(PlayerPlant->PlantResources <= 0.0f  || isnan(PlayerPlant->PlantResources))
	{
		PlayerPlant->PlantResources = 0.0f;
	}

}

void GrowLeaves(RandomState *Random, Plant * PlayerPlant)
{
	for(int i = 0;
		i < PlayerPlant->NumPlantNodes;
		++i)
	{
		PlantNode *Node = &PlayerPlant->PlantNodeContainer[i];
		for(int j = 0;
		j < Node->NumLeaves;
		++j)
		{
			Leaf *LeafToAlter = &Node->LeafContainer[j];
			if(!LeafToAlter->MaxSize)
			{
				LeafToAlter->Dimensions += VectorNormalize(v2D(Random->GetRandomFloat(0.0f, 1.0f), Random->GetRandomFloat(0.0f,1.0f))) * 0.1f;
				if(LeafToAlter->Dimensions.X * LeafToAlter->Dimensions.Y >= 500.0f)
				{
					LeafToAlter->MaxSize = true;
				}
			}
		}
	}
}

void InitializeWeatherEvent(RandomState *Random, WeatherEvent *Weather)
{
	int WeatherType = Random->NextNum()%NUM_WEATHER_TYPES;
	Weather->WeatherType = WeatherType;
	Weather->FrameDuration = (Random->NextNum()%(MAX_WEATHER_FRAME_LENGTH - MIN_WEATHER_FRAME_LENGTH)) + MIN_WEATHER_FRAME_LENGTH;
}

void CheckWeather(RandomState *Random, GameState *State)
{
	State->CurrentWeather.FrameDuration -= 1;
	if(State->CurrentWeather.FrameDuration <= 0)
	{
		InitializeWeatherEvent(Random, &State->CurrentWeather);
	}
}

#endif
