#include "defs.h"

#include <vector>

std::unordered_map<std::string, StructureDef> Defs::structures;
std::unordered_map<std::string, UnitDef> Defs::units;

void Defs::bind_lua( LuaState* L )
{
	//  define_structure
	L->CreateFunction( []( const char* id, const LuaObject& obj ) -> void {
		StructureData data {};
		
		//  convert to cpp
		LuaTable& tbl = (LuaTable&) obj;

		//  stats
		auto health = tbl.Get( "health" );
		if ( LuaEngine::is_number( health ) )
			data.health = (int) health.ToInteger();
		auto work_to_make = tbl.Get( "work_to_make" );
		if ( LuaEngine::is_number( work_to_make ) )
			data.work_to_make = (int) work_to_make.ToInteger();

		auto size = tbl.Get( "size" );
		if ( LuaEngine::is_table( size ) )
			data.size = LuaEngine::to_int2( size );

		//  renderer
		auto texture_path = tbl.Get( "texture_path" );
		if ( LuaEngine::is_string( texture_path ) )
			data.texture_path = texture_path.ToString();
		auto quad = tbl.Get( "quad" );
		if ( LuaEngine::is_table( quad ) )
			data.quad = LuaEngine::to_rectangle( quad );
		auto team_quad = tbl.Get( "team_quad" );
		if ( LuaEngine::is_table( team_quad ) )
			data.team_quad = LuaEngine::to_rectangle( team_quad );

		//  define
		define_structure( id, StructureDef { data } );
		printf( "Defs::define_structure: structure \"%s\" has been defined!\n", id );
	}, "define_structure" );

	//  define_unit
	L->CreateFunction( []( const char* id, const LuaObject& obj ) -> void {
		UnitData data {};

		//  convert to cpp
		LuaTable& tbl = (LuaTable&) obj;

		//  stats
		auto health = tbl.Get( "health" );
		if ( LuaEngine::is_number( health ) )
			data.health = (int) health.ToInteger();
		auto can_attack = tbl.Get( "can_attack" );
		if ( LuaEngine::is_bool( can_attack ) )
			data.can_attack = can_attack.ToBool();
		auto move_speed = tbl.Get( "move_speed" );
		if ( LuaEngine::is_number( move_speed ) )
			data.move_speed = (float) move_speed.ToNumber();

		//  renderer
		auto texture_path = tbl.Get( "texture_path" );
		if ( LuaEngine::is_string( texture_path ) )
			data.texture_path = texture_path.ToString();
		auto anim_quads = tbl.Get( "anim_quads" );
		if ( LuaEngine::is_table( anim_quads ) )
		{
			data.anim_quads.clear();

			LuaTable& tbl = (LuaTable&) anim_quads;
			tbl.ForEach( [&data]( const LuaObject& key, const LuaObject& value ) -> bool {
				if ( LuaEngine::is_table( value ) )
					data.anim_quads.push_back( LuaEngine::to_rectangle( value ) );
				else
					LuaEngine::error( "define_unit: 'anim_quads[%s]' is not a table", key.ToString() );

				return true;
			} );
		}
		auto team_quad = tbl.Get( "team_quad" );
		if ( LuaEngine::is_table( team_quad ) )
			data.team_quad = LuaEngine::to_rectangle( team_quad );

		//  build
		auto can_build = tbl.Get( "can_build" );
		if ( LuaEngine::is_bool( can_build ) )
			data.can_build = can_build.ToBool();
		auto work_time = tbl.Get( "work_time" );
		if ( LuaEngine::is_number( work_time ) )
			data.work_time = (float) work_time.ToNumber();

		//  shoot
		auto shoot = tbl.Get( "shoot" );
		if ( LuaEngine::is_table( shoot ) )
		{
			LuaTable& tbl = (LuaTable&) shoot;

			auto should_predict_movement = tbl.Get( "should_predict_movement" );
			if ( LuaEngine::is_bool( should_predict_movement ) )
				data.shoot.should_predict_movement = should_predict_movement.ToBool();
			auto attack_range = tbl.Get( "attack_range" );
			if ( LuaEngine::is_number( attack_range ) )
				data.shoot.attack_range = (float) attack_range.ToNumber();
			auto setup_delay = tbl.Get( "setup_delay" );
			if ( LuaEngine::is_number( setup_delay ) )
				data.shoot.setup_delay = (float) setup_delay.ToNumber();
			auto fire_delay = tbl.Get( "fire_delay" );
			if ( LuaEngine::is_number( fire_delay ) )
				data.shoot.fire_delay = (float) fire_delay.ToNumber();
			auto spread = tbl.Get( "spread" );
			if ( LuaEngine::is_number( spread ) )
				data.shoot.spread = (int) spread.ToInteger();
			auto accuracy = tbl.Get( "accuracy" );
			if ( LuaEngine::is_number( accuracy ) )
				data.shoot.accuracy = (float) accuracy.ToNumber();
			auto damage = tbl.Get( "damage" );
			if ( LuaEngine::is_number( damage ) )
				data.shoot.damage = (int) damage.ToInteger();
			auto bullet_speed = tbl.Get( "bullet_speed" );
			if ( LuaEngine::is_number( bullet_speed ) )
				data.shoot.bullet_speed = (float) bullet_speed.ToNumber();
			auto burst_count = tbl.Get( "burst_count" );
			if ( LuaEngine::is_number( burst_count ) )
				data.shoot.burst_count = (int) burst_count.ToInteger();
			auto burst_delay = tbl.Get( "burst_delay" );
			if ( LuaEngine::is_number( burst_delay ) )
				data.shoot.burst_delay = (float) burst_delay.ToNumber();
			auto explosion_radius = tbl.Get( "explosion_radius" );
			if ( LuaEngine::is_number( explosion_radius ) )
				data.shoot.explosion_radius = (int) explosion_radius.ToInteger();
		}

		define_unit( id, UnitDef { data } );
		printf( "Defs::define_unit: unit \"%s\" has been defined!\n", id );
	}, "define_unit" );
}