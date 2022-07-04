#pragma once

#include "explosion.h"
#include "../../utility/cardinal.h"

#include "../map.h"
#include "../../game_manager.h"

#include <unordered_set>

class ExplosionManager
{
private:
	static std::unordered_set<Int2, Int2> reservations;
public:
	static std::shared_ptr<Explosion> create_explosion( std::shared_ptr<Map> map, const Int2& pos, float power, int expansion, bool is_epicenter = true )
	{
		if ( !map->is_in_bounds( pos.x, pos.y ) )
			return NULL;

		if ( !try_reserve_pos( pos ) )
			return NULL;

		auto explosion = GameManager::create<Explosion>( map, power, expansion );
		explosion->set_pos( pos );
		explosion->set_epicenter( is_epicenter );
		return explosion;
	}

	static bool try_reserve_pos( const Int2& pos )
	{
		if ( has_reservation_at( pos ) )
			return false;

		reserve_pos( pos );
		return true;
	}
	static void reserve_pos( const Int2& pos ) { reservations.insert( pos ); }
	static void unreserve_pos( const Int2& pos )
	{
		if ( !has_reservation_at( pos ) )
			return;

		reservations.erase( pos );
	}
	static bool has_reservation_at( const Int2& pos ) { return !( reservations.find( pos ) == reservations.end() ); }
};

