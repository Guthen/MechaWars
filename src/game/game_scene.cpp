#include "game_scene.h"

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

	tile_cursor = GameManager::create<UITileCursor>( map );
}

void GameScene::update( float dt )
{
	camera.update( dt );

	if ( IsKeyPressed( KEY_R ) )
		DEFERED(
			auto scene = GameManager::create<GameScene>( (unsigned int) time( NULL ) );
			GameManager::change_scene_to( scene );
		);
}

void GameScene::render_hud()
{
	if ( auto map_tmp = map.lock() )
		utility::draw_shadow_text( TextFormat( "SEED: %d", map_tmp->get_seed() ), 20, 60, 12, RAYWHITE );

	utility::draw_shadow_text( TextFormat( "ZOOM: %f%%", camera.get_zoom() * 100.0f ), 20, 80, 12, RAYWHITE );

	/*camera.push();
	Int2 pos = tile_cursor->get_pos();
	Int2 render_pos = Int2 { pos } *Map::TILE_SIZE;
	utility::draw_shadow_text( TextFormat( "Tile: %d", map->get_tile_at_pos( pos.x, pos.y ) ), render_pos.x, render_pos.y - 20, 12, RAYWHITE );
	utility::draw_shadow_text( TextFormat( "TileID: %d", map->get_tile_id( pos.x, pos.y ) ), render_pos.x, render_pos.y - 30, 12, RAYWHITE );
	camera.pop();*/
}