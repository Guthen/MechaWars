#include "game_scene.h"

void GameScene::init()
{
	//  game init
	//GameManager::add_entity( std::make_shared<EntityMap>( EntityMap( 16, 16 ) ) );
	map = GameManager::create<Map>( 64, 64 );
	map->generate( seed );
	//GameMap->generate( std::chrono::system_clock::now().time_since_epoch().count() );
	//GameMap->fill( 0, 0, GameMap->get_size().x, GameMap->get_size().y, 1 );

	camera.set_offset( SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f );
	camera.set_bounds( 0, 0, map->get_size().x * 8.0f, map->get_size().y * 8.0f );
	//camera.set_is_bounded( true );
	//camera.move_to( camera.get_bounds().width / 2, camera.get_bounds().height / 2 );
	camera.move_to( 0.0f, 0.0f );

	tile_cursor = GameManager::create<UITileCursor>( map );
}

void GameScene::update( float dt )
{
	camera.update( dt );

	if ( IsKeyPressed( KEY_R ) )
	{
		DEFERED(
			GameScene* scene = GameManager::create<GameScene>( (unsigned int) time( NULL ) );
			GameManager::change_scene_to( scene );
		);
	}
	if ( IsKeyPressed( KEY_C ) )
		GameManager::clear();
}

void GameScene::render_hud()
{
	utility::draw_shadow_text( TextFormat( "SEED: %d", map->get_seed() ), 20, 60, 12, RAYWHITE );
	utility::draw_shadow_text( TextFormat( "ZOOM: %f%%", camera.get_zoom() * 100.0f ), 20, 80, 12, RAYWHITE );

	/*camera.push();
	Int2 pos = tile_cursor->get_pos();
	Int2 render_pos = Int2 { pos } *Map::TILE_SIZE;
	utility::draw_shadow_text( TextFormat( "Tile: %d", map->get_tile_at_pos( pos.x, pos.y ) ), render_pos.x, render_pos.y - 20, 12, RAYWHITE );
	utility::draw_shadow_text( TextFormat( "TileID: %d", map->get_tile_id( pos.x, pos.y ) ), render_pos.x, render_pos.y - 30, 12, RAYWHITE );
	camera.pop();*/
}