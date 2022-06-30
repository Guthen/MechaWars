#include "bullet.h"

Bullet::Bullet( Vector2 pos, Vector2 dir, float dist )
{
	render_pos = pos;
	set_move_dir( dir );
	dist_to_move = dist;

	texture = AssetsManager::get_or_load_texture( "assets/textures/effects/bullet.png" );
	quad = Rectangle { 0, 0, (float) texture.width, (float) texture.height };
}

void Bullet::update( float dt )
{
	//  movement
	render_pos.x += move_dir.x * move_speed * dt;
	render_pos.y += move_dir.y * move_speed * dt;
	_update_dest_rect();
	
	//  distance
	dist_to_move -= move_speed * dt;
	if ( dist_to_move <= 0.0f )
	{
		printf( "bullet damage!\n" );
		safe_destroy();
	}
}

void Bullet::render()
{
	DrawTexturePro( texture, quad, dest, Vector2 { 4, 4 }, angle, WHITE );

	//  debug: destination
	Int2 dest_pos { 
		(int) ( render_pos.x + move_dir.x * dist_to_move ),
		(int) ( render_pos.y + move_dir.y * dist_to_move )
	};
	DrawCircleLines( dest_pos.x, dest_pos.y, 2.0f, RED );
}

void Bullet::set_move_dir( Vector2 dir )
{
	move_dir = dir;
	angle = utility::get_direction_angle( dir );
}