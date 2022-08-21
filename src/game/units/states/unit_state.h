#pragma once

#include "unit_state.fwd.h"
#include "../unit.fwd.h"

#include <string>

class UnitState
{
protected:
	Unit* unit;
public:
	UnitState( Unit* unit );
	virtual ~UnitState() {};  //  virtual destructors are important

	virtual void init() {};

	virtual void update( float dt ) {};
	virtual void render() {};

	virtual std::string str() const { return "UnitState_Base"; };
};