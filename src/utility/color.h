#pragma once

#include <raylib.h>

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

	static Color multiply_color( Color a, Color b )
	{
		Vector4 a4 = ColorNormalize( a );
		Vector4 b4 = ColorNormalize( b );

		return ColorFromNormalized( 
			Vector4 {
				a4.x * b4.x,
				a4.y * b4.y,
				a4.z * b4.z,
				a4.w * b4.w
			}
		);
	}

	static Color multiply_color( Color a, float t )
	{
		Vector4 a4 = ColorNormalize( a );

		return ColorFromNormalized(
			Vector4 {
				a4.x * t,
				a4.y * t,
				a4.z * t,
				a4.w
			}
		);
	}
}