local DATA = {}
DATA.health = 5000
DATA.work_to_make = 500
DATA.size = { 2, 2 }

--  renderer
DATA.texture_path = "assets/textures/structures/nexus.png"
DATA.quad = { 0, 0, 16, 16 }
DATA.team_quad = { 16, 0, 16, 16 }

define_structure( "nexus", DATA )