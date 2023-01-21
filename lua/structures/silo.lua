local DATA = {}
DATA.health = 500
DATA.work_to_make = 50

--  renderer
DATA.texture_path = "assets/textures/structures/missile_silo.png"
DATA.quad = { 0, 0, 8, 8 }
DATA.team_quad = { 8, 0, 8, 8 }

define_structure( "silo", DATA )