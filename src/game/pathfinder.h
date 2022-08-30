#pragma once

#include <raylib.h>

#include <src/utility/astar2.h>
#include <src/game_manager.h>

class Pathfinder
{
private:
	static Astar2 astar;
	static Int2 size;

	static RenderTexture2D rt;
	static bool should_update_rt;

	static void _error_invalid_id( std::string signature, int node_id, Int2 pos );

	static void _rt_update();
public:
	Pathfinder() = delete;

	static void init( int w, int h );
	static void render();

	static void set_pos_weight( Int2 pos, float weight );
	static void set_pos_disabled( Int2 pos, bool disabled );

	static std::vector<Int2> find_path( Int2 start, Int2 end, bool find_end_nearest = false );
	
	static int get_pos_id( int x, int y );
	static Int2 grid_to_world_pos( int x, int y );
};