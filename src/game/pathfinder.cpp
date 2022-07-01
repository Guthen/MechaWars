#include "pathfinder.h"
#include "../map.h"

Astar2 Pathfinder::astar;
Int2 Pathfinder::size;

void Pathfinder::_error_invalid_id( std::string signature, int node_id, Int2 pos )
{
	printf( "Error: Pathfinder::%s: Node[%d] at x=%d, y=%d doesn't exists!", signature.c_str(), node_id, pos.x, pos.y);
}

void Pathfinder::init( int w, int h )
{
	size.x = w, size.y = h;

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
	//  NOTE: rendering performance are bad, drop to 30FPS with a 64x64 map
	//        hopefully it's a debug tool, but might be better to render once to a batch instead
	Int2 tile_size = Int2 { Map::TILE_SIZE, Map::TILE_SIZE };
	Int2 center = tile_size / 2;

	//  debug drawing
	for ( auto &pair : astar.get_nodes() )
	{
		auto &node = pair.second;
		
		//  node
		Int2 node_pos = node.pos * tile_size + center;
		DrawCircleLines( node_pos.x, node_pos.y, 2, node.disabled ? RED : GREEN );

		//  connections
		for ( int id : node.connections )
		{
			auto &neigh = astar.get_node( id );
			Int2 neigh_pos = neigh.pos * tile_size + center;
			DrawLine( node_pos.x, node_pos.y, neigh_pos.x, neigh_pos.y, ( node.disabled || neigh.disabled ) ? RED : GREEN );
		}
	}
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