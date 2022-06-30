#include "unit_state.h"
#include "../unit.h"

UnitState::UnitState( Unit* unit ) : unit( unit ) 
{
	printf( "UnitState::unit = %p\n", unit );
}