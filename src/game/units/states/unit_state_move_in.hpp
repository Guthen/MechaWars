#pragma once

#include "unit_state_move.hpp"

class UnitState_MoveIn : public UnitState_Move
{
private:
	Rectangle area;

	/*
	 *  homemade algorithm to find the nearest position in a rectangle by using its corners and edges:
	 *  it's simple & clean
	 * 
	 *  only problem I found is that it doesn't take in account the map's topography, so resulting paths
	 *  may look stupid. however, I do not plan to fix this, since I don't want this algorithm to be 
	 *	performance-consuming by computing the best path possible
	 */
	Int2 _get_area_nearest_goal()
	{
		Int2 unit_pos = unit->get_pos();

		//  get rect-corners
		int left = area.x, right = area.x + area.width - 1;
		int top = area.y, bottom = area.y + area.height - 1;
		Int2 corners[4] {
			Int2 { left, top },		//  top-left
			Int2 { right, top },	//  top-right
			Int2 { left, bottom },	//  bottom-left
			Int2 { right, bottom }	//  bottom-right
		};

		//  compute all rect-corners to unit distances
		float distances[4] 
		{
			utility::distance_to_sqr( unit_pos, corners[0] ),	//  top-left
			utility::distance_to_sqr( unit_pos, corners[1] ),	//  top-right
			utility::distance_to_sqr( unit_pos, corners[2] ),	//  bottom-left
			utility::distance_to_sqr( unit_pos, corners[3] ),	//  bottom-right
		};

		//  find the nearest edge by finding two nearest rect-corners
		int a_id = 0, b_id = 1;
		for ( int i = 0; i < 4; i++ )
		{
			float dist = distances[i];
			if ( dist < distances[a_id] )
			{
				//  swap IDs
				b_id = a_id;
				a_id = i;
			}
			else if ( !( i == a_id ) && dist < distances[b_id] )
				b_id = i;
		}

		Int2& a = corners[a_id], b = corners[b_id];
		corner_a = a, corner_b = b;  //  keep for debug

		//  get the nearest position on the edge
		if ( ( a.x < unit_pos.x && unit_pos.x < b.x ) || ( b.x < unit_pos.x && unit_pos.x < a.x ) )
			return Int2 { unit_pos.x, a.y };
		else if ( ( a.y < unit_pos.y && unit_pos.y < b.y ) || ( b.y < unit_pos.y && unit_pos.y < a.y ) )
			return Int2 { a.x, unit_pos.y };

		//  get the nearest corner
		return corners[a_id];
	}

	Int2 corner_a, corner_b;
public:
	UnitState_MoveIn( Unit* unit, Rectangle area ) : UnitState_Move( unit ), area( area ) {};

	void init() override
	{
		//  get goal
		goal = _get_area_nearest_goal();
		UnitState_Move::init();

		//  avoid manually changing goal w/ Unit::move_to
		can_change_goal = false;
	}

	void debug_render() override
	{
		utility::draw_debug_rect( corner_a.x * Map::TILE_SIZE, corner_a.y * Map::TILE_SIZE, Map::TILE_SIZE, Map::TILE_SIZE, RED );
		utility::draw_debug_rect( corner_b.x * Map::TILE_SIZE, corner_b.y * Map::TILE_SIZE, Map::TILE_SIZE, Map::TILE_SIZE, GREEN );
	}

	std::string str() const override { return "UnitState_MoveIn"; }
};