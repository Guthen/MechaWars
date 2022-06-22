#pragma once

#include "int2.h"

#include "map.h"
#include "explosion.h"
#include "game_manager.h"

#include <unordered_set>

class ExplosionManager
{
private:
	static std::unordered_set<Int2, Int2> reservations;
public:
	static Explosion* SpawnExplosion( Map* map, const Int2& pos, const float power, const int expansion )
	{
		if ( !map->is_in_bounds( pos.x, pos.y ) )
		{
			printf( "Gen::SpawnExplosion: abort attempt to generate Explosion outbounds of the map!" );
			return NULL;
		}

		if ( !TryReserve( pos ) )
		{
			return NULL;
		}

		Explosion* base_exp = GameManager::create<Explosion>( map, power, expansion );
		base_exp->set_pos( pos );
		return base_exp;
	}

	static bool TryReserve( const Int2& pos )
	{
		if ( !CanReservePos( pos ) )
			return false;

		reservations.insert( pos );
		return true;
	}

	static void UnReservePos( const Int2& pos )
	{
		if ( CanReservePos( pos ) )
			return;

		reservations.erase( pos );
	}

	static bool CanReservePos( const Int2& pos )
	{
		return reservations.find( pos ) == reservations.end();
	}
};

