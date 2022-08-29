#pragma once

#include <raylib.h>

namespace utility
{
	static Rectangle rect( Int2 pos, Int2 size )
	{
		return Rectangle {
			(float) pos.x, (float) pos.y,
			(float) size.x, (float) size.y
		};
	}

	static Rectangle rect_expand( Rectangle rect, float amount )
	{
		return Rectangle {
			rect.x - amount, rect.y - amount,
			rect.width + amount * 2.0f, rect.height + amount * 2.0f
		};
	}
}