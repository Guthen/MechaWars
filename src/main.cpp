#include <raylib.h>

#include <src/game_manager.h>
#include <src/game/game_scene.h>
#include <src/game/defs.h>
#include <src/globals.h>

void load()
{
	#pragma region LuaModding
	LuaState* L = LuaEngine::get_state();

	//  bindings
	Defs::bind_lua( L );

	//  requires
	printf( "\nLua Modding:\n" );
	LuaEngine::require_folder( "lua/structures" );
	printf( "\n" );
	#pragma endregion

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