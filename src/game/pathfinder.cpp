#include "pathfinder.h"
#include "../map.h"

/*
 * Define the Quality multiplier of the Render Texture
* 
 * Since it's a debug tool, it's not that useful and we should not upscale this value too much:
 * 1 looks very well with the pixel art style, but it can hide more informations we could add to the renderer (e.g. text)
 * 2 is better
 */
#define RT_QUALITY  2


Astar2 Pathfinder::astar;
Int2 Pathfinder::size;

RenderTexture2D Pathfinder::rt;
bool Pathfinder::should_update_rt;


void Pathfinder::_error_invalid_id( std::string signature, int node_id, Int2 pos )
{
	printf( "Error: Pathfinder::%s: Node[%d] at x=%d, y=%d doesn't exists!", signature.c_str(), node_id, pos.x, pos.y);
}

void Pathfinder::_rt_update()
{
	BeginTextureMode( rt );
	ClearBackground( Color { 0, 0, 0, 0 } );  //  must be transparent

	for ( auto& pair : astar.get_nodes() )
	{
		auto& node = pair.second;

		//  node
		Vector2 node_pos = ( grid_to_world_pos( node.pos.x, node.pos.y ) * RT_QUALITY ).to_v2();
		DrawCircle( node_pos.x, node_pos.y, RT_QUALITY, node.disabled ? RED : GREEN );
		//DrawText( TextFormat( "%d", node.heuristic ), node_pos.x, node_pos.y, 0.1f, GRAY);

		//  connections
		if ( !node.disabled )
		{
			for ( int id : node.connections )
			{
				auto& neigh = astar.get_node( id );
				Vector2 neigh_pos = ( grid_to_world_pos( neigh.pos.x, neigh.pos.y ) * RT_QUALITY ).to_v2();
				DrawLineEx( node_pos, neigh_pos, RT_QUALITY, ( neigh.disabled ) ? RED : GREEN );
			}
		}
	}

	EndTextureMode();
}

void Pathfinder::init( int w, int h )
{
	size.x = w, size.y = h;

	//  update rt
	rt = LoadRenderTexture( w * Map::TILE_SIZE * RT_QUALITY, h * Map::TILE_SIZE * RT_QUALITY );
	should_update_rt = true;

	//  init nodes
	astar.reserve_nodes( w * h );
	for ( int y = 0; y < h; y++ )
		for ( int x = 0; x < w; x++ )
		{
			//  create node
			astar.add_node( get_pos_id( x, y ), x, y, 1.0f );
		}

	//  connect with adjacents nodes
	for ( auto &pair : astar.get_nodes() )
	{
		auto& node = pair.second;

		for ( int neigh_y = std::max( 0, node.pos.y - 1 ); neigh_y <= std::min( h - 1, node.pos.y + 1 ); neigh_y++ )
			for ( int neigh_x = std::max( 0, node.pos.x - 1 ); neigh_x <= std::min( w - 1, node.pos.x + 1 ); neigh_x++ )
			{
				//  don't connect node to itself
				if ( neigh_x == node.pos.x && neigh_y == node.pos.y )
					continue;
				
				//  don't connect to unexisting nodes
				int neigh_id = get_pos_id( neigh_x, neigh_y );
				if ( !astar.has_node( neigh_id ) )
					continue;

				astar.connect_node_to( node.id, neigh_id, true );
			}
	}
}

void Pathfinder::render()
{
	//  NOTE: rendering performance were bad: drop to 17FPS with a 64x64 map
	//  so I draw everything in a render texture when an update is needed, work like a charm! :)
	//  I'm gonna do the same for the Map rendering..
	if ( !GameManager::is_debug_state( DEBUG_STATE::PATHFINDING ) )
		return;
	
	if ( should_update_rt )
	{
		_rt_update();
		should_update_rt = false;
	}

	Rectangle source { 0, 0, (float) rt.texture.width, (float) -rt.texture.height };
	Rectangle dest { 0, 0, source.width / RT_QUALITY, -source.height / RT_QUALITY };
	DrawTexturePro( rt.texture, source, dest, Vector2 { 0 ,0 }, 0.0f, WHITE );
}

void Pathfinder::set_pos_weight( Int2 pos, float weight )
{
	int node_id = get_pos_id( pos.x, pos.y );
	if ( !astar.has_node( node_id ) )
	{
		_error_invalid_id( "set_pos_weight( Int2 pos, float weight )", node_id, pos );
		return;
	}

	astar.set_node_weight( node_id, weight );
}

void Pathfinder::set_pos_disabled( Int2 pos, bool disabled )
{
	int node_id = get_pos_id( pos.x, pos.y );
	if ( !astar.has_node( node_id ) )
	{
		_error_invalid_id( "set_pos_disabled( Int2 pos, bool disabled )", node_id, pos );
		return;
	}

	astar.set_node_disabled( node_id, disabled );
	should_update_rt = true;
}

std::vector<Int2> Pathfinder::find_path( Int2 start, Int2 end )
{
	int start_id = get_pos_id( start.x, start.y );
	if ( !astar.has_node( start_id ) )
	{
		_error_invalid_id( "find_path( Int2 start, Int2 end )", start_id, start );
		return std::vector<Int2>();
	}

	int end_id = get_pos_id( end.x, end.y );
	if ( !astar.has_node( start_id ) )
	{
		_error_invalid_id( "find_path( Int2 start, Int2 end )", end_id, end );
		return std::vector<Int2>();
	}

	return astar.find_path( start_id, end_id );
}

int Pathfinder::get_pos_id( int x, int y )
{ return x + y * size.x; }

Int2 Pathfinder::grid_to_world_pos( int x, int y )
{
	return Int2 { 
		x * Map::TILE_SIZE + Map::TILE_SIZE / 2, 
		y * Map::TILE_SIZE + Map::TILE_SIZE / 2 
	};
}