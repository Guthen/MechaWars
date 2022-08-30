local DATA = {}
DATA.health = 50

--  renderer
DATA.texture_path = "assets/textures/units/fastor.png"
DATA.anim_quads = {
	{ 0, 0, 8, 8 },
	{ 8, 0, 8, 8 },
} 
DATA.team_quad = { 16, 0, 8, 8 }

--  stats
DATA.can_attack = false
DATA.move_speed = 12.0

--  build
DATA.can_build = true
DATA.work_time = .3

define_unit( "fastor", DATA )