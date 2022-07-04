#include "game_manager.h"

#include <algorithm>

std::shared_ptr<Scene> GameManager::current_scene;

float GameManager::time;

DEBUG_STATE GameManager::debug_state;

std::vector<lambda> GameManager::defereds;
std::vector<Timer> GameManager::timers;
std::vector<std::string> GameManager::debug_texts;

std::vector<std::shared_ptr<Entity>> GameManager::awaiting_queue;
std::vector<std::shared_ptr<Entity>> GameManager::entities;
std::vector<std::shared_ptr<Entity>> GameManager::deletion_queue;

bool GameManager::is_queued_sorting;
int GameManager::sorting_frames_delay;
bool GameManager::_is_clearing;

void GameManager::change_scene_to( std::shared_ptr<Scene> scene )
{
	clear();

	current_scene = scene;
	current_scene->init();
	printf( "GameManager: scene changed!\n" );
}

void GameManager::add_entity( std::shared_ptr<Entity> entity ) 
{ awaiting_queue.push_back( entity ); }

void GameManager::remove_entity( std::shared_ptr<Entity>& entity )
{
	auto iterator = std::find_if( entities.begin(), entities.end(), [&]( std::shared_ptr<Entity>& c ) { return c == entity; });
	if ( iterator == entities.end() ) throw std::exception( "Attempt to remove an invalid entity!" ); //  tried to remove an invalid entity

	//const unsigned int id = entity->get_id();
	entities.erase( iterator );
	//printf( "GameManager: Entity[%d] has been removed!\n", id );
}

void GameManager::queue_entity_to_deletion( std::shared_ptr<Entity> entity )
{ deletion_queue.push_back( entity ); }

void GameManager::clear()
{
	//  reset global id
	Entity::global_id = 0;

	//  clear entities
	_is_clearing = true;
	size_t count = entities.size();
	entities.clear();
	_is_clearing = false;

	printf( "GameManager: clear, %d entities removed\n", count );
}

void GameManager::free()
{
	_is_clearing = true;
	entities.clear();
	_is_clearing = false;
}

bool sort_z_order( std::shared_ptr<Entity> a, std::shared_ptr<Entity> b )
{
	return a->get_z_order() < b->get_z_order();
}

void GameManager::sort( int frames_delay )
{
	if ( frames_delay <= 0 )
	{
		std::sort( entities.begin(), entities.end(), sort_z_order );

		is_queued_sorting = false;
		printf( "GameManager: sorted!\n" );
	}
	else
	{
		sorting_frames_delay = frames_delay;
		is_queued_sorting = true;
	}
}

void GameManager::handle_input()
{
	int mouse_button = -1;
	bool is_pressed = false;

	for ( int button = MOUSE_BUTTON_LEFT; button < MOUSE_BUTTON_BACK; button++ )
	{
		if ( IsMouseButtonPressed( button ) )
		{
			mouse_button = button;
			is_pressed = true;
			break;
		}
		else if ( IsMouseButtonReleased( button ) )
		{
			mouse_button = button;
			is_pressed = false;
			break;
		}
	}

	if ( mouse_button >= 0 )
		for ( const std::shared_ptr<Entity>& ent : entities )
		{
			bool is_handled = ent->unhandled_mouse_click( mouse_button, is_pressed );
			if ( is_handled )
				break;
		}
}

void GameManager::update( float dt )
{
	time += dt;

	//  update awaiting queue
	size_t size = awaiting_queue.size();
	if ( size > 0 )
	{
		//  add to entities list
		for ( const std::shared_ptr<Entity>& ent : awaiting_queue )
			entities.push_back( ent );

		awaiting_queue.clear();

		printf( "GameManager: awaiting queue cleared (%d added)!\n", size );

		//  sort
		sort();
	}

	//  update entities
	handle_input();
	for ( const std::shared_ptr<Entity>& ent : entities )
		ent->update( dt );

	//  update deletion queue
	size = deletion_queue.size();
	if ( size > 0 )
	{
		for ( std::shared_ptr<Entity>& ent : deletion_queue )
			remove_entity( ent );
		deletion_queue.clear();

		printf( "GameManager: deletion queue cleared (%d removed)!\n", size );
	}

	//  sort
	if ( is_queued_sorting )
	{
		if ( sorting_frames_delay <= 0 )
			sort();
		sorting_frames_delay--;
	}

	//  defered
	size = defereds.size();
	if ( size > 0 )
	{
		for ( const lambda call : defereds )
			call();
		defereds.clear();
		
		printf( "GameManager: called %d defered!\n", size );
	}

	//  timers
	size = timers.size();
	if ( size > 0 )
	{
		std::vector<Timer>::iterator it = timers.begin();
		while ( !( it == timers.end() ) )
		{
			it->time -= dt;
			if ( it->time <= 0 )
			{
				it->call();
				it = timers.erase( it );
				//printf( "GameManager: a timer has been deleted!\n" );
			}
			else
				it++;
		}
	}
}

void GameManager::render()
{
	for ( const std::shared_ptr<Entity>& ent : entities )
		ent->render();
}

void GameManager::render_hud()
{
	for ( const std::shared_ptr<Entity>& ent : entities )
		ent->render_hud();

	//  draw debug
	if ( !debug_texts.empty() )
	{
		Int2 text_pos { 20, 130 };
		for ( std::string text : debug_texts )
		{
			utility::draw_shadow_text( text.c_str(), text_pos.x, text_pos.y, 12, RAYWHITE);
			text_pos.y += 20;
		}

		debug_texts.clear();
	}
}