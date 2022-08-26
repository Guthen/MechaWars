#pragma once

#include "structure.h"

class StructureBlueprint : public Structure
{
protected:
	int work_to_make = 0;
	int health_per_work = 0;
public:
	StructureBlueprint( int x, int y, std::weak_ptr<Map> map ) : Structure( x, y, map )
	{
		work_to_make = 100;
		max_health = 1000;
		health_per_work = max_health / work_to_make;
	}

	void init() override 
	{
		health = 0;
	}

	void debug_update( float dt ) override
	{
		//  base
		Structure::debug_update( dt );

		DRAW_DEBUG( TextFormat( "work_to_make: %d", work_to_make ) );
		DRAW_DEBUG( TextFormat( "health_per_work: %d", health_per_work ) );
	}

	void advance_work()
	{
		if ( work_to_make-- <= 0 )
			finish_work();
		else
			health += health_per_work;
	}

	void finish_work()
	{
		//  TODO: create implemented structure
		//  ..

		//  destroy blueprint
		safe_destroy();
	}

	void render() override
	{
		DrawRectangleRec( dest, color );
	}
};