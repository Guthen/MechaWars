#pragma once

#include <raylib.h>
#include <vector>

class Animator
{
private:
	std::vector<Rectangle> frames;
	float fps;

	bool looping = true;
	bool playing = true;

	int current_frame_id = 0;
	float next_frame_time = 0.0f;
public:
	Animator( const float fps = 8.0f ) : fps( fps ) 
	{
		next_frame_time = 1.0f / fps;
	}

	bool update( float dt )
	{
		if ( !playing )
			return false;

		if ( ( next_frame_time -= dt ) <= 0.0f )
		{
			next_frame_time += 1.0f / fps;

			//  next frame
			int next_frame_id = ( current_frame_id + 1 ) % (int) frames.size();
			if ( next_frame_id == 0 && !looping )
				playing = false;
			else
				current_frame_id = next_frame_id;

			return true;
		}

		return false;
	}

	void set_playing( bool play ) { playing = play; }
	bool is_playing() { return playing; }

	void set_loop( bool loop ) { looping = loop; }
	bool is_looping() { return looping; }

	void set_fps( float _fps ) { fps = _fps; }
	void set_fps_to_time( float time ) { set_fps( ( frames.size() / time ) ); }
	float get_fps() { return fps; }

	void add_frame( Rectangle quad ) { frames.push_back( quad ); }
	Rectangle get_current_frame() { return frames[current_frame_id]; }
	size_t get_frame_count() { return frames.size(); }
};