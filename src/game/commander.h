#pragma once
#include "commander.fwd.h"
#include "fog_of_war.fwd.h"

#include "map.h"
#include "teams.h"

using world_entity_vector = std::vector<std::weak_ptr<WorldEntity>>;

class Commander
{
private:
	TEAM team_id;
	std::weak_ptr<FogOfWar> fog_of_war;

	std::weak_ptr<Map> map;
	world_entity_vector owned_entities;
public:
	Int2 start_pos { 0, 0 };

	static Commander* PLAYER_COMMANDER;
	static std::vector<Commander*> ALL;

	Commander( std::weak_ptr<Map> map, TEAM team );
	~Commander();

	static Commander* get_team_commander( TEAM team );
	static void clear();

	void register_entity( std::weak_ptr<WorldEntity> ent );
	void unregister_entity( std::weak_ptr<WorldEntity> ent );

	world_entity_vector* get_owned_entities() { return &owned_entities; }
	std::weak_ptr<FogOfWar> get_fog_of_war() { return fog_of_war; }
};

