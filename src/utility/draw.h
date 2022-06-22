#pragma once

#include "raylib.h"

namespace utility
{
	static void draw_shadow_text( const char* text, const int x, const int y, const int size, const Color main_color = RAYWHITE, const int shadow_offset = 1, const Color shadow_color = BLACK )
	{
		DrawText( text, x + shadow_offset, y + shadow_offset, size, shadow_color );
		DrawText( text, x, y, size, main_color );
	}
}