#pragma once

#include "int2.h"
#include <cmath>

namespace utility
{
	static float lerp( float a, float b, float t ) 
	{ return ( 1 - t ) * a + b * t; }

	static float approach( float current, float target, float delta )
	{
		delta = abs( delta );

		if ( current < target )
			return fmin( current + delta, target );
		else if ( current > target )
			return fmax( current - delta, target );
		
		return target;
	}

	static float get_direction_angle( Vector2 dir )
	{ return atan2( dir.y, dir.x ) * RAD2DEG; }

	static float distance_to_sqr( float a_x, float a_y, float b_x, float b_y ) 
	{ return powf( a_x - b_x, 2 ) + powf( a_y - b_y, 2 ); }
	static float distance_to_sqr( Int2 a, Int2 b )
	{ return distance_to_sqr( (float) a.x, (float) a.y, (float) b.x, (float) b.y ); }

	static float distance( float a_x, float a_y, float b_x, float b_y ) 
	{ return sqrtf( distance_to_sqr( a_x, a_y, b_x, b_y ) ); }
	static float distance( Int2 a, Int2 b ) 
	{ return distance( (float) a.x, (float) a.y, (float) b.x, (float) b.y ); }

	static float clamp( float value, float min, float max ) 
	{ return value < min ? min : value > max ? max : value; }

	static float remap_11_to_01( float value ) 
	{ return ( value + 1.0f ) / 2.0f; }

	static float round( float value, float decimals = 2.0f )
	{
		float mul = powf( 10, decimals );
		return ceil( value * mul ) / mul;
	}

	//  mathematical functions from https://easings.net/
	static float ease_in_expo( float x ) { return x == 0 ? 0 : powf( 2, 10 * x - 10 ); }
	static float ease_in_cubic( float x ) { return x * x * x; }
}