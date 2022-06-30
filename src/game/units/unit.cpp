#include "unit.h"

#include "states/unit_state_idle.hpp"
#include "states/unit_state_shoot.hpp"

//  static
std::vector<Unit*> Unit::units;


Unit::Unit( const int x, const int y, Map* map ) : WorldEntity( x, y, map )
{
	change_state<UnitState_Idle>();
	render_pos = ( pos * Map::TILE_SIZE ).to_v2();

	units.push_back( this );
}

Unit::~Unit()
{
	//  free state
	if ( state )
		delete state;

	//  remove from list
	std::vector<Unit*>::iterator it = units.begin();
	while ( !( it == units.end() ) )
	{
		if ( *it == this )
		{
			units.erase( it );
			printf( "%p erased!\n", this );
			break;
		}
		it++;
	}
}

void Unit::update( float dt )
{
	//  render pos
	if ( should_update_render_pos )
	{
		render_pos.x = utility::approach( render_pos.x, (float) pos.x * Map::TILE_SIZE, dt * move_speed );
		render_pos.y = utility::approach( render_pos.y, (float) pos.y * Map::TILE_SIZE, dt * move_speed );
		_update_dest_rect();
	}

	//  animator
	animator.update( dt );
	quad = animator.get_current_frame();

	//  debug draw
	DRAW_DEBUG( TextFormat( "UNIT [%d]", get_id() ) );
	DRAW_DEBUG( "TEAM: " + std::to_string( team_id ) );
	DRAW_DEBUG( "STATE: " + state->str() );

	//  state
	if ( state )
		state->update( dt );
}

void Unit::on_right_click_selected()
{
	Int2 tile_mouse_pos = GameCamera::get_current()->get_tile_mouse_pos();
	if ( map->has_structure_at( tile_mouse_pos.x, tile_mouse_pos.y ) )
	{
		WorldEntity* target = map->get_structure_at_pos( tile_mouse_pos.x, tile_mouse_pos.y );

		TEAM target_team = target->get_team();
		if ( target_team == TEAM_NONE || target_team == get_team() )
			return;

		//  change target
		shoot_target( target );
	}
}

void Unit::shoot_target( WorldEntity* target )
{
	UnitState_Shoot* shoot_state = nullptr;
	if ( ( shoot_state = dynamic_cast<UnitState_Shoot*>( state ) ) )
		shoot_state->set_target( target );
	//  start to shoot
	else
		change_state<UnitState_Shoot>( target );
}

void Unit::shoot_to( Int2 shoot_target )
{
	Vector2 dir = ( shoot_target - pos ).to_v2();
	Vector2 move_dir = Vector2Normalize( dir );
	
	//  bullet
	#pragma region Bullet
		float dist_to_move = Vector2Length( dir ) * Map::TILE_SIZE;

		//  spawn bullet
		Vector2 bullet_pos = ( pos * Map::TILE_SIZE ).to_v2();
		bullet_pos.x += (float) ( Map::TILE_SIZE / 2 );
		bullet_pos.y += (float) ( Map::TILE_SIZE / 2 );
		GameManager::create<Bullet>( bullet_pos, move_dir, dist_to_move );
	#pragma endregion

	//  knockback
	render_pos.x -= move_dir.x * 5.0f;
	render_pos.y -= move_dir.y * 5.0f;
	_update_dest_rect();

	//  wait effect
	should_update_render_pos = false;
	TIMER( .1f, should_update_render_pos = true; );
}