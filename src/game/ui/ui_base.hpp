#pragma once

#include "../../entity.h"

class UIBase : public Entity
{
protected:
	Color color = WHITE;
public:
	UIBase()
	{
		z_order = UI_Z_ORDER;
	}

	void set_color( Color _color ) { color = _color; }
	Color get_color() { return color; }
};