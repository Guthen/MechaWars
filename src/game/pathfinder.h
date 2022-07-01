#pragma once

#include <raylib.h>
#include "../utility/astar2.hpp"

class Pathfinder
{
private:
	static Astar2 astar;
	static Int2 size;

	static void _error_invalid_id( std::string signature, int node_id, Int2 pos );
public:
	Pathfinder() = delete;

	static void init( int w, int h );
	static void render();

	static void set_pos_disabled( Int2 pos, bool disabled );
	static std::vector<Int2> find_path( Int2 start, Int2 end );
	
	static int get_pos_id( int x, int y );
};