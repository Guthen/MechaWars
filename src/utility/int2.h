#pragma once

#include <raylib.h>

#include <cmath>

/*
 *  Similar to Vector2 but with int instead of float
 */
struct Int2
{
	int x;
	int y;

	Int2 operator *( Int2& a ) { return Int2 { x * a.x, y * a.y }; }
	Int2 operator *( int n ) { return Int2 { x * n, y * n }; }

	Int2 operator /( Int2& a ) { return Int2 { x / a.x, y / a.y }; }
	Int2 operator /( int n ) { return Int2 { x / n, y / n }; }

	Int2 operator +( Int2& a ) { return Int2 { x + a.x, y + a.y }; }
	Int2 operator -( Int2& a ) { return Int2 { x - a.x, y - a.y }; }

	//  required to implement std::unordered_set
	bool operator ==( const Int2& a ) const { return x == a.x && y == a.y; }
	size_t operator()( const Int2& a ) const
	{
		return (size_t) ( a.x + a.y * 10 );
	}

	static Int2 from_v2( Vector2& v ) { return Int2 { (int) roundf( v.x ), (int) roundf( v.y ) }; }
	Vector2 to_v2() { return Vector2 { (float) x, (float) y }; }
};