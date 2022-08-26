#include "structure_blueprint.h"

StructureBlueprint::StructureBlueprint( int x, int y, int w, int h, std::weak_ptr<Map> map ) : Structure( x, y, w, h, map )
{
	texture = AssetsManager::get_or_load_texture( "assets/textures/buildings/blueprint.png" );

	//  render texture
	rt = LoadRenderTexture( w * Map::TILE_SIZE, h * Map::TILE_SIZE );
	quad = Rectangle { 0, 0, (float) rt.texture.width, (float) -rt.texture.height };
	should_update_rt = true;  //  defered update for team color

	work_to_make = 100;
	max_health = 1000;
	health_per_work = max_health / work_to_make;
}

Vector2 zero { 0.0f, 0.0f };
void _draw_tile( Texture2D texture, Int2 quad, Rectangle dest, bool flip_x, bool draw_team_quad, Color team_color = WHITE )
{
	Rectangle source { quad.x * Map::TILE_SIZE, quad.y * Map::TILE_SIZE, Map::TILE_SIZE, Map::TILE_SIZE };

	//  flip horizontally
	if ( flip_x )
		source.width *= -1.0f;

	//  draw shape tile
	DrawTexturePro( texture, source, dest, zero, 0.0f, WHITE );

	//  draw team colored tile
	if ( draw_team_quad )
	{
		source.x += 3 * Map::TILE_SIZE;
		DrawTexturePro( texture, source, dest, zero, 0.0f, team_color );
	}
}

void StructureBlueprint::_rt_update()
{
	Rectangle dest { 0, 0, Map::TILE_SIZE, Map::TILE_SIZE };
	Rectangle quad = dest;
	Vector2 half { Map::TILE_SIZE / 2, Map::TILE_SIZE / 2 };

	BeginTextureMode( rt );

	//  tiles
	if ( size.x == 1 && size.y == 1 )
		_draw_tile( texture, QUAD_SINGLE, dest, false, true, color );
	else
	{
		int last_y = size.y - 1, last_x = size.x - 1;

		for ( int y = 0; y < size.y; y++ )
		{
			dest.y = y * Map::TILE_SIZE;

			for ( int x = 0; x < size.x; x++ )
			{
				dest.x = x * Map::TILE_SIZE;

				//  thin tall
				if ( size.y == 1 )
				{
					if ( x == 0 )
						_draw_tile( texture, QUAD_LEFT_CORNER, dest, false, true, color );
					else if ( x == last_x )
						_draw_tile( texture, QUAD_LEFT_CORNER, dest, true, true, color );
					else
						_draw_tile( texture, QUAD_HORIZONTAL, dest, false, true, color );
				}
				//  thin width
				else if ( size.x == 1 )
				{
					if ( y == 0 )
						_draw_tile( texture, QUAD_TOP_CORNER, dest, false, true, color );
					else if ( y == last_y )
						_draw_tile( texture, QUAD_BOTTOM_CORNER, dest, true, true, color );
					else
						_draw_tile( texture, QUAD_VERTICAL, dest, false, true, color );
				}
				else
				{
					//  1st row..
					if ( y == 0 )
					{
						if ( x == 0 )
							_draw_tile( texture, QUAD_TOP_LEFT, dest, false, true, color );
						else if ( x == last_x )
							_draw_tile( texture, QUAD_TOP_LEFT, dest, true, true, color );
						else
							_draw_tile( texture, QUAD_TOP, dest, false, true, color );
					}
					//  last row..
					else if ( y == last_y )
					{
						if ( x == 0 )
							_draw_tile( texture, QUAD_BOTTOM_LEFT, dest, false, true, color );
						else if ( x == last_x )
							_draw_tile( texture, QUAD_BOTTOM_LEFT, dest, true, true, color );
						else
							_draw_tile( texture, QUAD_BOTTOM, dest, false, true, color );
					}
					//  remaining rows..
					else
					{
						if ( x == 0 )
							_draw_tile( texture, QUAD_LEFT, dest, false, true, color );
						else if ( x == last_x )
							_draw_tile( texture, QUAD_LEFT, dest, true, true, color );
						else
						{
							//  stuff variety
							Int2 quad = QUADS_STUFF[GetRandomValue( 0, QUAD_STUFF_LENGTH - 1 )];
							_draw_tile( texture, quad, dest, false, false, WHITE );
						}
					}
				}
			}
		}
	}
	
	EndTextureMode();
}

void StructureBlueprint::init()
{
	health = 0;
}

void StructureBlueprint::debug_update( float dt )
{
	//  base
	Structure::debug_update( dt );

	DRAW_DEBUG( TextFormat( "work_to_make: %d", work_to_make ) );
	DRAW_DEBUG( TextFormat( "health_per_work: %d", health_per_work ) );
}

void StructureBlueprint::advance_work()
{
	if ( work_to_make-- <= 0 )
		finish_work();
	else
		health += health_per_work;
}

void StructureBlueprint::finish_work()
{
	//  TODO: create implemented structure
	//  ..

	//  destroy blueprint
	safe_destroy();
}

void StructureBlueprint::render()
{
	//  update render texture
	if ( should_update_rt )
	{
		_rt_update();
		should_update_rt = false;
	}

	//  start hit shader
	if ( is_currently_hit )
		BeginShaderMode( hit_shader );

	//  draw render texture
	DrawTextureRec( rt.texture, quad, Vector2 { dest.x, dest.y }, WHITE );

	//  end hit shader
	if ( is_currently_hit )
		EndShaderMode();
}