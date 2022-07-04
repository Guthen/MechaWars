#pragma once

#include <cmath>
#include <raylib.h>

#include "int2.h"

enum class Cardinal
{
	SOUTH,
	WEST,
	NORTH,
	EAST,
};

namespace utility
{
	static Cardinal get_cardinal( const Int2 from, const Int2 to )
	{
		float fourty_five = PI / 4;
		float one_hundred_thirty_five = PI / 1.33333f;

		double ang = atan2( to.y - from.y, to.x - from.x );
		if ( ang > -fourty_five && ang < fourty_five )
			return Cardinal::EAST;
		if ( ang >= fourty_five && ang <= one_hundred_thirty_five )
			return Cardinal::SOUTH;
		if ( ang < -one_hundred_thirty_five || ang > one_hundred_thirty_five )
			return Cardinal::WEST;

		return Cardinal::NORTH;
	}

	static Int2 get_cardinal_offset( const Cardinal dir )
	{
		switch ( dir )
		{
		case Cardinal::NORTH:
			return Int2 { 0, -1 };
		case Cardinal::SOUTH:
			return Int2 { 0, 1 };
		case Cardinal::WEST:
			return Int2 { -1, 0 };
		case Cardinal::EAST:
			return Int2 { 1, 0 };
		}

		return Int2 { 0, 0 };
	}
};