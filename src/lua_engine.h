#pragma once
#include <luacpp/luacpp.h>
using namespace luacpp;

#include <filesystem>

#include <raylib.h>
#include <src/utility/int2.h>

class LuaEngine
{
private:
	static LuaState L;
public:
	static void require( std::string file_path );
	static void require_folder( std::string folder_path );

	template <typename... Args>
	static void error( std::string msg, Args... args )
	{
		msg = TextFormat( msg.c_str(), args... );
		printf( TextFormat( "\x1B[91mERROR: %s: %s\033[0m\n", "LuaEngine", msg.c_str() ) );
	}

	static bool is_nil( const LuaObject& obj ) { return obj.GetType() == LUA_TNIL; }
	static bool is_string( const LuaObject& obj ) { return obj.GetType() == LUA_TSTRING; }
	static bool is_bool( const LuaObject& obj ) { return obj.GetType() == LUA_TBOOLEAN; }
	static bool is_number( const LuaObject& obj ) { return obj.GetType() == LUA_TNUMBER; }
	static bool is_table( const LuaObject& obj ) { return obj.GetType() == LUA_TTABLE; }

	static Rectangle to_rectangle( const LuaObject& obj );
	static Int2 to_int2( const LuaObject& obj );

	static LuaState* get_state() { return &L; }
};