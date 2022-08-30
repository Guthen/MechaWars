#pragma once

#include <memory>
#include <vector>

#include "scene.hpp"
#include "entity.h"

#include <src/utility/defered_call.hpp>
#include <src/utility/draw.h>

#define DRAW_DEBUG( text )  GameManager::debug_text( text )

#define DEBUG_STATE_COUNT  3
enum class DEBUG_STATE
{
	NONE,
	ENTITY,
	PATHFINDING,
};


class GameManager
{
private:
	static std::shared_ptr<Scene> current_scene;

	static float time;

	static DEBUG_STATE debug_state;

	static std::vector<lambda> defereds;
	static std::vector<Timer> timers;
	static std::vector<std::string> debug_texts;

	//  std::shared_ptr is necessary to contain the values even after the creation & exiting current scope
	//  it also manages the deallocation of the Entity by itself (since I use the 'new' keyword which allocates on the heap)
	//  it also a good way to prevent dangling pointers with the use of std::weak_ptr
	static std::vector<std::shared_ptr<Entity>> awaiting_queue;
	static std::vector<std::shared_ptr<Entity>> entities;
	static std::vector<std::shared_ptr<Entity>> deletion_queue;

	static bool is_queued_sorting;
	static int sorting_frames_delay;
	static bool _is_clearing;
public:
	GameManager() = delete;

	template <typename T, typename... Args> 
	static std::shared_ptr<T> create( Args... args )
	{
		std::shared_ptr<T> ptr( new T( args... ) );
		if ( !ptr )
		{
			printf( "Error: OUT OF MEMORY!" );
			exit( 1 );
		}

		add_entity( ptr );
		return ptr;
	}

	static void reset();

	template <typename T, typename... Args>
	static void change_scene( Args... args ) 
	{ change_scene_to( create<T>( args... ) ); }
	static void change_scene_to( std::shared_ptr<Scene> scene );

	static void defered_call( lambda call )
	{
		printf( "defereds.push_back\n" );
		defereds.push_back( call );
	}
	static void timed_call( float time, lambda call )
	{ timers.push_back( Timer( time, call ) ); }
	static void debug_text( std::string text )
	{ debug_texts.push_back( text ); }

	static void add_entity( std::shared_ptr<Entity> entity );
	static void remove_entity( std::shared_ptr<Entity>& entity );
	static void queue_entity_to_deletion( std::shared_ptr<Entity> entity );

	static void clear();
	static void free();
	static void sort( int frames_delay = 0 );

	static void handle_input();

	static void update( float dt );
	static void render();
	static void render_hud();

	static bool is_clearing() { return _is_clearing; }
	static std::vector<std::shared_ptr<Entity>> get_entities() { return entities; }
	static float get_time() { return time; }

	static void set_debug_state( DEBUG_STATE state ) { debug_state = state; }
	static bool is_debug_state( DEBUG_STATE state ) { return debug_state == state; }
	static DEBUG_STATE get_debug_state() { return debug_state; }
};