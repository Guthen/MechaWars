#include "explosion.h"
#include "explosion_manager.h"

//  rule: EXPANSION_TIME < UNRESERVE_TIME < LIFE_TIME
const float Explosion::LIFE_TIME = .7f;  //  how much time the explosion stays on screen
const float Explosion::UNRESERVE_TIME = .2f; //  how much time to wait before unreserving the position
const float Explosion::EXPANSION_TIME = .05f;  //  how much time to wait before the explosion expand

Explosion::Explosion( std::weak_ptr<Map> map, int damage, int expansion ) :
	map( map ), damage( damage ),
	expansion( expansion ), expansion_time( EXPANSION_TIME ), life_time( LIFE_TIME )
{
	//rotation = GetRandomValue( 1, 4 ) * 90.0f;
	texture = AssetsManager::get_or_load_texture( "assets/textures/effects/explosion.png" );

	//  animator
	int i = 0;
	for ( const Rectangle& quad : AssetsManager::slice_texture( texture, 8, 8 ) )
	{
		animator.add_frame( quad );

		//  duplicate the 3rd frame (holds the frame longer)
		if ( i == 2 ||  i == 3 )
			animator.add_frame( quad );

		i++;
	}
	animator.set_fps_to_time( life_time );
	animator.set_loop( false );
	quad = animator.get_current_frame();
}

Explosion::~Explosion()
{
	//  unreserve position & allow other explosion to take place
	if ( !has_unreserved )
		ExplosionManager::unreserve_pos( pos );
}

void Explosion::update( const float dt )
{
	//  animator
	if ( animator.update( dt ) )
		quad = animator.get_current_frame();

	//  deal damage
	if ( !damage_dealt )
	{
		if ( auto map_tmp = map.lock() )
			if ( map_tmp->has_structure_at( pos.x, pos.y ) )
			{
				auto ent = map_tmp->get_structure_at_pos( pos.x, pos.y );
				if ( auto ent_tmp = ent.lock() )
					ent_tmp->take_damage( damage );
			}
		damage_dealt = true;
	}

	//  expansion
	if ( !expansion_done && expansion > 0 && ( expansion_time -= dt ) <= 0.0f )
	{
		//  expand
		if ( auto map_tmp = map.lock() )
		{
			//  vertical & horizontal
			for ( const Cardinal dir : { Cardinal::SOUTH, Cardinal::WEST, Cardinal::NORTH, Cardinal::EAST } )
			{
				Int2 cell = pos + utility::get_cardinal_offset( dir );
				ExplosionManager::create_explosion( map_tmp, cell, damage, expansion - 1, false );
			}

			//  diagonals
			if ( expansion >= 2 )
				for ( Int2 dir : { Int2 { -1, -1 }, Int2 { 1, -1 }, Int2 { -1, 1 }, Int2 { 1, 1 } } )
					ExplosionManager::create_explosion( map_tmp, pos + dir, damage, expansion - 2, false );
		}
		expansion_done = true;
	}

	//  life time
	life_time -= dt;
	if ( !has_unreserved && life_time <= LIFE_TIME - UNRESERVE_TIME )
	{
		ExplosionManager::unreserve_pos( pos );
		has_unreserved = true;
	}
	if ( life_time <= 0.0f )
		safe_destroy();
}

void Explosion::render()
{
	//  draw blasts effects
	if ( is_epicenter )
	{
		//  alpha
		Color color = ColorAlpha( ORANGE, .25f );
		float max_time = LIFE_TIME - UNRESERVE_TIME;
		if ( life_time <= max_time )
			color.a = (unsigned char) ( utility::ease_in_cubic( life_time / ( max_time - UNRESERVE_TIME ) ) * 255.0f );

		//  blasts
		_anim_expansion = 1.0f - utility::ease_in_expo( life_time / max_time ) * expansion + 1;
		DrawCircle( (int) dest.x, (int) dest.y, _anim_expansion * Map::TILE_SIZE, color );
		DrawCircleLines( (int) dest.x, (int) dest.y, ( _anim_expansion + 2 ) * Map::TILE_SIZE, color );
	}

	//  draw animated sprite
	DrawTexturePro( texture, quad, dest, Vector2 { (float) Map::TILE_SIZE / 2, (float) Map::TILE_SIZE / 2 }, rotation, WHITE );
}