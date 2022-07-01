#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "int2.h"
#include "math.h"

/*
 * My very first Implementation of A*, in C++
 * Sources: 
 * - https://dev.to/jansonsa/a-star-a-path-finding-c-4a4h
 * - https://fr.wikipedia.org/wiki/Algorithme_A*  <--  '*' is in the URL
 * - https://docs.godotengine.org/fr/stable/classes/class_astar2d.html
 * 
 * @Vyrkx (A.K.A. Guthen, A.K.A. Arthur CATHELAIN)
 */

class Astar2
{
public:
	struct Node
	{
		Node() : id( -1 ), pos( Int2 { -1, -1 } ) {};
		Node( int id, int x, int y, float weight = 1.0f ) : id( id ), pos( Int2 { x, y } ), weight( weight ) {};
		
		int id;
		Int2 pos;
		float cost = 0.0f, dist = 0.0f, heuristic = 0.0f, weight = 1.0f;
		bool disabled = false;
		std::unordered_set<int> connections; //  set of neighboors IDs
		Node* parent = nullptr;

		bool operator ==( const Node& n ) const { return id == n.id; }
		bool operator <( const Node& n ) const { return heuristic < n.heuristic; }
		bool operator >( const Node& n ) const { return heuristic > n.heuristic; }

		//  hash for std::unordered_set
		size_t operator()( const Node& n ) const { return std::hash<int>()( n.id ); }
	};
protected:
	std::unordered_map<int, Node> nodes;

	float _compute_cost( Node from, Node to )
	{ return utility::distance_to_sqr( from.pos, to.pos ); }
public:
	Astar2() {};

	///  node management
	//  add, remove, has, get
	void add_node( int id, int x, int y, float weight ) { nodes[id] = Node( id, x, y, weight ); }
	void remove_node( int id ) { nodes.erase( id ); }
	bool has_node( int id ) { return !( nodes.find( id ) == nodes.end() ); }
	void reserve_nodes( size_t size ) { nodes.reserve( size ); }
	std::unordered_map<int, Node>& get_nodes() { return nodes; }
	Node& get_node( int id ) { return nodes[id]; }

	//  setters
	void set_node_disabled( int id, bool disabled ) { nodes[id].disabled = disabled; }
	void set_node_weight( int id, float weight ) { nodes[id].weight = weight; }

	//  connections
	void connect_node_to( int from_id, int to_id, bool bidirectional = true )
	{
		nodes[from_id].connections.insert( to_id );
		if ( bidirectional )
			nodes[to_id].connections.insert( to_id );
	}
	void disconnect_node_to( int from_id, int to_id ) { nodes[from_id].connections.erase( to_id ); }
	bool is_node_connected_to( int from_id, int to_id )
	{
		auto &connections = nodes[from_id].connections;
		return !( connections.find( to_id ) == connections.end() );
	}

	///  pathfinding
	std::vector<Int2> find_path( Node& start, Node& end )
	{
		std::vector<Int2> path;

		//  unreachable!
		if ( start.disabled || end.disabled )
			return path;

		//  create our open list (queue of nodes to visit)
		std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open_list;

		//  create our close list (visited nodes)
		std::unordered_set<Node, Node> close_list;

		//  uninitialized our the nodes heuristic cost & parent
		for ( auto &pair : nodes )
		{
			Node& node = pair.second;
			node.heuristic = FLT_MAX;
			node.parent = nullptr;
		}

		//  initialize start node
		start.heuristic = 0.0f, start.cost = 0.0f, start.dist = 0.0f;
		open_list.push( start );

		//  let's goo!
		while ( !open_list.empty() )
		{
			//  retrieve the lowest node
			Node current = open_list.top();
			open_list.pop();

			//  is it destination?
			if ( current.pos == end.pos )
			{
				//  yep, let's go backwards & make our path!
				while ( !( current.parent == nullptr ) )
				{
					path.push_back( current.pos );
					current = *current.parent;
				}

				return path;
			}
			
			//  loop over neighboors
			for ( int id : current.connections )
			{
				Node& neigh = nodes[id];

				//  unreachable
				if ( neigh.disabled )
					continue;

				//  check it's not already visited
				if ( close_list.find( neigh ) == close_list.end() )
				{
					float cost = current.cost + 1.0f;
					float dist = _compute_cost( neigh, current );
					float heuristic = cost + dist;

					//  is uninitialized or is present in open list with a lower cost?
					if ( neigh.heuristic == FLT_MAX || neigh.heuristic < heuristic )
					{
						neigh.cost = cost;
						neigh.dist = dist;
						neigh.heuristic = heuristic;
						neigh.parent = &current;

						//  add to open list
						open_list.push( neigh );
					}
				}
			}

			//  add current node to close list
			close_list.insert( current );
		}

		return path;
	}

	std::vector<Int2> find_path( int start_id, int end_id )
	{
		Node& start_node = nodes[start_id];
		Node& end_node = nodes[end_id];
		return find_path( start_node, end_node );
	}
	
};
