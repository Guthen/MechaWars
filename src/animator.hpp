#pragma once

#include <raylib.h>
#include <vector>

class Animator
{
private:
	std::vector<Rectangle> frames;
	int fps = 6;

	int current_frame_id = 0;
	float next_frame_time = 0.0f;
public:
	Animator( const int fps = 8 ) : fps( fps ) 
	{
		next_frame_time = 1.0f / fps;
	}

	void update( float dt )
	{
		next_frame_time -= dt;
		if ( next_frame_time <= 0.0f )
		{
			next_frame_time += 1.0f / fps;
			current_frame_id = ( current_frame_id + 1 ) % frames.size();
			//printf( "next frame! %d/%d\n", current_frame_id, frames.size() );
		}
	}

	void add_frame( Rectangle quad ) { frames.push_back( quad ); }
	Rectangle get_current_frame() { return frames[current_frame_id]; }
};