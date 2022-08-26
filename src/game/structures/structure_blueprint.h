#pragma once

#include "structure.h"

constexpr size_t QUAD_STUFF_LENGTH = 2;

class StructureBlueprint : public Structure
{
private:
	RenderTexture2D rt;  //  render texture used to batch the tilemap into one render call
	void _rt_update();
	bool should_update_rt;
protected:
	int work_to_make = 0;
	int health_per_work = 0;

	//  quads tile pos constant
	const Int2 QUAD_SINGLE { 0, 0 }; const Int2 QUAD_TOP_LEFT { 1, 0 }; const Int2 QUAD_TOP { 2, 0 };
	const Int2 QUAD_LEFT { 0, 1 }; const Int2 QUAD_BOTTOM_LEFT { 1, 1 }; const Int2 QUAD_BOTTOM { 2, 1 };
	const Int2 QUAD_LEFT_CORNER { 0, 2 }; const Int2 QUAD_HORIZONTAL { 1, 2 }; 
	const Int2 QUAD_TOP_CORNER { 0, 3 }; const Int2 QUAD_VERTICAL { 1, 3 }; const Int2 QUAD_BOTTOM_CORNER { 2, 3 };

	const Int2 QUADS_STUFF[QUAD_STUFF_LENGTH] 
	{
		Int2 { 0, 4 },
		Int2 { 1, 4 },
	};
public:
	StructureBlueprint( int x, int y, int w, int h, std::weak_ptr<Map> map );

	void init() override;
	void debug_update( float dt ) override;

	void advance_work();
	void finish_work();

	void render() override;
};