#pragma once

#include "raylib.h"

const enum TEAM
{
	TEAM_NONE,
	TEAM_RED,
	TEAM_PURPLE,
	TEAM_GREEN,
	TEAM_ORANGE,
	TEAM_BLUE,
	TEAM_GRAY,
	TEAM_YELLOW,
	TEAM_PINK,
};

const Color TEAM_COLORS[9] =
{
	WHITE,
	Color { 230, 59, 67, 255 },  //  red
	Color { 156, 42, 112, 255 }, //  purple
	Color { 99, 171, 63, 255 },  //  green
	Color { 255, 137, 51, 255 }, //  orange
	Color { 79, 164, 184, 255 }, //  blue
	Color { 76, 104, 133, 255 }, //  gray
	Color { 240, 181, 65, 255 }, //  yellow
	Color { 255, 82, 119, 255 }, //  pink
};

Color get_team_color( TEAM team_id ) { return TEAM_COLORS[team_id]; }