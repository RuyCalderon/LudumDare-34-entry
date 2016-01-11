#if !defined(LD_34_HELPERFUNCTIONS_H)
#define LD_34_HELPERFUNCTIONS_H

static int
SafeDivide(int X, int Y)
{
	if(Y == 0)
	{
		return 0;
	}
	else
	{
		return (X/Y);
	}
}

static float
SafeDivide(float X, float Y)
{
	if(Y == 0.0f)
	{
		return 0.0f;
	}
	else
	{
		return (X/Y);
	}
}

int Linterp(int KnownAxisLength, int DistOnKnownAxis, int UnknownAxisLength, int UnknownAxisStart)
{
	r32 DistPercent = (r32) DistOnKnownAxis / (r32)KnownAxisLength;
	int DistOnUnknownAxis = (int) ((r32) UnknownAxisLength * DistPercent) + UnknownAxisStart;
	return DistOnUnknownAxis;
}

r32 Linterp(r32 KnownAxisLength, r32 DistOnKnownAxis, r32 UnknownAxisLength, r32 UnknownAxisStart)
{
	r32 DistPercent = DistOnKnownAxis / KnownAxisLength;
	r32 DistOnUnknownAxis = ( UnknownAxisLength * DistPercent) + UnknownAxisStart;
	return DistOnUnknownAxis;
}

int ExclusiveClamp(int Min, int Max, int Value)
{
	#define CLAMP_FAILURE_VALUE 0xFFFAE13F //Obscure enough?
	Assert(Value != CLAMP_FAILURE_VALUE);
	Assert(Min != CLAMP_FAILURE_VALUE);
	Assert(Max != CLAMP_FAILURE_VALUE);

	if(Value >= Max)
	{
		return CLAMP_FAILURE_VALUE;
	}
	if(Value <= Min)
	{
		return CLAMP_FAILURE_VALUE;
	}
	return Value;
}

uint32_t BuildColor(float R, float G, float B, float A)
{
	uint32_t RedChannel = (uint32_t)(R * A * 255.0f);
	uint32_t GreenChannel = (uint32_t)(G * 255.0f);
	uint32_t BlueChannel = (uint32_t)(B * A * 255.0f);
	uint32_t AlphaChannel = (uint32_t)(A * 255.0f);
	uint32_t Result = (AlphaChannel << 24) | (RedChannel << 16) | (GreenChannel << 8) | BlueChannel;

	return Result;
}

uint32_t BuildColor(v4D_f Color)
{
	uint32_t RedChannel = (uint32_t)(Color.R * Color.A * 255.0f);
	uint32_t GreenChannel = (uint32_t)(Color.G * Color.A * 255.0f);
	uint32_t BlueChannel = (uint32_t)(Color.B * Color.A * 255.0f);
	uint32_t AlphaChannel = (uint32_t)(Color.A * 255.0f);
	uint32_t Result = (AlphaChannel << 24) | (RedChannel << 16) | (GreenChannel << 8) | BlueChannel;

	return Result;
}

double
GetFrameTime(int FramesPerSecond)
{
	double TimePerFrame = 1.0/(double)FramesPerSecond;
	return TimePerFrame;
}

#endif
