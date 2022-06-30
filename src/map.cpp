#include "map.h"

#include "game_manager.h"
#include "game/structures/structure_tree.h"
#include "game/structures/structure_steel.h"
#include "game/structures/structure_nexus.h"
#include "game/structures/structure_generator.h"
#include "game/structures/structure_drill.h"
#include "game/structures/structure_silo.h"
#include "game/units/unit_vk2.hpp"
#include "game/units/unit_rider.hpp"

#include <algorithm>
#include <random>
#include <numeric>

Map::Map( int w, int h )
{
	texture = AssetsManager::get_or_load_texture( "assets/textures/tiles.png" );
	quads = AssetsManager::slice_texture( texture, TILE_SIZE, TILE_SIZE );
	set_size( w, h );

	//fill( 0, 0, w, h, 0 );
}

Vector2 zero;
void Map::render() 
{
	Rectangle dest { 0, 0, TILE_SIZE, TILE_SIZE };

	for ( int y = 0; y < size.y; y++ )
		for ( int x = 0; x < size.x; x++ )
		{
			unsigned int tile_id = get_tile_at_pos( x, y );
			dest.x = (float) x * TILE_SIZE, dest.y = (float) y * TILE_SIZE;

			DrawTexturePro( texture, quads[tile_id - 1], dest, zero, 0, WHITE );
		}
}

void Map::fill( const int x, const int y, const int w, const int h, const int tile_id )
{
	for ( int _y = y; _y < h; _y++ )
		for ( int _x = x; _x < w; _x++ )
		{
			tiles[get_tile_id( _x, _y )] = tile_id;//rand() % quads.size();
		}
}

void Map::generate_tiles( FastNoiseLite gen_fnl, FastNoiseLite warp_fnl, float river_threshold )
{
	for ( int y = 0; y < size.y; y++ )
		for ( int x = 0; x < size.x; x++ )
		{
			float nx = (float) x, ny = (float) y;

			//  warp position
			warp_fnl.DomainWarp( nx, ny );

			//  generate noise
			float noise = utility::remap_11_to_01( gen_fnl.GetNoise( nx, ny ) );

			//  assign tile
			unsigned int tile_id = TILE_SNOW;
			if ( noise >= river_threshold )
				tile_id = TILE_ICE_FULL;
			
			//printf( "%d\n", get_tile_id( x, y ) );
			tiles[get_tile_id( x, y )] = tile_id; //  WARNING: crash if !( size.x == size.y ) 
		}

	smooth_tiles();
}

void Map::generate_trees( FastNoiseLite fnl, float threshold )
{
	for ( int y = 0; y < size.y; y++ )
		for ( int x = 0; x < size.x; x++ )
		{
			if ( !( get_tile_at_pos( x, y ) == TILE_SNOW ) || has_structure_at( x, y ) )
				continue;

			float noise = utility::remap_11_to_01( fnl.GetNoise( (float) x, (float) y ) );
			if ( noise >= threshold )
				GameManager::create<StructureTree>( x, y, this );
		}
}

void Map::generate_ores( FastNoiseLite fnl, float threshold )
{
	for ( int y = 0; y < size.y; y++ )
		for ( int x = 0; x < size.x; x++ )
		{
			if ( !( get_tile_at_pos( x, y ) == TILE_SNOW ) || has_structure_at( x, y ) )
				continue;

			float noise = utility::remap_11_to_01( fnl.GetNoise( (float) x, (float) y ) );
			if ( noise >= threshold )
				GameManager::create<StructureSteel>( x, y, this );
		}
}

void Map::generate( const unsigned int _seed )
{
	structures_reservations.clear();

	seed = _seed;
	SetRandomSeed( seed );

	#pragma region TileGeneration
		//  https://github.com/Auburn/FastNoiseLite/wiki/Documentation
		//  configured for generating rivers (thanks to ridged fractal)
		FastNoiseLite gen_fnl( seed );
		gen_fnl.SetNoiseType( FastNoiseLite::NoiseType_OpenSimplex2S );
		gen_fnl.SetFrequency( .025f ); //  default: .01f
		gen_fnl.SetFractalType( FastNoiseLite::FractalType_Ridged ); //  default: None
		gen_fnl.SetFractalGain( .5f ); //  default: .5f
		gen_fnl.SetFractalLacunarity( 2.0f ); //  default: 2.0f
		gen_fnl.SetFractalOctaves( 1 ); //  default: 3

		//  configured to give a natural look to rivers
		FastNoiseLite warp_fnl( seed );
		warp_fnl.SetDomainWarpType( FastNoiseLite::DomainWarpType_OpenSimplex2 );
		warp_fnl.SetDomainWarpAmp( 40.0f );
		warp_fnl.SetFrequency( 0.025f );
		warp_fnl.SetFractalType( FastNoiseLite::FractalType_DomainWarpIndependent );
		warp_fnl.SetFractalOctaves( 2 );
		warp_fnl.SetFractalLacunarity( 2.0f );
		warp_fnl.SetFractalGain( 1.0f );

		generate_tiles( gen_fnl, warp_fnl, .85f );
	#pragma endregion

	#pragma region TreeGeneration
		FastNoiseLite tree_fnl( seed );
		tree_fnl.SetNoiseType( FastNoiseLite::NoiseType_OpenSimplex2 );
		tree_fnl.SetFrequency( .08f );

		generate_trees( tree_fnl, .5f );
	#pragma endregion

	#pragma region OreGeneration
		FastNoiseLite ore_fnl( seed * 2 );
		ore_fnl.SetNoiseType( FastNoiseLite::NoiseType_OpenSimplex2 );
		ore_fnl.SetFrequency( .1f );

		generate_ores( ore_fnl, .9f );
	#pragma endregion

	#pragma region PlayersGeneration
		int n_players = 4;

		//  create a list of players ids
		std::vector<int> players_ids( n_players );
		std::iota( players_ids.begin(), players_ids.end(), 1 );

		//  shuffle players ids (=start positions)
		auto rng = std::default_random_engine { seed };
		std::shuffle( players_ids.begin(), players_ids.end(), rng );

		//  init players
		for ( unsigned int i = 0; i < players_ids.size(); i++ )
		{
			int id = players_ids[i];
			TEAM team = (TEAM) ( i + 1 );

			//  search for a valid nexus position
			Int2 pos = find_nexus_position( get_start_position_for_player( id ));

			//  create nexus
			GameManager::create<StructureNexus>( pos.x, pos.y, this )->set_team( team );
			GameManager::create<StructureGenerator>( pos.x + 2, pos.y, this )->set_team( team );
			GameManager::create<StructureDrill>( pos.x + 2, pos.y + 1, this )->set_team( team );
			GameManager::create<UnitVK2>( pos.x, pos.y + 2, this )->set_team( team );
			GameManager::create<UnitRider>( pos.x + 1, pos.y + 2, this )->set_team( team );
			/*GameManager::create<StructureNexus>( 4 + team * 2, 5, this )->set_team( (TEAM) team );
			GameManager::create<StructureGenerator>( 4 + team * 2, 7, this )->set_team( (TEAM) team );
			GameManager::create<StructureDrill>( 5 + team * 2, 7, this )->set_team( (TEAM) team );
			
			auto silo = GameManager::create<StructureSilo>( 4 + team * 2, 8, this );
			silo->set_team( (TEAM) team );
			silo->set_opened( team % 2 == 0 );*/
		}
	#pragma endregion
}

bool is_ice( int tile ) { return tile == 0 || tile >= Map::TILE_ICE_FULL; }

void Map::smooth_tiles()
{
	for ( int y = 0; y < size.y; y++ ) 
		for ( int x = 0; x < size.x; x++ )
		{
			unsigned int id = get_tile_id( x, y );
			if ( tiles[id] == TILE_ICE_FULL )
			{
				unsigned int left_tile = get_tile_at_pos( x - 1, y );
				unsigned int right_tile = get_tile_at_pos( x + 1, y );
				unsigned int top_tile = get_tile_at_pos( x, y - 1 );
				unsigned int bottom_tile = get_tile_at_pos( x, y + 1 );
			
				#pragma region DiagonalIceSmooth
					//  is neighboors in bounds?
					if ( x - 1 < 0 || y - 1 < 0 || x + 1 >= size.x || y + 1 >= size.y )
						continue;

					//if ( tiles[id] == TILE_ICE_FULL )  //  ensure there is no change since auto-tile
					//{
						//  top left
						unsigned int top_left_tile = get_tile_at_pos( x - 1, y - 1 );
						if ( top_left_tile == TILE_ICE_FULL && top_tile == TILE_SNOW )
						{
							top_tile = TILE_ICE_TOP_RIGHT;
							tiles[get_tile_id( x, y - 1 )] = top_tile;
						}
						if ( top_left_tile == TILE_ICE_FULL && left_tile == TILE_SNOW )
						{
							left_tile = TILE_ICE_BOTTOM_LEFT;
							tiles[get_tile_id( x - 1, y )] = left_tile;
						}

						//  top right
						unsigned int top_right_tile = get_tile_at_pos( x + 1, y - 1 );
						if ( top_right_tile == TILE_ICE_FULL && top_tile == TILE_SNOW )
						{
							top_tile = TILE_ICE_TOP_LEFT;
							tiles[get_tile_id( x, y - 1 )] = top_tile;
						}
						if ( top_right_tile == TILE_ICE_FULL && right_tile == TILE_SNOW )
						{
							right_tile = TILE_ICE_BOTTOM_RIGHT;
							tiles[get_tile_id( x + 1, y )] = right_tile;
						}

						//  bottom right
						unsigned int bottom_right_tile = get_tile_at_pos( x + 1, y + 1 );
						if ( bottom_right_tile == TILE_ICE_FULL && bottom_tile == TILE_SNOW )
						{
							bottom_tile = TILE_ICE_BOTTOM_LEFT;
							tiles[get_tile_id( x, y + 1 )] = bottom_tile;
						}
						if ( bottom_right_tile == TILE_ICE_FULL && right_tile == TILE_SNOW )
						{
							right_tile = TILE_ICE_TOP_RIGHT;
							tiles[get_tile_id( x + 1, y )] = right_tile;
						}

						unsigned int bottom_left_tile = get_tile_at_pos( x - 1, y + 1 );
						if ( bottom_left_tile == TILE_ICE_FULL && bottom_tile == TILE_SNOW )
						{
							bottom_tile = TILE_ICE_BOTTOM_RIGHT;
							tiles[get_tile_id( x, y + 1 )] = bottom_tile;
						}
						if ( bottom_left_tile == TILE_ICE_FULL && left_tile == TILE_SNOW )
						{
							left_tile = TILE_ICE_TOP_LEFT;
							tiles[get_tile_id( x - 1, y )] = left_tile;
						}
					//}
				#pragma endregion

				#pragma region AutoTile
					if ( left_tile == TILE_SNOW && top_tile == TILE_SNOW && is_ice( right_tile ) && is_ice( bottom_tile ) )
						tiles[id] = TILE_ICE_TOP_LEFT;
					else if ( is_ice( left_tile ) && top_tile == TILE_SNOW && right_tile == TILE_SNOW && is_ice( bottom_tile ) )
						tiles[id] = TILE_ICE_TOP_RIGHT;
					else if ( left_tile == TILE_SNOW && is_ice( top_tile ) && is_ice( right_tile ) && bottom_tile == TILE_SNOW )
						tiles[id] = TILE_ICE_BOTTOM_LEFT;
					else if ( is_ice( left_tile ) && is_ice( top_tile ) && right_tile == TILE_SNOW && bottom_tile == TILE_SNOW )
						tiles[id] = TILE_ICE_BOTTOM_RIGHT;
				#pragma endregion
			}
		}
}

/*
 *  From a static position, run a series of iterations to find a position to place a @StructureNexus considering a 
 *  rectangle of size `nexus_size` with no structures nor ice tiles
 */
const Int2 nexus_size { 4, 4 };  //  how much space we have to search for a nexus position
Int2 Map::find_nexus_position( Int2 start_pos, int search_radius )
{
	const Vector2 center_dir = Vector2Normalize( Vector2 { (float) size.x / 2 - (float) start_pos.x, (float) size.y / 2 - (float) start_pos.y } );
	//printf( "center_dir=%f;%f\n", center_dir.x, center_dir.y );

	int max_iters = ( size.x / search_radius / 2 + size.y / search_radius / 2 ) / 2;
	Int2 search_pos { start_pos.x, start_pos.y };
	for ( int iter = 0; iter < max_iters; iter++ )
	{
		search_pos = Int2 { start_pos.x + (int) ( center_dir.x * search_radius * iter ), start_pos.y + (int) ( center_dir.y * search_radius * iter ) };
		//printf( "search_pos=%d;%d\n", search_pos.x, search_pos.y );

		for ( int x = std::max( 0, search_pos.x - search_radius ); x < std::min( size.x - 1, search_pos.x + search_radius ); x++ )
			for ( int y = std::max( 0, search_pos.y - search_radius ); y < std::min( size.y - 1, search_pos.y + search_radius ); y++ )
			{
				if ( !is_in_bounds( x + nexus_size.x - 1, y + nexus_size.y - 1 ) ) continue;

				//  valid in size?
				bool is_valid = true;
				for ( int offset_x = 0; offset_x < nexus_size.x; offset_x++ )
				{
					for ( int offset_y = 0; offset_y < nexus_size.y; offset_y++ )
					{
						if ( 
							!( get_tile_at_pos( x + offset_x, y + offset_y ) == TILE_SNOW ) 
							|| has_structure_at( x + offset_x, y + offset_y ) 
						   )
						{
							is_valid = false;
							break;
						}
					}
				
					if ( !is_valid )
						break;
				}
			
				if ( is_valid )
				{
					//printf( "found at iter %d/%d\n", iter, max_iters );
					return Int2 { x + nexus_size.x / 2 - 1, y + nexus_size.y / 2 - 1 };
				}
			}
	}

	//printf( "not found after %d iterations", max_iters );
	return search_pos;
}

/*
 *  Static start position, might be more efficient with a static array but works fine for now;
 *  Handle 1 to 8 positions (1 <= id <= 8)
 */
Int2 Map::get_start_position_for_player( int id )
{
	switch ( id )
	{
		//  diagonals
		case 1:
			return Int2 {};  //  top-left
		case 2:
			return Int2 { size.x - 2, size.y - 2 };  //  bottom-right
		case 3:	
			return Int2 { 0, size.y - 2 };  //  bottom-left
		case 4:
			return Int2 { size.x - 2, 0 };  //  top-right
		//  lines
		case 5:
			return Int2 { size.x / 2 - 2, 0 };  //  top-middle
		case 6:
			return Int2 { size.x / 2 - 2, size.y - 2 };  //  bottom-middle
		case 7:
			return Int2 { 0, size.y / 2 - 2 };  //  left-middle
		case 8:
			return Int2 { size.x - 2, size.y / 2 - 2 };  //  right-middle
	}

	return Int2 { 0, 0 };
}

bool Map::is_in_bounds( const int x, const int y )
{
	return 0 <= x && x < size.x
		&& 0 <= y && y < size.y;
}

unsigned int Map::get_tile_id( const int x, const int y ) { 
	return x + y * size.y;  
}

unsigned int Map::get_tile_at_pos( const int x, const int y )
{
	if ( !is_in_bounds( x, y ) )
		return 0;

	unsigned int tile_id = get_tile_id( x, y );
	if ( tile_id >= tiles.size() ) return 0;
	return tiles[tile_id];
}

WorldEntity* Map::get_structure_at_pos( const int x, const int y )
{ return structures_reservations[Int2 { x, y }]; }

void Map::reserve_structure_pos( const int x, const int y, WorldEntity* structure )
{ structures_reservations[Int2 { x, y }] = structure; }

void Map::unreserve_structure_pos( const int x, const int y )
{
	if ( structures_reservations.size() <= 0 ) return;

	if ( has_structure_at( x, y ) )
		structures_reservations.erase( Int2 { x, y } );
}

bool Map::has_structure_at( const int x, const int y )
{ return !( structures_reservations.find( Int2 { x, y } ) == structures_reservations.end() ); }

