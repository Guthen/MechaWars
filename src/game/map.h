#pragma once

#include <vector>
#include <unordered_map>

#include <raylib.h>
#include <raymath.h>

#include "../libs/fast_noise_lite.h"
#include "../utility/math.h"
#include "../utility/int2.h"

#include "../assets_manager.h"
#include "../entity.h"
#include "world_entity.fwd.h"

class Map : public Entity
{
private:
	RenderTexture2D rt;  //  render texture used to batch the tilemap into one render call
	void _rt_update();
	bool should_update_rt;

	Texture tileset_texture;
	std::vector<Rectangle> quads;

	std::vector<unsigned int> tiles;
	std::unordered_map<Int2, std::weak_ptr<WorldEntity>, Int2> structures_reservations;

	unsigned int seed;
public:
	static const int TILE_SIZE = 8;
	const enum TILE
	{
		TILE_SNOW = 1,
		TILE_ICE_FULL,
		TILE_ICE_TOP_LEFT,
		TILE_ICE_TOP_RIGHT,
		TILE_ICE_BOTTOM_LEFT,
		TILE_ICE_BOTTOM_RIGHT,
	};

	Map( int w, int h );
	//~Map() override { printf( "hey map is going out" ); }

	void set_size( const int w, const int h ) override
	{
		Entity::set_size( w, h );
		tiles.resize( w * h );
	}

	void render() override;

	void fill( const int x, const int y, const int w, const int h, const int tile_id );
	void generate_tiles( FastNoiseLite gen_fnl, FastNoiseLite warp_fnl, float river_threshold );
	void generate_trees( FastNoiseLite fnl, float threshold );
	void generate_ores( FastNoiseLite fnl, float threshold );
	void generate( const unsigned int seed );
	void smooth_tiles();
	
	Int2 find_nexus_position( Int2 pos, int search_radius = 10 );
	Int2 get_start_position_for_player( int id );

	bool is_in_bounds( const int x, const int y );
	unsigned int get_tile_id( const int x, const int y );
	unsigned int get_tile_at_pos( const int x, const int y );

	int get_seed() { return seed; }

	std::weak_ptr<WorldEntity> get_structure_at_pos( const int x, const int y );
	void reserve_structure_pos( const int x, const int y, std::weak_ptr<WorldEntity> );
	void unreserve_structure_pos( const int x, const int y );
	bool has_structure_at( const int x, const int y );
};

