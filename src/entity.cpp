#include "entity.h"
#include "game_manager.h"

unsigned int Entity::global_id = 0;

Entity::Entity() : pos( { 0, 0 } ), size( { 1, 1 } )
{
	id = global_id++;
	_update_dest_rect();
}

Entity::~Entity() {}

bool Entity::operator==( Entity& a ) { return a.id == id; }

void Entity::safe_destroy()
{
	GameManager::queue_entity_to_deletion( this );
}