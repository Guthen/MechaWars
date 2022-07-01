#pragma once

#include "../scene.hpp"

#include <chrono>

#include "../map.h"
#include "game_camera.h"
#include "ui/ui_tile_cursor.h"
#include "../game_manager.h"
#include "../utility/draw.h"

class GameScene : public Scene
{
private:
	GameCamera camera;
	std::weak_ptr<Map> map;
	std::weak_ptr<UITileCursor> tile_cursor;

	unsigned int seed;
public:
	GameScene( unsigned int seed = 1353857574 ) : seed( seed ) {};

	void init() override;
	void update( float dt ) override;
	void render_hud() override;

	std::weak_ptr<Map> get_map() { return map; }
};

