#include "astar2.h"

int Astar2::_compute_cost( Node* from, Node* to )
{
	Int2 dist { 
		abs( from->pos.x - to->pos.x ), 
		abs( from->pos.y - to->pos.y ) 
	};

	if ( dist.x > dist.y )
		return 14 * dist.y + 10 * ( dist.x - dist.y );

	return 14 * dist.x + 10 * ( dist.y - dist.x );
	//return utility::distance_to_sqr( from->pos, to->pos ); 
}

//  TODO: nodes seems to keep their informations of the last pathfinding, causing inconsistent & weird paths
std::vector<Int2> Astar2::find_path( Node* start, Node* end )
{
	NodeComparator comparator;

	std::vector<Int2> path;

	//  unreachable!
	if ( end->disabled )
	{
		printf( "Error: Astar2::find_path( Node& start, Node& end ): end node is disabled!\n" );
		return path;
	}

	//  create our open list (queue of nodes to visit)
	std::vector<Node*> open_list;

	//  create our close list (visited nodes)
	std::unordered_set<Node*, Node> close_list;

	//  uninitialize the nodes costs & parent
	for ( auto &pair : nodes )
	{
		Node* node = &pair.second;
		node->g = 0, node->h = 0, node->f = 0;
		node->parent = nullptr;
	}

	//  initialize start node
	open_list.push_back( start );

	//  let's goo!
	while ( !open_list.empty() )
	{
		//  retrieve the lowest node
		std::pop_heap( open_list.begin(), open_list.end(), comparator );
		Node* current = open_list.back();
		open_list.pop_back();

		//  add current node to close list
		close_list.insert( current );

		//  is it destination?
		if ( current == end )
		{
			//  yep, let's go backwards & make our path!
			Node* parent = current;
			while ( !( parent->parent == nullptr ) )
			{
				path.emplace( path.begin(), parent->pos ); //  insert at first
				parent = parent->parent;
			}

			return path;
		}

		//  loop over neighboors
		for ( int id : current->connections )
		{
			Node* neigh = &nodes[id];

			//  skip if unreachable or exists in close list
			if ( neigh->disabled || !( close_list.find( neigh ) == close_list.end() ) )
				continue;

			//  update if cost is lower than the previous one
			bool not_contained = std::find( open_list.begin(), open_list.end(), neigh ) == open_list.end();
			int g = current->g + _compute_cost( current, neigh ) * neigh->weight;
			if ( g < neigh->g || not_contained )
			{
				neigh->g = g;
				neigh->h = _compute_cost( end, neigh );
				neigh->f = neigh->g + neigh->f;
				neigh->parent = current;

				//  add to open list if not yet
				if ( not_contained )
				{
					open_list.push_back( neigh );
					std::push_heap( open_list.begin(), open_list.end(), comparator );  //  sort neighboor into the heap
				}
			}
		}
	}

	return path;
}

std::vector<Int2> Astar2::find_path( int start_id, int end_id )
{
	Node* start_node = &nodes[start_id];
	Node* end_node = &nodes[end_id];
	return find_path( start_node, end_node );
}