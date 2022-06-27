#include "unit.h"

void Unit::update( float dt )
{
	animator.update( dt );
	quad = animator.get_current_frame();
}