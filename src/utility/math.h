#pragma once

#include <cmath>

namespace utility
{
	static float lerp( float a, float b, float t ) { return ( 1 - t ) * a + b * t; }
	static float get_direction_angle( Vector2 dir )
	{ return atan2( dir.y, dir.x ) * RAD2DEG; }

	static float distance( float a_x, float a_y, float b_x, float b_y ) 
	{ return sqrt( pow( a_x - b_x, 2 ) + pow( a_y - b_y, 2 ) ); }
	static float clamp( float value, float min, float max ) 
	{ return value < min ? min : value > max ? max : value; }
	static float remap_11_to_01( float value ) 
	{ return ( value + 1.0f ) / 2.0f; }

	static float round( float value, int decimals = 2 )
	{
		float mul = pow( 10, decimals );
		return ceil( value * mul ) / mul;
	}

	//  mathematical functions from https://easings.net/
	static float ease_in_expo( float x ) { return x == 0 ? 0 : powf( 2, 10 * x - 10 ); }
	static float ease_in_cubic( float x ) { return x * x * x; }
}