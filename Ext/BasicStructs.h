#if !defined(BASICSTRUCTS_H)
#define BASICSTRUCTS_H

#include<cstdarg>

struct Box2D
{
	v2D_f CenterPoint;
	v2D_f Dimensions;
};

Box2D Box(v2D_f CenterPoint, v2D_f Dimensions)
{
	Box2D NewBox = {};
	NewBox.CenterPoint = CenterPoint;
	NewBox.Dimensions = Dimensions;
	return NewBox;
}

Box2D Box(r32 X0, r32 Y0, r32 X1, r32 Y1)
{
	Box2D NewBox = {};
	NewBox.CenterPoint = v2D(X1 + X0,Y1 + Y0) / 2.0f;
	NewBox.Dimensions = v2D(X1 - X0, Y1 - Y0);
	return NewBox;
}

bool
PointInsideBox(Box2D Box, v2D_f Point)
{
	r32 BoxMinX = Box.CenterPoint.X - Box.Dimensions.X/2.0f;
	r32 BoxMaxX = BoxMinX + Box.Dimensions.X;
	r32 BoxMinY = Box.CenterPoint.Y - Box.Dimensions.Y/2.0f;
	r32 BoxMaxY = BoxMinY + Box.Dimensions.Y;

	if(Point.X > BoxMinX && 
	   Point.X < BoxMaxX &&
	   Point.Y > BoxMaxY &&
	   Point.Y < BoxMaxY)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool
BoxesCollide(Box2D A, Box2D B)
{
	if((abs(A.CenterPoint.X - B.CenterPoint.X)*2 <= (A.Dimensions.X + B.Dimensions.X)) &&
	   (abs(A.CenterPoint.Y - B.CenterPoint.Y)*2 <= (A.Dimensions.Y + B.Dimensions.Y)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

v2D_f 
GetMin(Box2D A)
{
	v2D_f Result = v2D(0.0f,0.0f);

	Result = A.CenterPoint - (A.Dimensions / 2.0f);
	return Result;
}

v2D_f 
GetMax(Box2D A)
{
	v2D_f Result = v2D(0.0f,0.0f);

	Result = A.CenterPoint + (A.Dimensions / 2.0f);
	return Result;	
}

#endif
