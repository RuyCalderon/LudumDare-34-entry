#if !defined(BASICVECTOR_H)
#define BASICVECTOR_H

//Math vectors so a templated type is unnecessary as I will only need ints and r32s
//Maybe revisit templates if I find the need for signed/unsigned values of many variable sizes

//TODO(Ruy): Double check all of the math in the vector class
union v2D_i
{
	struct
	{
		int X;
		int Y;
	};
	struct
	{
		int Width;
		int Height;
	};
	struct
	{
		int A;
		int B;
	};
	int El[2];
};

union v2D_f
{
	struct
	{
		r32 X;
		r32 Y;
	};
	struct
	{
		r32 Width;
		r32 Height;
	};
	struct
	{
		r32 A;
		r32 B;
	};
	r32 El[2];
};

union v3D_i
{
	struct
	{
		int X;
		int Y;
		int Z;
	};
	struct
	{
		int A;
		int B;
		int C;
	};
	int El[3];
};

union v3D_f
{
	struct
	{
		r32 X;
		r32 Y;
		r32 Z;
	};
	struct
	{
		r32 A;
		r32 B;
		r32 C;
	};
	r32 El[3];
};

union v4D_i
{
	struct
	{
		int X;
		int Y;
		int Z;
		int W;
	};
	struct
	{
		int R;
		int G;
		int B;
		int A;
	};
	struct
	{
		v3D_i Vector;
		int Null_0;
	}v3;
	struct
	{
		v2D_i Vector;
		int Null_0;
		int Null_1;
	}v2;
	int El[4];
};

union v4D_f
{
	struct
	{
		r32 X;
		r32 Y;
		r32 Z;
		r32 W;
	};
	struct
	{
		r32 R;
		r32 G;
		r32 B;
		r32 A;
	};
	struct
	{
		v3D_f Vector;
		r32 Null_0;
	}v3;
	struct
	{
		v2D_f Vector;
		r32 Null_0;
		r32 Null_1;
	}v2;
	r32 El[4];
};

//This implementation begs the question of initializing an integer vector with r32s and
//vice-versa. I don't want to create functionality to do so because I want that conversion
//to be explicit every time it is used becuase of the risk of losing information.
inline v2D_i
v2D(int X, int Y)
{
	v2D_i New = {};

	New.X = X;
	New.Y = Y;

	return New;
}

inline v2D_f
v2D(r32 X, r32 Y)
{
	v2D_f New = {};

	New.X = X;
	New.Y = Y;

	return New;
}

inline v3D_i
v3D(int X, int Y, int Z)
{
	v3D_i New = {};

	New.X = X;
	New.Y = Y;
	New.Z = Z;

	return New;
}

inline v3D_f
v3D(r32 X, r32 Y, r32 Z)
{
	v3D_f New = {};

	New.X = X;
	New.Y = Y;
	New.Z = Z;

	return New;
}

inline v4D_i
v4D(int X, int Y, int Z, int W)
{
	v4D_i New = {};

	New.X = X;
	New.Y = Y;
	New.Z = Z;
	New.W = W;

	return New;
}

inline v4D_f
v4D(r32 X, r32 Y, r32 Z, r32 W)
{
	v4D_f New = {};

	New.X = X;
	New.Y = Y;
	New.Z = Z;
	New.W = W;

	return New;
}

inline v2D_f
v2D_IntToFloat(v2D_i A)
{
	v2D_f New = {};

	New.X = (r32)A.X; 
	New.Y = (r32)A.Y;

	return New;
}

inline v3D_f
v3D_IntToFloat(v3D_i A)
{
	v3D_f New = {};

	New.X = (r32)A.X; 
	New.Y = (r32)A.Y;
	New.Z = (r32)A.Z;

	return New;
}

inline v2D_i
v2D_RoundFloatToInt(v2D_f A)
{
	v2D_i New = {};

	New.X = (A.X > 0) ? (int)(A.X + 0.5f) : (int)(A.X - 0.5f); 
	New.Y = (A.Y > 0) ? (int)(A.Y + 0.5f) : (int)(A.Y - 0.5f);

	return New;
}

inline v3D_i
v3D_RoundFloatToInt(v3D_f A)
{
	v3D_i New = {};

	New.X = (A.X > 0) ? (int)(A.X + 0.5f) : (int)(A.X - 0.5f); 
	New.Y = (A.Y > 0) ? (int)(A.Y + 0.5f) : (int)(A.Y - 0.5f);
	New.Z = (A.Z > 0) ? (int)(A.Z + 0.5f) : (int)(A.Z - 0.5f);
	
	return New;
}


inline v2D_i& 
operator+=(v2D_i& A, v2D_i B)
{
	A.X = A.X + B.X;
	A.Y = A.Y + B.Y;
	return A;
}

inline v2D_f& 
operator+=(v2D_f& A, v2D_f B)
{
	A.X = A.X + B.X;
	A.Y = A.Y + B.Y;
	return A;
}

inline v3D_i& 
operator+=(v3D_i& A, v3D_i B)
{
	A.X = A.X + B.X;
	A.Y = A.Y + B.Y;
	A.Z = A.Z + B.Z;
	return A;
}

inline v3D_f& 
operator+=(v3D_f& A, v3D_f B)
{
	A.X = A.X + B.X;
	A.Y = A.Y + B.Y;
	A.Z = A.Z + B.Z;
	return A;
}

inline v4D_i& 
operator+=(v4D_i& A, v4D_i B)
{
	A.X = A.X + B.X;
	A.Y = A.Y + B.Y;
	A.Z = A.Z + B.Z;
	A.W = A.W + B.W;
	return A;
}

inline v4D_f& 
operator+=(v4D_f& A, v4D_f B)
{
	A.X = A.X + B.X;
	A.Y = A.Y + B.Y;
	A.Z = A.Z + B.Z;
	A.W = A.W + B.W;
	return A;
}


inline v2D_i 
operator+(v2D_i A, v2D_i B)
{
	v2D_i C = {};
	C.X = A.X + B.X;
	C.Y = A.Y + B.Y;
	return C;
}

inline v2D_f 
operator+(v2D_f A, v2D_f B)
{
	v2D_f C = {};
	C.X = A.X + B.X;
	C.Y = A.Y + B.Y;
	return C;
}

inline v3D_i 
operator+(v3D_i A, v3D_i B)
{
	v3D_i C = {};
	C.X = A.X + B.X;
	C.Y = A.Y + B.Y;
	C.Z = A.Z + B.Z;
	return C;
}

inline v3D_f 
operator+(v3D_f A, v3D_f B)
{
	v3D_f C = {};
	C.X = A.X + B.X;
	C.Y = A.Y + B.Y;
	C.Z = A.Z + B.Z;
	return C;
}

inline v4D_i 
operator+(v4D_i A, v4D_i B)
{
	v4D_i C = {};
	C.X = A.X + B.X;
	C.Y = A.Y + B.Y;
	C.Z = A.Z + B.Z;
	C.W = A.W + B.W;
	return C;
}

inline v4D_f 
operator+(v4D_f A, v4D_f B)
{
	v4D_f C = {};
	C.X = A.X + B.X;
	C.Y = A.Y + B.Y;
	C.Z = A.Z + B.Z;
	C.W = A.W + B.W;
	return C;
}


inline v2D_i& 
operator-=(v2D_i& A, v2D_i B)
{
	A.X = A.X - B.X;
	A.Y = A.Y - B.Y;
	return A;
}

inline v2D_f& 
operator-=(v2D_f& A, v2D_f B)
{
	A.X = A.X - B.X;
	A.Y = A.Y - B.Y;
	return A;
}

inline v3D_i& 
operator-=(v3D_i& A, v3D_i B)
{
	A.X = A.X - B.X;
	A.Y = A.Y - B.Y;
	A.Z = A.Z - B.Z;
	return A;
}

inline v3D_f& 
operator-=(v3D_f& A, v3D_f B)
{
	A.X = A.X - B.X;
	A.Y = A.Y - B.Y;
	A.Z = A.Z - B.Z;
	return A;
}

inline v4D_i& 
operator-=(v4D_i& A, v4D_i B)
{
	A.X = A.X - B.X;
	A.Y = A.Y - B.Y;
	A.Z = A.Z - B.Z;
	A.W = A.W - B.W;
	return A;
}

inline v4D_f& 
operator-=(v4D_f& A, v4D_f B)
{
	A.X = A.X - B.X;
	A.Y = A.Y - B.Y;
	A.Z = A.Z - B.Z;
	A.W = A.W - B.W;
	return A;
}


inline v2D_i 
operator-(v2D_i A, v2D_i B)
{
	v2D_i C = {};
	C.X = A.X - B.X;
	C.Y = A.Y - B.Y;
	return C;
}

inline v2D_f 
operator-(v2D_f A, v2D_f B)
{
	v2D_f C = {};
	C.X = A.X - B.X;
	C.Y = A.Y - B.Y;
	return C;
}

inline v3D_i 
operator-(v3D_i A, v3D_i B)
{
	v3D_i C = {};
	C.X = A.X - B.X;
	C.Y = A.Y - B.Y;
	C.Z = A.Z - B.Z;
	return C;
}

inline v3D_f 
operator-(v3D_f A, v3D_f B)
{
	v3D_f C = {};
	C.X = A.X - B.X;
	C.Y = A.Y - B.Y;
	C.Z = A.Z - B.Z;
	return C;
}

inline v4D_i 
operator-(v4D_i A, v4D_i B)
{
	v4D_i C = {};
	C.X = A.X - B.X;
	C.Y = A.Y - B.Y;
	C.Z = A.Z - B.Z;
	C.W = A.W - B.W;
	return C;
}

inline v4D_f 
operator-(v4D_f A, v4D_f B)
{
	v4D_f C = {};
	C.X = A.X - B.X;
	C.Y = A.Y - B.Y;
	C.Z = A.Z - B.Z;
	C.W = A.W - B.W;
	return C;
}


inline v2D_i& 
operator*=(v2D_i& A, int B)
{
	A.X = A.X * B;
	A.Y = A.Y * B;
	return A;
}

inline v2D_f& 
operator*=(v2D_f& A, int B)
{
	A.X = A.X * B;
	A.Y = A.Y * B;
	return A;
}

inline v3D_i& 
operator*=(v3D_i& A, int B)
{
	A.X = A.X * B;
	A.Y = A.Y * B;
	A.Z = A.Z * B;
	return A;
}

inline v3D_f& 
operator*=(v3D_f& A, int B)
{
	A.X = A.X * B;
	A.Y = A.Y * B;
	A.Z = A.Z * B;
	return A;
}

inline v4D_i& 
operator*=(v4D_i& A, int B)
{
	A.X = A.X * B;
	A.Y = A.Y * B;
	A.Z = A.Z * B;
	A.W = A.W * B;
	return A;
}

inline v4D_f& 
operator*=(v4D_f& A, int B)
{
	A.X = A.X * B;
	A.Y = A.Y * B;
	A.Z = A.Z * B;
	A.W = A.W * B;
	return A;
}


inline v2D_i& 
operator*=(v2D_i& A, r32 B)
{
	A.X = (int)(A.X * B);
	A.Y = (int)(A.Y * B);
	return A;
}

inline v2D_f& 
operator*=(v2D_f& A, r32 B)
{
	A.X = A.X * B;
	A.Y = A.Y * B;
	return A;
}

inline v3D_i& 
operator*=(v3D_i& A, r32 B)
{
	A.X = (int)(A.X * B);
	A.Y = (int)(A.Y * B);
	A.Z = (int)(A.Z * B);
	return A;
}

inline v3D_f& 
operator*=(v3D_f& A, r32 B)
{
	A.X = A.X * B;
	A.Y = A.Y * B;
	A.Z = A.Z * B;
	return A;
}

inline v4D_i& 
operator*=(v4D_i& A, r32 B)
{
	A.X = (int)(A.X * B);
	A.Y = (int)(A.Y * B);
	A.Z = (int)(A.Z * B);
	A.W = (int)(A.W * B);
	return A;
}

inline v4D_f& 
operator*=(v4D_f& A, r32 B)
{
	A.X = A.X * B;
	A.Y = A.Y * B;
	A.Z = A.Z * B;
	A.W = A.W * B;
	return A;
}


inline v2D_i 
operator*(v2D_i A, int B)
{
	v2D_i C = {};
	C.X = A.X * B;
	C.Y = A.Y * B;
	return C;
}

inline v2D_f 
operator*(v2D_f A, int B)
{
	v2D_f C = {};
	C.X = A.X * B;
	C.Y = A.Y * B;
	return C;
}

inline v3D_i 
operator*(v3D_i A, int B)
{
	v3D_i C = {};
	C.X = A.X * B;
	C.Y = A.Y * B;
	C.Z = A.Z * B;
	return C;
}

inline v3D_f 
operator*(v3D_f A, int B)
{
	v3D_f C = {};
	C.X = A.X * B;
	C.Y = A.Y * B;
	C.Z = A.Z * B;
	return C;
}

inline v4D_i 
operator*(v4D_i A, int B)
{
	v4D_i C = {};
	C.X = A.X * B;
	C.Y = A.Y * B;
	C.Z = A.Z * B;
	C.W = A.W * B;
	return C;
}

inline v4D_f 
operator*(v4D_f A, int B)
{
	v4D_f C = {};
	C.X = A.X * B;
	C.Y = A.Y * B;
	C.Z = A.Z * B;
	C.W = A.W * B;
	return C;
}


inline v2D_i 
operator*(v2D_i A, r32 B)
{
	v2D_i C = {};
	C.X = (int)(A.X * B);
	C.Y = (int)(A.Y * B);
	return C;
}

inline v2D_f 
operator*(v2D_f A, r32 B)
{
	v2D_f C = {};
	C.X = A.X * B;
	C.Y = A.Y * B;
	return C;
}

inline v3D_i 
operator*(v3D_i A, r32 B)
{
	v3D_i C = {};
	C.X = (int)(A.X * B);
	C.Y = (int)(A.Y * B);
	C.Z = (int)(A.Z * B);
	return C;
}

inline v3D_f 
operator*(v3D_f A, r32 B)
{
	v3D_f C = {};
	C.X = A.X * B;
	C.Y = A.Y * B;
	C.Z = A.Z * B;
	return C;
}

inline v4D_i 
operator*(v4D_i A, r32 B)
{
	v4D_i C = {};
	C.X = (int)(A.X * B);
	C.Y = (int)(A.Y * B);
	C.Z = (int)(A.Z * B);
	C.W = (int)(A.W * B);
	return C;
}

inline v4D_f 
operator*(v4D_f A, r32 B)
{
	v4D_f C = {};
	C.X = A.X * B;
	C.Y = A.Y * B;
	C.Z = A.Z * B;
	C.W = A.W * B;
	return C;
}


inline v2D_i& 
operator/=(v2D_i& A, int B)
{
	if( B != 0)
	{
		A.X = A.X / B;
		A.Y = A.Y / B;
	}
	else
	{
		A.X = 0;
		A.Y = 0; 	
	}
	return A;
}

inline v2D_f& 
operator/=(v2D_f& A, int B)
{
	if( B != 0)
	{
		A.X = A.X / (r32)B;
		A.Y = A.Y / (r32)B;
	}
	else
	{
		A.X = 0.0f;
		A.Y = 0.0f;
	}
	return A;
}

inline v3D_i& 
operator/=(v3D_i& A, int B)
{
	if( B != 0)
	{
		A.X = A.X / B;
		A.Y = A.Y / B;
		A.Z = A.Z / B;
	}
	else
	{
		A.X = 0;
		A.Y = 0;
		A.Z = 0;
	}
	return A;
}

inline v3D_f& 
operator/=(v3D_f& A, int B)
{
	if( B != 0)
	{
		A.X = A.X / (r32)B;
		A.Y = A.Y / (r32)B;
		A.Z = A.Z / (r32)B;
	}
	else
	{
		A.X = 0.0f;
		A.Y = 0.0f;
		A.Z = 0.0f;
	}
	return A;
}

inline v4D_i& 
operator/=(v4D_i& A, int B)
{
	if( B != 0)
	{
		A.X = A.X / B;
		A.Y = A.Y / B;
		A.Z = A.Z / B;
		A.W = A.W / B;
	}
	else
	{
		A.X = 0;
		A.Y = 0;
		A.Z = 0;
		A.W = 0;	
	}
	return A;
}

inline v4D_f& 
operator/=(v4D_f& A, int B)
{
	if(B != 0)
	{
		A.X = A.X / (r32)B;
		A.Y = A.Y / (r32)B;
		A.Z = A.Z / (r32)B;
		A.W = A.W / (r32)B;
	}
	else
	{
		A.X = 0.0f;
		A.Y = 0.0f;
		A.Z = 0.0f;
		A.W = 0.0f;
	}
	return A;
}


inline v2D_i& 
operator/=(v2D_i& A, r32 B)
{
	if(B != 0.0f)
	{
		r32 X = (r32)A.X / B;
		r32 Y = (r32)A.Y / B; 
		A.X = (X > 0) ? ((int)(X + 0.5f)) : ((int)(X - 0.5f));
		A.Y = (Y > 0) ? ((int)(Y + 0.5f)) : ((int)(Y - 0.5f));
	}
	else
	{
		A.X = 0;
		A.Y = 0;	
	}
	return A;
}

inline v2D_f& 
operator/=(v2D_f& A, r32 B)
{
	if(B != 0.0f)
	{
		A.X = A.X / B;
		A.Y = A.Y / B;		
	}
	else
	{
		A.X = 0.0f;
		A.Y = 0.0f;	
	}
	return A;
}

inline v3D_i& 
operator/=(v3D_i& A, r32 B)
{
	if(B != 0.0f)
	{
		r32 X = (r32)A.X / B;
		r32 Y = (r32)A.Y / B;
		r32 Z = (r32)A.Z / B; 
		A.X = (X > 0) ? ((int)(X + 0.5f)) : ((int)(X - 0.5f));
		A.Y = (Y > 0) ? ((int)(Y + 0.5f)) : ((int)(Y - 0.5f));
		A.Z = (Z > 0) ? ((int)(Z + 0.5f)) : ((int)(Z - 0.5f));
	}
	else
	{
		A.X = 0;
		A.Y = 0;
		A.Z = 0;
	}
	return A;
}

inline v3D_f& 
operator/=(v3D_f& A, r32 B)
{
	if(B != 0.0f)
	{
		A.X = A.X / B;
		A.Y = A.Y / B;
		A.Z = A.Z / B;
	}
	else
	{
		A.X = 0.0f;
		A.Y = 0.0f;
		A.Z = 0.0f;
	}
	return A;
}

inline v4D_i& 
operator/=(v4D_i& A, r32 B)
{
	if(B != 0.0f)
	{
		r32 X = (r32)A.X / B;
		r32 Y = (r32)A.Y / B;
		r32 Z = (r32)A.Z / B;
		r32 W = (r32)A.W / B; 
		A.X = (X > 0) ? ((int)(X + 0.5f)) : ((int)(X - 0.5f));
		A.Y = (Y > 0) ? ((int)(Y + 0.5f)) : ((int)(Y - 0.5f));
		A.Z = (Z > 0) ? ((int)(Z + 0.5f)) : ((int)(Z - 0.5f));
		A.W = (W > 0) ? ((int)(W + 0.5f)) : ((int)(W - 0.5f));
	}
	else
	{
		A.X = 0;
		A.Y = 0;
		A.Z = 0;
		A.W = 0;
	}
	return A;
}

inline v4D_f& 
operator/=(v4D_f& A, r32 B)
{
	if(B != 0.0f)
	{
		A.X = A.X / B;
		A.Y = A.Y / B;
		A.Z = A.Z / B;
		A.W = A.W / B;
	}
	else
	{
		A.X = 0.0f;
		A.Y = 0.0f;
		A.Z = 0.0f;
		A.W = 0.0f;
	}
	return A;
}


inline v2D_i 
operator/(v2D_i A, r32 B)
{
	v2D_i C = {};
	if(B != 0.0f)
	{
		r32 X = (r32)A.X / B;
		r32 Y = (r32)A.Y / B; 
		C.X = (X > 0) ? ((int)(X + 0.5f)) : ((int)(X - 0.5f));
		C.Y = (Y > 0) ? ((int)(Y + 0.5f)) : ((int)(Y - 0.5f));
	}
	else
	{
		C.X = 0;
		C.Y = 0;
	}
	return C;
}

inline v2D_f 
operator/(v2D_f A, r32 B)
{
	v2D_f C = {};

	if(B != 0.0f)
	{
		C.X = A.X / B;
		C.Y = A.Y / B;
	}
	else
	{
		C.X = 0.0f;
		C.Y = 0.0f;
	}
	return C;
}

inline v3D_i 
operator/(v3D_i A, r32 B)
{
	v3D_i C = {};
	
	if(B != 0.0f)
	{
		r32 X = (r32)A.X / B;
		r32 Y = (r32)A.Y / B;
		r32 Z = (r32)A.Z / B; 
		C.X = (X > 0) ? ((int)(X + 0.5f)) : ((int)(X - 0.5f));
		C.Y = (Y > 0) ? ((int)(Y + 0.5f)) : ((int)(Y - 0.5f));
		C.Z = (Z > 0) ? ((int)(Z + 0.5f)) : ((int)(Z - 0.5f));
	}
	else
	{
		C.X = 0;
		C.Y = 0;
		C.Z = 0;
	}
	return C;
}

inline v3D_f 
operator/(v3D_f A, r32 B)
{
	v3D_f C = {};

	if(B != 0.0f)
	{
		C.X = A.X / B;
		C.Y = A.Y / B;
		C.Z = A.Z / B;
	}
	else
	{
		C.X = 0.0f;
		C.Y = 0.0f;
		C.Z = 0.0f;
	}
	return C;
}

inline v4D_i 
operator/(v4D_i A, r32 B)
{
	v4D_i C = {};

	if(B != 0.0f)
	{
		r32 X = (r32)A.X / B;
		r32 Y = (r32)A.Y / B;
		r32 Z = (r32)A.Z / B;
		r32 W = (r32)A.W / B; 
		C.X = (X > 0) ? ((int)(X + 0.5f)) : ((int)(X - 0.5f));
		C.Y = (Y > 0) ? ((int)(Y + 0.5f)) : ((int)(Y - 0.5f));
		C.Z = (Z > 0) ? ((int)(Z + 0.5f)) : ((int)(Z - 0.5f));
		C.W = (W > 0) ? ((int)(W + 0.5f)) : ((int)(W - 0.5f));
	}
	else
	{
		C.X = 0;
		C.Y = 0;
		C.Z = 0;
		C.W = 0;
	}
	return C;
}

inline v4D_f 
operator/(v4D_f A, r32 B)
{
	v4D_f C = {};

	if(B != 0.0f)
	{
		C.X = A.X / B;
		C.Y = A.Y / B;
		C.Z = A.Z / B;
		C.W = A.W / B;
	}
	else
	{
		C.X = 0.0f;
		C.Y = 0.0f;
		C.Z = 0.0f;
		C.W = 0.0f;
	}
	return C;
}


inline void
VectorClamp(v2D_f *Value, v2D_f min, v2D_f max)
{
	if(Value->X < min.X)
	{
		Value->X = min.X;
	}
	if(Value->Y < min.Y)
	{
		Value->Y = min.Y;
	}

	if(Value->X > max.X)
	{
		Value->X = max.X;
	}
	if(Value->Y > max.Y)
	{
		Value->Y = max.Y;
	}
}

inline void
VectorClamp(v2D_i *Value, v2D_i min, v2D_i max)
{
	if(Value->X < min.X)
	{
		Value->X = min.X;
	}
	if(Value->Y < min.Y)
	{
		Value->Y = min.Y;
	}

	if(Value->X > max.X)
	{
		Value->X = max.X;
	}
	if(Value->Y > max.Y)
	{
		Value->Y = max.Y;
	}
}

inline void
VectorClamp(v3D_f *Value, v3D_f min, v3D_f max)
{
	if(Value->X < min.X)
	{
		Value->X = min.X;
	}
	if(Value->Y < min.Y)
	{
		Value->Y = min.Y;
	}
	if(Value->Z > max.Z)
	{
		Value->Z = max.Z;
	}
	
	if(Value->X > max.X)
	{
		Value->X = max.X;
	}
	if(Value->Y > max.Y)
	{
		Value->Y = max.Y;
	}
	if(Value->Z > max.Z)
	{
		Value->Z = max.Z;
	}

}

inline void
VectorClamp(v3D_i *Value, v3D_i min, v3D_i max)
{
	if(Value->X < min.X)
	{
		Value->X = min.X;
	}
	if(Value->Y < min.Y)
	{
		Value->Y = min.Y;
	}
	if(Value->Z > max.Z)
	{
		Value->Z = max.Z;
	}
	
	if(Value->X > max.X)
	{
		Value->X = max.X;
	}
	if(Value->Y > max.Y)
	{
		Value->Y = max.Y;
	}
	if(Value->Z > max.Z)
	{
		Value->Z = max.Z;
	}
}

inline v2D_i
VectorConvert(v2D_f A)
{
	v2D_i Converted = {};
	Converted.X = (int)A.X;
	Converted.Y = (int)A.Y;
	return Converted;
}

inline v2D_f
VectorConvert(v2D_i A)
{
	v2D_f Converted = {};
	Converted.X = (r32)A.X;
	Converted.Y = (r32)A.Y;
	return Converted;
}

inline v3D_i
VectorConvert(v3D_f A)
{
	v3D_i Converted = {};
	Converted.X = (int)A.X;
	Converted.Y = (int)A.Y;
	Converted.Z = (int)A.Z;
	return Converted;
}

inline v3D_f
VectorConvert(v3D_i A)
{
	v3D_f Converted = {};
	Converted.X = (r32)A.X;
	Converted.Y = (r32)A.Y;
	Converted.Z = (r32)A.Z;
	return Converted;
}

r32
VectorToScalar(v2D_f DistanceVector)
{
	r32 Distance = sqrt(DistanceVector.X * DistanceVector.X + DistanceVector.Y * DistanceVector.Y);
	return Distance;
}

v2D_f
VectorAbsolute(v2D_f A)
{
	v2D_f Result = v2D(abs(A.X), abs(A.Y));
	return Result;
}

v2D_i
VectorAbsolute(v2D_i A)
{
	v2D_i Result = v2D(abs(A.X), abs(A.Y));
	return Result;
}

v3D_f
VectorAbsolute(v3D_f A)
{
	v3D_f Result = v3D(abs(A.X), abs(A.Y), abs(A.Z));
	return Result;
}

v3D_i
VectorAbsolute(v3D_i A)
{
	v3D_i Result = v3D(abs(A.X), abs(A.Y), abs(A.Z));
	return Result;
}

v4D_f
VectorAbsolute(v4D_f A)
{
	v4D_f Result = v4D(abs(A.X), abs(A.Y), abs(A.Z), abs(A.W));
	return Result;
}

v4D_i
VectorAbsolute(v4D_i A)
{
	v4D_i Result = v4D(abs(A.X), abs(A.Y), abs(A.Z), abs(A.W));
	return Result;
}

v2D_f
VectorInverse(v2D_f A)
{
	v2D_f Result = v2D((A.X == 0.0f) ? 0.0f : (1.0f / A.X),
					   (A.Y == 0.0f) ? 0.0f : (1.0f / A.Y));
	return Result;
}

v3D_f
VectorInverse(v3D_f A)
{
	v3D_f Result = v3D((A.X == 0.0f) ? 0.0f : (1.0f / A.X),
					   (A.Y == 0.0f) ? 0.0f : (1.0f / A.Y),
					   (A.Z == 0.0f) ? 0.0f : (1.0f / A.Z));
	return Result;
}

v4D_f
VectorInverse(v4D_f A)
{
	v4D_f Result = v4D((A.X == 0.0f) ? 0.0f : (1.0f / A.X),
					   (A.Y == 0.0f) ? 0.0f : (1.0f / A.Y),
					   (A.Z == 0.0f) ? 0.0f : (1.0f / A.Z),
					   (A.W == 0.0f) ? 0.0f : (1.0f / A.W));
	return Result;
}

v2D_f
VectorFloor(v2D_f A)
{
	v2D_f Result = v2D(floor(A.X), floor(A.Y));
	return Result;
}

v3D_f
VectorFloor(v3D_f A)
{
	v3D_f Result = v3D(floor(A.X), floor(A.Y), floor(A.Z));
	return Result;
}

v4D_f
VectorFloor(v4D_f A)
{
	v4D_f Result = v4D(floor(A.X), floor(A.Y), floor(A.Z), floor(A.W));
	return Result;
}

v2D_f
VectorCeil(v2D_f A)
{
	v2D_f Result = v2D(ceil(A.X), ceil(A.Y));
	return Result;
}

v3D_f
VectorCeil(v3D_f A)
{
	v3D_f Result = v3D(ceil(A.X), ceil(A.Y), ceil(A.Z));
	return Result;
}

v4D_f
VectorCeil(v4D_f A)
{
	v4D_f Result = v4D(ceil(A.X), ceil(A.Y), ceil(A.Z), ceil(A.W));
	return Result;
}

v2D_f
VectorMagMin(v2D_f A)
{
	v2D_f Result = v2D((A.X >= 0.0f) ? floor(A.X) : ceil(A.X), 
					   (A.Y >= 0.0f) ? floor(A.Y) : ceil(A.Y));
	return Result;
}

v3D_f
VectorMagMin(v3D_f A)
{
	v3D_f Result = v3D((A.X >= 0.0f) ? floor(A.X) : ceil(A.X), 
					   (A.Y >= 0.0f) ? floor(A.Y) : ceil(A.Y), 
					   (A.Z >= 0.0f) ? floor(A.Z) : ceil(A.Z));
	return Result;
}

v4D_f
VectorMagMin(v4D_f A)
{
	v4D_f Result = v4D((A.X >= 0.0f) ? floor(A.X) : ceil(A.X), 
					   (A.Y >= 0.0f) ? floor(A.Y) : ceil(A.Y), 
					   (A.Z >= 0.0f) ? floor(A.Z) : ceil(A.Z), 
					   (A.W >= 0.0f) ? floor(A.W) : ceil(A.W));
	return Result;
}

v2D_f
VectorMagMax(v2D_f A)
{
	v2D_f Result = v2D((A.X < 0.0f) ? floor(A.X) : ceil(A.X), 
					   (A.Y < 0.0f) ? floor(A.Y) : ceil(A.Y));
	return Result;
}

v3D_f
VectorMagMax(v3D_f A)
{
	v3D_f Result = v3D((A.X < 0.0f) ? floor(A.X) : ceil(A.X), 
					   (A.Y < 0.0f) ? floor(A.Y) : ceil(A.Y), 
					   (A.Z < 0.0f) ? floor(A.Z) : ceil(A.Z));
	return Result;
}

v4D_f
VectorMagMax(v4D_f A)
{
	v4D_f Result = v4D((A.X < 0.0f) ? floor(A.X) : ceil(A.X), 
					   (A.Y < 0.0f) ? floor(A.Y) : ceil(A.Y), 
					   (A.Z < 0.0f) ? floor(A.Z) : ceil(A.Z), 
					   (A.W < 0.0f) ? floor(A.W) : ceil(A.W));
	return Result;
}

r32 VectorDOT(v2D_f A, v2D_f B)
{
	r32 C = A.X * B.X + A.Y * B.Y;
	return C;
}

int VectorDOT(v2D_i A, v2D_i B)
{
	int C = A.X * B.X + A.Y * B.Y;
	return C;
}

r32 VectorDOT(v3D_f A, v3D_f B)
{
	r32 C = A.X * B.X + A.Y * B.Y + A.Z * B.Z;
	return C;
}

int VectorDOT(v3D_i A, v3D_i B)
{
	int C = A.X * B.X + A.Y * B.Y + A.Z * B.Z;
	return C;
}

v2D_i VectorHadamard(v2D_i A, v2D_i B)
{
	v2D_i Result = {};
	Result.X = A.X * B.X;
	Result.Y = A.Y * B.Y;
	
	return Result;
}

v2D_f VectorHadamard(v2D_f A, v2D_f B)
{
	v2D_f Result = {};
	Result.X = A.X * B.X;
	Result.Y = A.Y * B.Y;
	
	return Result;
}

v3D_i VectorHadamard(v3D_i A, v3D_i B)
{
	v3D_i Result = {};
	Result.X = A.X * B.X;
	Result.Y = A.Y * B.Y;
	Result.Z = A.Z * B.Z;
	
	return Result;
}

v3D_f VectorHadamard(v3D_f A, v3D_f B)
{
	v3D_f Result = {};
	Result.X = A.X * B.X;
	Result.Y = A.Y * B.Y;
	Result.Z = A.Z * B.Z;

	return Result;
}

v4D_i VectorHadamard(v4D_i A, v4D_i B)
{
	v4D_i Result = {};
	Result.X = A.X * B.X;
	Result.Y = A.Y * B.Y;
	Result.Z = A.Z * B.Z;
	Result.W = A.W * B.W;
	
	return Result;
}

v4D_f VectorHadamard(v4D_f A, v4D_f B)
{
	v4D_f Result = {};
	Result.X = A.X * B.X;
	Result.Y = A.Y * B.Y;
	Result.Z = A.Z * B.Z;
	Result.W = A.W * B.W;

	return Result;
}

v2D_f VectorNormalize(v2D_f A)
{
	v2D_f Result = {};
	Result = A / VectorToScalar(A);
	return Result;
}

r32 VectorLength(v2D_f A)
{
	r32 Result = 0.0f;
	Result = sqrt(A.X *A.X + A.Y *A.Y);
	return Result;
}

#endif
