#pragma once
#include <src/scene.hpp>

#include <chrono>

#include <src/utility/draw.h>

#include <src/game/map.h>
#include <src/game/game_camera.h>
#include <src/game/ui/ui_tile_cursor.h>
#include <src/game_manager.h>

class GameScene : public Scene
{
private:
	GameCamera camera;
	std::weak_ptr<Map> map;
	std::weak_ptr<UITileCursor> tile_cursor;

	unsigned int seed;
public:
	GameScene( unsigned int seed = 1353857574 ) : seed( seed ) {};
	~GameScene();

	void init() override;
	void update( float dt ) override;
	void render_hud() override;

	std::weak_ptr<Map> get_map() { return map; }
};

