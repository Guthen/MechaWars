#include "game_scene.h"

#include "pathfinder.h"
#include "commander.h"
#include "fog_of_war.h"

void GameScene::init()
{
	//  init map
	auto map_tmp = GameManager::create<Map>( 64, 64 );
	map_tmp->generate( seed );
	map = map_tmp;  //  store the weak pointer

	//  init camera
	camera.set_offset( SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f );
	camera.set_bounds( 0, 0, map_tmp->get_size().x * 8.0f, map_tmp->get_size().y * 8.0f );
	//camera.set_is_bounded( true );
	//camera.move_to( camera.get_bounds().width / 2, camera.get_bounds().height / 2 );
	Int2 start_pos = Commander::PLAYER_COMMANDER->start_pos * Map::TILE_SIZE;
	camera.move_to( start_pos.x, start_pos.y );

	//  create tile cursor
	tile_cursor = GameManager::create<UITileCursor>( map );
}

GameScene::~GameScene()
{
	Commander::clear();
}

void GameScene::update( float dt )
{
	camera.update( dt );

	if ( IsKeyPressed( KEY_R ) )
		DEFERED(
			auto scene = GameManager::create<GameScene>( (unsigned int) time( NULL ) );
			GameManager::change_scene_to( scene );
		);
	else if ( IsKeyPressed( KEY_COMMA ) )
		GameManager::set_debug_state( (DEBUG_STATE) ( ( (int) GameManager::get_debug_state() + 1 ) % DEBUG_STATE_COUNT ) );
	
	//  debug
	if ( GameManager::get_debug_state() > DEBUG_STATE::NONE )
	{
		if ( IsKeyPressed( KEY_CAPS_LOCK ) )
			GameManager::time_scale = GameManager::time_scale == 1.0f ? 0.2f : 1.0f;
		if ( IsKeyPressed( KEY_F ) )
		{
			Commander* commander = Commander::PLAYER_COMMANDER;
			auto fow_tmp = commander->get_fog_of_war().lock();
			auto map_tmp = map.lock();

			fow_tmp->provide_vision( map_tmp->get_size() / 2, map_tmp->get_size().x );
			fow_tmp->force_update();
		}
	}
}

void GameScene::render_hud()
{
	DEBUG_STATE debug_state = GameManager::get_debug_state();
	if ( debug_state == DEBUG_STATE::NONE )
		return;

	//  debug
	utility::draw_shadow_text( TextFormat( "FPS: %d", GetFPS() ), 20, 20, 12 );
	utility::draw_shadow_text( TextFormat( "DEBUG: %d", debug_state ), 20, 40, 12 );
	utility::draw_shadow_text( TextFormat( "EC: %d", GameManager::get_entities().size() ), 20, 60, 12 );

	if ( auto map_tmp = map.lock() )
		utility::draw_shadow_text( TextFormat( "SEED: %d", map_tmp->get_seed() ), 20, 80, 12 );

	utility::draw_shadow_text( TextFormat( "ZOOM: %f%%", camera.get_zoom() * 100.0f ), 20, 100, 12 );
}