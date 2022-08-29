#include "defs.h"

using namespace luacpp;

std::unordered_map<std::string, StructureDef> Defs::structures;

void Defs::bind_lua( LuaState* L )
{
	L->CreateFunction( []( const char* id, const LuaObject& obj ) -> void {
		StructData data {};
		
		//  read Lua values
		LuaTable& tbl = (LuaTable&) obj;
		auto health = tbl.Get( "health" );
		auto work_to_make = tbl.Get( "work_to_make" );
		auto size = tbl.Get( "size" );
		auto texture_path = tbl.Get( "texture_path" );
		auto quad = tbl.Get( "quad" );
		auto team_quad = tbl.Get( "team_quad" );

		//  convert to cpp
		if ( LuaEngine::is_number( health ) )
			data.health = health.ToInteger();
		if ( LuaEngine::is_number( work_to_make ) )
			data.work_to_make = work_to_make.ToInteger();

		if ( LuaEngine::is_table( size ) )
			data.size = LuaEngine::to_int2( size );

		if ( LuaEngine::is_string( texture_path ) )
			data.texture_path = texture_path.ToString();
		if ( LuaEngine::is_table( quad ) )
			data.quad = LuaEngine::to_rectangle( quad );
		if ( LuaEngine::is_table( team_quad ) )
			data.team_quad = LuaEngine::to_rectangle( team_quad );

		//  define
		define_structure( id, StructureDef { data } );
		printf( "Defs::define_structure: structure \"%s\" has been defined!\n", id );
	}, "define_structure" );
}