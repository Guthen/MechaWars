#include "game_manager.h"

#include <algorithm>

Scene* GameManager::current_scene;

std::vector<lambda> GameManager::defereds;

std::vector<std::shared_ptr<Entity>> GameManager::awaiting_queue;
std::vector<std::shared_ptr<Entity>> GameManager::entities;
std::vector<Entity*> GameManager::deletion_queue;

bool GameManager::is_queued_sorting;
int GameManager::sorting_frames_delay;
bool GameManager::_is_clearing;

void GameManager::change_scene_to( Scene* scene )
{
	clear();

	current_scene = scene;
	current_scene->init();
	printf( "GameManager: scene changed!\n" );
}

void GameManager::call_defered( lambda call )
{ defereds.push_back( call ); }

void GameManager::add_entity( std::shared_ptr<Entity> entity ) 
{ awaiting_queue.push_back( entity ); }

void GameManager::remove_entity( Entity* entity )
{
	auto iterator = std::find_if( entities.begin(), entities.end(), [&]( std::shared_ptr<Entity> c ) { return c.get() == entity; });
	if ( iterator == entities.end() ) throw std::exception( "Attempt to remove an invalid entity!" ); //  tried to remove an invalid entity

	//const unsigned int id = entity->get_id();
	entities.erase( iterator );

	//printf( "GameManager: Entity[%d] has been removed!\n", id );
}

void GameManager::queue_entity_to_deletion( Entity* entity )
{
	deletion_queue.push_back( entity );
}

void GameManager::clear()
{
	Entity::global_id = 0;
	_is_clearing = true;

	int count = 0;
	std::vector<std::shared_ptr<Entity>>::iterator i = entities.begin();
	while ( !( i == entities.end() ) )
	{
		if ( ( *i )->is_clearable )
		{
			i = entities.erase( i );
			count++;
		}
		else
			i++;
	}

	printf( "GameManager: clear, %d entities removed\n", count );
	_is_clearing = false;
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

void GameManager::call_update( float dt )
{
	size_t size = awaiting_queue.size();

	//  update awaiting queue
	if ( size > 0 )
	{
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
		for ( Entity* ent : deletion_queue )
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
}

void GameManager::call_render()
{
	for ( const std::shared_ptr<Entity>& ent : entities )
		ent->render();
}

void GameManager::call_render_hud()
{
	for ( const std::shared_ptr<Entity>& ent : entities )
		ent->render_hud();
}