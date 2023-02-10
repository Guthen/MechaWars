#include "missile.h"

Missile::Missile( std::weak_ptr<Map> map, Int2 pos, Int2 target, int damage, int explosion_radius )
	: map( map ),
	damage( damage ), explosion_radius( explosion_radius )
{
	dest.x = ( (float) pos.x + 0.5f ) * Map::TILE_SIZE, dest.y = (float) pos.y * Map::TILE_SIZE;
	dest.width = 8.0f, dest.height = 16.0f;

	target_pos = ( target * Map::TILE_SIZE ).to_v2();
	
	texture = AssetsManager::get_or_load_texture( "assets/textures/effects/missile.png" );
	quad = Rectangle { 0, 0, 8, 16 };
	team_quad = Rectangle { 8, 0, 8, 16 };

	//  setup flames animation
	animator.add_frame( Rectangle { 0, 16, 8, 16 } );
	animator.add_frame( Rectangle { 8, 16, 8, 16 } );
	animator.add_frame( Rectangle { 16, 16, 8, 16 } );
	animator.add_frame( Rectangle { 24, 16, 8, 16 } );
}

void Missile::update( float dt )
{
	//  animator
	if ( animator.update( dt ) )
		flame_quad = animator.get_current_frame();

	//  moving
	float y_dir = is_falling ? 1.0f : -1.0f;
	dest.y += y_dir * current_move_speed * dt;

	if ( !is_falling )
	{
		//  speed update
		current_move_speed = utility::lerp( current_move_speed, move_speed, dt * 0.5f );

		if ( dest.y <= -Map::TILE_SIZE - move_speed * 2.0f )
		{
			is_falling = true;
			is_drawing_flames = false;

			dest.x = target_pos.x;
			dest.y = -dest.height;

			angle = 180.0f;
		}
	}
	else
	{
		current_move_speed = std::min( move_speed * 2.0f, current_move_speed + current_move_speed * 2.0f * dt );

		if ( dest.y >= target_pos.y )
		{
			impact();
			safe_destroy();
		}
	}
}

void Missile::render()
{
	Vector2 origin { dest.width / 2.0f, dest.height / 2.0f };

	//  draw missile
	DrawTexturePro( texture, quad, dest, origin, angle, WHITE );
	
	//  draw team color
	if ( auto owner_tmp = owner.lock() )
	{
		Color team_color = get_team_color( owner_tmp->get_team() );
		DrawTexturePro( texture, team_quad, dest, origin, angle, team_color );
	}

	//  draw flames
	if ( is_drawing_flames )
	{
		DrawTexturePro( texture, flame_quad, dest, origin, angle, WHITE );
	}
}

void Missile::impact()
{
	auto map_tmp = map.lock();
	if ( !map_tmp )
		return;

	//  transform pos to grid
	Int2 dest_pos {
		(int) ( dest.x / Map::TILE_SIZE ),
		(int) ( dest.y / Map::TILE_SIZE )
	};

	ExplosionManager::create_explosion( map_tmp, dest_pos, damage, explosion_radius, owner, true );
}