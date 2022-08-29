#include "lua_engine.h"

LuaState LuaEngine::L( luaL_newstate(), true );

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