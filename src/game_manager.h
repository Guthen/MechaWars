#pragma once

#include <memory>
#include <vector>

#include "scene.hpp"
#include "entity.h"

#include "utility/defered_call.hpp"

class GameManager
{
private:
	static Scene* current_scene;

	static std::vector<lambda> defereds;

	//  std::shared_ptr is necessary to contain the values even after the creation & exiting current scope
	//  it also manages the deallocation of the Entity by itself (since I use the 'new' keyword which allocates on the heap)
	static std::vector<std::shared_ptr<Entity>> awaiting_queue;
	static std::vector<std::shared_ptr<Entity>> entities;
	static std::vector<Entity*> deletion_queue;

	static bool is_queued_sorting;
	static int sorting_frames_delay;
	static bool _is_clearing;
public:
	GameManager() = delete;

	template <typename T, typename... Args> 
	static T* create( Args... args )
	{
		auto instance = new T( args... );
		std::shared_ptr<T> ptr = std::shared_ptr<T>( instance );
		add_entity( ptr );
		return ptr.get();
	}

	template <typename T, typename... Args>
	static void change_scene( Args... args ) 
	{ change_scene_to( create<T>( args... ) ); }
	static void change_scene_to( Scene* scene );

	static void call_defered( lambda call );

	static void add_entity( std::shared_ptr<Entity> entity );
	static void remove_entity( Entity* entity );
	static void queue_entity_to_deletion( Entity* entity );

	static void clear();
	static void free();
	static void sort( int frames_delay = 0 );

	static void handle_input();

	static void call_update( float dt );
	static void call_render();
	static void call_render_hud();

	static bool is_clearing() { return _is_clearing; }
	static std::vector<std::shared_ptr<Entity>> get_entities() { return entities; }
};

//#define DEFERED( code )  GameManager::call_defered( DeferedCall( [&]() { code } ) )  //  macro are awesome