#pragma once

#include "raylib.h"

#include "math.h"

namespace utility
{
	static Color lerp_color( Color a, Color b, float t )
	{
		return Color { 
			utility::lerp( a.r, b.r, t ), 
			utility::lerp( a.g, b.g, t ), 
			utility::lerp( a.b, b.b, t ), 
			utility::lerp( a.a, b.a, t ) 
		};
	}
}