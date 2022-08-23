#pragma once

#include "int2.h"
#include <cmath>

#include "raymath.h"

namespace utility
{
	static float lerp( float a, float b, float t ) { return ( 1.0f - t ) * a + b * t; }
	static unsigned char lerp( unsigned char a, unsigned char b, float t ) { return (unsigned char) ( ( 1.0f - t ) * a + b * t ); }

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

	//  Projectile Linear Prediction from a GDC talk:
	//  https://www.youtube.com/watch?v=6OkhjWUIUf0&ab_channel=GDC
	static Vector2 get_linear_predicted_position( Vector2 target_pos, Vector2 target_vel, Vector2 bullet_pos, float bullet_speed )
	{
		//  let's solve a quadratic equation
		//  get a
		float target_vel_scalar = Vector2DotProduct( target_vel, target_vel );
		float a = target_vel_scalar - bullet_speed * bullet_speed;

		//  get b
		Vector2 dir = Vector2 { target_pos.x - bullet_pos.x, target_pos.y - bullet_pos.y };
		float b = Vector2DotProduct( Vector2Scale( dir, 2.0f ), target_vel );

		//  get c
		float c = Vector2DotProduct( dir, dir );

		//  compute t
		float t = 0.0f;
		if ( a == 0.0f )
			t = -c / b;
		else
		{
			float discriminant = b * b - 4.0f * a * c;
			if ( discriminant < 0.0f )  //  no solution: no prediction
				return target_pos;
			else if ( discriminant == 0.0f )  //  d=0: only one solution
				t = -b / ( 2.0f * a );
			else  //  d>0: two possible solutions
			{
				float discriminant_unsqr = sqrtf( discriminant );
				float t1 = ( -b + discriminant_unsqr ) / ( 2.0f * a );
				float t2 = ( -b - discriminant_unsqr ) / ( 2.0f * a );
				
				//  choose the minimum
				if ( t1 > t2 )
					t = t2;
				else
					t = t1;
			}
		}

		return Vector2Add( target_pos, Vector2Scale( target_vel, t ) );
	}

	//  mathematical functions from https://easings.net/
	static float ease_in_expo( float x ) { return x == 0 ? 0 : powf( 2, 10 * x - 10 ); }
	static float ease_in_cubic( float x ) { return x * x * x; }
}