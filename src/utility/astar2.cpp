#include "astar2.h"

float Astar2::_compute_cost( Node* from, Node* to ) { return utility::distance_to_sqr( from->pos, to->pos ); }

//  TODO: nodes seems to keep their informations of the last pathfinding, causing inconsistent & weird paths
std::vector<Int2> Astar2::find_path( Node* start, Node* end )
{
	std::vector<Int2> path;

	//  unreachable!
	if ( end->disabled )
	{
		printf( "Error: Astar2::find_path( Node& start, Node& end ): end node is disabled!\n" );
		return path;
	}

	//  create our open list (queue of nodes to visit)
	std::priority_queue<Node*, std::vector<Node*>, std::greater<Node*>> open_list;

	//  create our close list (visited nodes)
	//std::unordered_set<Node*, Node> close_list;

	//  uninitialized our the nodes heuristic cost & parent
	for ( auto &pair : nodes )
	{
		Node* node = &pair.second;
		node->heuristic = FLT_MAX, node->cost = FLT_MAX;
		node->parent = nullptr;
	}

	//  initialize start node
	start->heuristic = 0.0f, start->cost = 0.0f;
	open_list.push( start );


	//  let's goo!
	while ( !open_list.empty() )
	{
		//  retrieve the lowest node
		Node* current = open_list.top();
		open_list.pop();

		//  is it destination?
		if ( current->pos == end->pos )
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

			//  unreachable
			if ( neigh->disabled )
				continue;

			//  update if cost is lower than the previous one
			float cost = current->cost + 1.0f, heuristic = ( cost + _compute_cost( neigh, end ) ) * neigh->weight;
			if ( heuristic < neigh->heuristic )
			{
				neigh->cost = cost;
				neigh->heuristic = heuristic;
				neigh->parent = current;

				//  add to open list
				open_list.push( neigh );
			}
		}

		//  add current node to close list
		//close_list.insert( current );
	}

	return path;
}

std::vector<Int2> Astar2::find_path( int start_id, int end_id )
{
	Node* start_node = &nodes[start_id];
	Node* end_node = &nodes[end_id];
	return find_path( start_node, end_node );
}