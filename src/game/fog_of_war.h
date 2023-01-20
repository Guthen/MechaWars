#pragma once
#include "fog_of_war.fwd.h"
#include "commander.fwd.h"

#include <raylib.h>

#include "map.h"

using bool_vector = std::vector<bool>;
using bool_vector2D = std::vector<bool_vector>;

class FogOfWar : public Entity
{
private:
	RenderTexture2D rt;
	void _rt_update();
	bool should_update_rt { true };
	bool should_update { true };

	bool_vector2D visible_tiles;

	Commander* commander { nullptr };
	std::weak_ptr<Map> map;
public:
	FogOfWar( Commander* commander, std::shared_ptr<Map> map );

	void schedule_update() { should_update = true; }

	bool is_visible( const Int2& pos );
	void provide_vision( Int2 pos, float range );

	void force_update();

	void update( float dt ) override;
	void render() override;
};

