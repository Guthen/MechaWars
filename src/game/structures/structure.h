#pragma once
#include "../world_entity.h"

#include <src/game_manager.h>

struct StructureData
{
	int health = 100;
	int work_to_make = 10;  //  how much times should the builder hit the building to finish it? 

	Int2 size { 1, 1 };  //  structure size in tiles

	std::string texture_path = "assets/textures/error.png";  //  path to the texture
	Rectangle quad { 0, 0, Map::TILE_SIZE, Map::TILE_SIZE };  //  region of the texture to draw
	Rectangle team_quad { 0, 0, 0, 0 };  //  region of the texture to draw for the team indicator coloring
};

struct StructureDef
{
	StructureDef() {}
	StructureDef( StructureData data ) : data( data ) {}
	StructureData data;
};

class Structure : public WorldEntity
{
protected:
	StructureData data;
public:
	Structure::Structure( const int x, const int y, StructureData data, std::weak_ptr<Map> map ) : WorldEntity( x, y, data.size.x, data.size.y, map ), data( data ) {};
	Structure( const int x, const int y, const int w, const int h, StructureData data, std::weak_ptr<Map> map ) : WorldEntity( x, y, w, h, map ), data( data ) {};
	virtual ~Structure() {};

	void init() override;
	void debug_update( float dt ) override;

	StructureData get_data() { return data; }
};