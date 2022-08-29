#include <iostream>
#include <chrono>

#include <luacpp/luacpp.h>

#include "raylib.h"

#include "utility/draw.h"

#include "assets_manager.h"
#include "game_manager.h"
#include "game/game_camera.h"
#include "game/game_scene.h"

#include "game/defs.h"

#include "globals.h"

void load()
{
	LuaState* L = LuaEngine::get_state();

	//  bindings
	Defs::bind_lua( L );

	//  requires
	printf( "\n" );
	LuaEngine::require_folder( "lua/structures" );
	printf( "\n" );

	//  define game data
	/*StructData data {};
	data.health = 5000;
	data.work_to_make = 10;
	data.size.x = 2, data.size.y = 2;
	data.texture_path = "assets/textures/buildings/nexus.png";
	data.quad = Rectangle { 0, 0, 16, 16 };
	data.team_quad = Rectangle { 16, 0, 16, 16 };
	Defs::define_structure( "nexus", StructureDef { data } );*/

	//  load game scene
	GameManager::change_scene<GameScene>();
}

void unload()
{
	GameManager::free();
	AssetsManager::free();
}

void update( float dt )
{
	//  update entities
	GameManager::update( dt );
}

void render()
{
	//  render entities
	GameManager::render();
}

void render_hud()
{
	//  render entities
	GameManager::render_hud();
}

int main()
{
	//SetTraceLogLevel( LOG_NONE ); //  just hide the raylib logs to see mine

	//  init
	InitWindow( SCREEN_WIDTH, SCREEN_HEIGHT, GAME_TITLE.c_str() );
	SetTargetFPS( 60 );

	load();

	//  game-loop
	while ( !WindowShouldClose() )
	{
		update( GetFrameTime() );
		
		GameCamera* camera = GameCamera::get_current();
		#pragma region Rendering
			BeginDrawing();
				ClearBackground( BACKGROUND_COLOR );

				//  draw world
				camera->push();
					render();
				camera->pop();

				//  draw hud
				render_hud();
			EndDrawing();
		#pragma endregion
	}

	//  de-init
	unload();
	CloseWindow();

	return 0;
}