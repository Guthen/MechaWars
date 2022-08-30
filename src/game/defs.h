#pragma once

#include <unordered_map>

#include <src/lua_engine.h>
#include <src/game/structures/structure.h>

#define __MAP_ACCESSORS( NAME, MAP, TYPE )								  \
	static void define_##NAME( const char* id, TYPE def ) { MAP[id] = def; }  \
	static TYPE& get_##NAME( const char* id ) { return MAP[id]; }

class Defs
{
private:
	static std::unordered_map<std::string, StructureDef> structures;
public:
	static void bind_lua( luacpp::LuaState* L );

	__MAP_ACCESSORS( structure, structures, StructureDef )
};

