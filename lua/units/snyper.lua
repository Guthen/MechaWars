local DATA = {}
DATA.health = 120

--  renderer
DATA.texture_path = "assets/textures/units/snyper.png"
DATA.anim_quads = {
	{ 0, 0, 8, 8 },
	{ 8, 0, 8, 8 },
} 
DATA.team_quad = { 16, 0, 8, 8 }

--  stats
DATA.can_attack = true
DATA.move_speed = 16.0
DATA.vision_range = 8.5

--  build
DATA.can_build = false
DATA.work_time = .3

--  shoot
DATA.shoot = {}
DATA.shoot.should_predict_movement = true
DATA.shoot.attack_range = 22.0
DATA.shoot.setup_delay = 3.0
DATA.shoot.fire_delay = 5.0
DATA.shoot.spread = 1
DATA.shoot.accuracy = 1.0
DATA.shoot.damage = 40
DATA.shoot.bullet_speed = 300.0
DATA.shoot.burst_count = 1
DATA.shoot.burst_delay = .5
DATA.shoot.explosion_radius = 0

define_unit( "snyper", DATA )