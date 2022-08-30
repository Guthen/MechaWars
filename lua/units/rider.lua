local DATA = {}
DATA.health = 75

--  renderer
DATA.texture_path = "assets/textures/units/rider.png"
DATA.anim_quads = {
	{ 0, 0, 8, 8 },
	{ 8, 0, 8, 8 },
} 
DATA.team_quad = { 16, 0, 8, 8 }

--  stats
DATA.can_attack = true
DATA.move_speed = 20.0

--  build
DATA.can_build = false
DATA.work_time = .3

--  shoot
DATA.shoot = {}
DATA.shoot.should_predict_movement = false
DATA.shoot.attack_range = 10.0
DATA.shoot.setup_delay = 0.0
DATA.shoot.fire_delay = .7
DATA.shoot.spread = 10
DATA.shoot.accuracy = 0.7
DATA.shoot.damage = 5
DATA.shoot.bullet_speed = 200.0
DATA.shoot.burst_count = 1
DATA.shoot.burst_delay = .5
DATA.shoot.explosion_radius = 0

define_unit( "rider", DATA )