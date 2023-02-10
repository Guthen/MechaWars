#include "missile.h"

Missile::Missile( std::weak_ptr<Map> map, Int2 pos, Int2 target, int damage, int explosion_radius )
	: map( map ),
	damage( damage ), explosion_radius( explosion_radius )
{
	this->pos = pos * Map::TILE_SIZE + Int2 { Map::TILE_SIZE / 2, Map::TILE_SIZE };
	dest.x = this->pos.x, dest.y = this->pos.y;
	dest.width = 8.0f, dest.height = 16.0f;

	target_pos = ( target * Map::TILE_SIZE ).to_v2();
	
	texture = AssetsManager::get_or_load_texture( "assets/textures/effects/missile.png" );
	quad = Rectangle { 0, 0, 8, 16 };
	team_quad = Rectangle { 8, 0, 8, 16 };

	//  setup flames animation
	animator.set_fps( 12.0f );
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

	life_time = life_time + dt;

	if ( !is_falling )
	{
		//  speed update
		current_move_speed = utility::lerp( current_move_speed, move_speed, dt * LAUNCH_GAIN_SPEED );

		//  shake update
		float shake_time = life_time <= MAX_SHAKE_TIME ? MAX_SHAKE_TIME : life_time;
		float shake_scale = utility::ease_in_cubic( shake_time / MAX_SHAKE_TIME );

		float game_time = GameManager::get_time();
		dest.x = pos.x 
			   + sinf( life_time + shake_scale * HEAVY_SHAKE_FREQUENCY ) * 0.25f
			   + cosf( life_time - 0.5f + shake_scale * LOW_SHAKE_FREQUENCY ) * 1.0f;

		if ( dest.y <= -Map::TILE_SIZE - move_speed * TIME_TO_START_FALLING )
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
		current_move_speed = std::min( fall_move_speed, current_move_speed + current_move_speed * FALL_GAIN_SPEED * dt );

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

	GameCamera* camera = GameCamera::get_current();
	Rectangle clipping = camera->world_to_viewport( world_clipping );

	//  draw missile
	//DrawRectangleLinesEx( world_clipping, 1.0f, RED );
	bool should_clip = !is_falling && clipping.width > 0.0f;
	if ( should_clip )
		BeginScissorMode( clipping.x, clipping.y, clipping.width, clipping.height );

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

	if ( should_clip )
		EndScissorMode();
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