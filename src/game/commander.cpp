#include "commander.h"

#include "fog_of_war.h"

#include <src/game_manager.h>

Commander* Commander::PLAYER_COMMANDER { nullptr };
std::vector<Commander*> Commander::ALL;

Commander::Commander( std::weak_ptr<Map> map, TEAM team_id )
	: map( map ), team_id( team_id )
{
	//  init fog-of-war
	if ( auto map_tmp = map.lock() )
		fog_of_war = GameManager::create<FogOfWar>( this, map_tmp );
	else
		printf( "Commander::Commander: failed to lock map for FogOfWar creation" );

	//  register
	ALL.push_back( this );
}

Commander::~Commander()
{
	//  release fog of war
	//if ( auto fow_tmp = fog_of_war.lock() )
	//	fow_tmp->safe_destroy();

	//  remove from list
	/*auto itr = std::find( ALL.begin(), ALL.end(), this );
	if ( itr != ALL.end() )
		ALL.erase( itr );*/
}

void Commander::clear()
{
	for ( Commander* commander : ALL )
		delete commander;

	ALL.clear();
}

Commander* Commander::get_team_commander( TEAM team )
{
	int id = (int) team - 1;
	if ( id < 0 || id >= ALL.size() ) 
		return nullptr;

	return ALL[id];
}

void Commander::register_entity( std::weak_ptr<WorldEntity> ent )
{
	owned_entities.push_back( ent );
}

void Commander::unregister_entity( std::weak_ptr<WorldEntity> ent )
{
	auto ent_tmp = ent.lock();
	for ( auto itr = owned_entities.begin(); itr != owned_entities.end(); itr++ )
		if ( ent_tmp == itr->lock() )
		{
			owned_entities.erase( itr );
			break;
		}
}