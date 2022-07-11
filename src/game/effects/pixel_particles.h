#pragma once

#include "../../entity.h"
#include "../../utility/color.h"

#include <vector>

class PixelParticles : public Entity
{
public:
	struct Particle
	{
		Particle( Int2 pos, float lifetime ) : pos( pos ), lifetime( lifetime ), max_lifetime( lifetime ) {};

		Int2 pos;
		float lifetime;
		float max_lifetime;
	};
private:
	std::vector<Particle> particles;
	Color start_color;
	Color end_color;

	bool auto_destroy = false;
public:
	PixelParticles( Color color ) : start_color( color ), end_color( color ) {}
	PixelParticles( Color start_color, Color end_color ) : start_color( start_color ), end_color( end_color ) {};

	void update( float dt ) override
	{
		if ( particles.size() == 0 )
		{
			if ( auto_destroy )
				safe_destroy();
			return;
		}

		for ( auto it = particles.begin(); it != particles.end(); it )
		{
			Particle* particle = &*it;

			//  remove on lifetime expiry
			if ( ( particle->lifetime -= dt ) <= 0.0f )
				it = particles.erase( it );
			else
				it++;
		}
	}

	void render() override
	{
		for ( auto it = particles.begin(); it != particles.end(); it++ )
		{
			Particle* particle = &*it;

			Color color = utility::lerp_color( end_color, start_color, particle->lifetime / particle->max_lifetime );
			DrawRectangle( particle->pos.x, particle->pos.y, 1, 1, color );
		}
	}

	void emit( Particle particle ) { particles.push_back( particle ); }

	void set_auto_destroy( bool value ) { auto_destroy = value; }
};