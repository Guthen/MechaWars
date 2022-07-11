#include "bullet.h"

Bullet::Bullet( std::weak_ptr<Map> map, Vector2 pos, Vector2 dir, float dist ) : map( map ), dist_to_move( dist )
{
	dest.x = (float) pos.x, dest.y = (float) pos.y;
	dest.width = (float) size.x * Map::TILE_SIZE, dest.height = (float) size.y * Map::TILE_SIZE;

	set_move_dir( dir );

	particles = GameManager::create<PixelParticles>( Color { 245, 255, 232, 178 }, Color { 245, 255, 232, 0 } );

	texture = AssetsManager::get_or_load_texture( "assets/textures/effects/bullet.png" );
	quad = Rectangle { 0, 0, (float) texture.width, (float) texture.height };
}

void Bullet::update( float dt )
{
	//  movement
	dest.x += move_dir.x * move_speed * dt;
	dest.y += move_dir.y * move_speed * dt;

	//  particles
	if ( auto particles_tmp = particles.lock() )
	{
		for ( int y = 0; y < 2; y++ )
			for ( int x = 0; x < 3; x++ )
			{
				Int2 particle_pos { (int) dest.x + x, (int) dest.y + y };

				particles_tmp->emit( PixelParticles::Particle( particle_pos, 1.0f ) );
			}
	}
	
	//  distance
	dist_to_move -= move_speed * dt;
	if ( dist_to_move <= 0.0f )
	{
		impact();
		safe_destroy();
	}
}

void Bullet::render()
{
	DrawTexturePro( texture, quad, dest, Vector2 { 4, 4 }, angle, WHITE );

	//  debug: destination
	Int2 dest_pos { 
		(int) ( dest.x + move_dir.x * dist_to_move ),
		(int) ( dest.y + move_dir.y * dist_to_move )
	};
	DrawCircleLines( dest_pos.x, dest_pos.y, 2.0f, RED );
}

void Bullet::impact()
{
	if ( auto map_tmp = map.lock() )
	{
		Int2 dest_pos {
			(int) ( dest.x / Map::TILE_SIZE ),
			(int) ( dest.y / Map::TILE_SIZE )
		};

		ExplosionManager::create_explosion( map_tmp, dest_pos, 5.0f, 2 );
	}
}

void Bullet::safe_destroy()
{
	Entity::safe_destroy();
	
	if ( auto particles_tmp = particles.lock() )
		particles_tmp->set_auto_destroy( true );
}

void Bullet::set_move_dir( Vector2 dir )
{
	move_dir = dir;
	angle = utility::get_direction_angle( dir );
}