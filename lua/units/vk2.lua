local DATA = {}
DATA.health = 250

--  renderer
DATA.texture_path = "assets/textures/units/vk2.png"
DATA.anim_quads = {
	{ 0, 0, 8, 8 },
	{ 8, 0, 8, 8 },
} 
DATA.team_quad = { 16, 0, 8, 8 }

--  stats
DATA.can_attack = true
DATA.move_speed = 8.0
DATA.vision_range = 3.5

--  build
DATA.can_build = false
DATA.work_time = .3

--  shoot
DATA.shoot = {}
DATA.shoot.should_predict_movement = false
DATA.shoot.attack_range = 16.0
DATA.shoot.setup_delay = 1.0
DATA.shoot.fire_delay = 7.0
DATA.shoot.spread = 15
DATA.shoot.accuracy = 0.15
DATA.shoot.damage = 20
DATA.shoot.bullet_speed = 150.0
DATA.shoot.burst_count = 3
DATA.shoot.burst_delay = .5
DATA.shoot.explosion_radius = 2

define_unit( "vk2", DATA )