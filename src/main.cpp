#include <iostream>
#include <chrono>

#include "raylib.h"

#include "utility/draw.h"

#include "assets_manager.h"
#include "game_manager.h"
#include "explosion_manager.h"
#include "game/game_camera.h"
#include "map.h"
#include "game/ui/ui_tile_cursor.h"
#include "game/game_scene.h"

#include "globals.h"

void load()
{
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
	GameManager::call_update( dt );
}

void render()
{
	//  render entities
	GameManager::call_render();
}

void render_hud()
{
	//  render entities
	GameManager::call_render_hud();
	
	//  debug show grid
	/*Int2 size = GameMap->get_size();
	for ( int y = 0; y < size.y; y++ )
		for ( int x = 0; x < size.x; x++ )
			DrawRectangleLines( x * Map::TILE_SIZE, y * Map::TILE_SIZE, Map::TILE_SIZE, Map::TILE_SIZE, WHITE );*/

	//  debug
	utility::draw_shadow_text( TextFormat( "FPS: %d", GetFPS() ), 20, 20, 12, RAYWHITE );
	utility::draw_shadow_text( TextFormat( "EC: %d", GameManager::get_entities().size() ), 20, 40, 12, RAYWHITE );
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