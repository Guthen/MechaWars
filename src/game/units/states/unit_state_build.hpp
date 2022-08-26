#pragma once

#include "unit_state_move_to.hpp"
#include "../../structures/structure_blueprint.hpp"

class UnitState_Build : public _UnitState_Target
{
protected:
	float next_work_time = 0.0f;
public:
	UnitState_Build( Unit* unit, std::weak_ptr<WorldEntity> target ) : _UnitState_Target( unit, target ) {}

	void init() override
	{
		next_work_time = unit->get_data().work_time;
	}

	void update( float dt ) override
	{
		//  check target validity
		auto target_tmp = target.lock();
		if ( !target_tmp )
		{
			unit->next_state();
			return;
		}

		//  check it's a blueprint structure
		StructureBlueprint* blueprint = dynamic_cast<StructureBlueprint*>( target_tmp.get() );
		if ( !blueprint )
		{
			unit->next_state();
			return;
		}

		//  ensure we are adjacent to the target
		float dist = utility::distance_to_sqr( unit->get_pos(), target_tmp->get_pos() );
		if ( dist > 1.0f )
		{
			unit->change_state( true, unit->new_state<UnitState_MoveTo>( target, 1.0f ) );
			unit->push_state( true, this );
		}

		//  advance to work
		if ( ( next_work_time -= dt ) <= 0.0f )
		{
			blueprint->advance_work();
			
			//  reset timer
			next_work_time = unit->get_data().work_time;
		}
	}

	std::string str() const override { return "UnitState_Build"; }
};