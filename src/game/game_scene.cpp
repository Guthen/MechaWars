#include "game_scene.h"

#include "pathfinder.h"
#include "commander.h"

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
	camera.move_to( 0.0f, 0.0f );

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