#pragma once

#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "int2.h"
#include "math.h"

/*
 * My very first Implementation of A*, in C++
 * 
 * Due to the access on nodes and their connections, it's quite modulable and not only specific to a 2D Tilemap
 * (thanks Godot Engine)
 * 
 * Sources: 
 * - https://dev.to/jansonsa/a-star-a-path-finding-c-4a4h
 * - https://fr.wikipedia.org/wiki/Algorithme_A*  <--  '*' is in the URL
 * - https://en.wikipedia.org/wiki/A*_search_algorithm
 * - https://docs.godotengine.org/fr/stable/classes/class_astar2d.html
 * 
 * @Vyrkx (A.K.A. Guthen, A.K.A. Arthur CATHELAIN)
 * June 2022
 */

class Astar2
{
public:
	struct Node
	{
		Node() {};
		Node( int id, int x, int y, float weight = 1.0f ) : id( id ), pos( Int2 { x, y } ), weight( weight ) {};
		
		int id = -1;
		Int2 pos { -1, -1 };
		float cost = 0.0f, heuristic = 0.0f, weight = 1.0f;
		bool disabled = false;
		std::unordered_set<int> connections; //  set of neighboors IDs
		Node* parent = nullptr;

		bool operator ==( const Node& n ) const { return id == n.id; }
		bool operator <( const Node& n ) const { return heuristic < n.heuristic; }
		bool operator >( const Node& n ) const { return heuristic > n.heuristic; }

		//  hash for std::unordered_set
		size_t operator()( const Node& n ) const { return std::hash<int>()( n.id ); }
		size_t operator()( const Node* n ) const { return std::hash<int>()( n->id ); }
	};
protected:
	std::unordered_map<int, Node> nodes;

	float _compute_cost( Node* from, Node* to );
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
			nodes[to_id].connections.insert( from_id );
	}
	void disconnect_node_to( int from_id, int to_id ) { nodes[from_id].connections.erase( to_id ); }
	bool is_node_connected_to( int from_id, int to_id )
	{
		auto &connections = nodes[from_id].connections;
		return !( connections.find( to_id ) == connections.end() );
	}

	///  pathfinding
	std::vector<Int2> find_path( Node* start, Node* end );
	std::vector<Int2> find_path( int start_id, int end_id );
};
