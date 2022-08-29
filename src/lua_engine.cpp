#include "lua_engine.h"

LuaState LuaEngine::L( luaL_newstate(), true );

void LuaEngine::require( std::string file_path )
{
	const char* path = file_path.c_str();
	printf( "LUA: require \"%s\"\n", path );

	std::string err;
	if ( !L.DoFile( path, &err ) )
		printf( "    ERROR: \"%s\"\n", err.c_str() );
}

void LuaEngine::require_folder( std::string folder_path )
{
	for ( const auto& entry : std::filesystem::directory_iterator( folder_path ) )
	{
		std::string file_path = entry.path().string();
		if ( file_path.rfind( ".lua" ) == std::string::npos )  //  only require '.lua' files
			continue;
		
		require( file_path );
	}
}

Rectangle LuaEngine::to_rectangle( const LuaObject& obj )
{
	LuaTable tbl = (LuaTable) obj;
	return Rectangle {
		(float) tbl.GetNumber( 1 ),
		(float) tbl.GetNumber( 2 ),
		(float) tbl.GetNumber( 3 ),
		(float) tbl.GetNumber( 4 )
	};
}

Int2 LuaEngine::to_int2( const LuaObject& obj )
{
	LuaTable tbl = (LuaTable) obj;
	return Int2 {
		(int) tbl.GetInteger( 1 ),
		(int) tbl.GetInteger( 2 ),
	};
}