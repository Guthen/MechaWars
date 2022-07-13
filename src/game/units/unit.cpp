#include "unit.h"

#include "states/unit_state_idle.hpp"
#include "states/unit_state_shoot.hpp"
#include "states/unit_state_move.hpp"

//  static
std::vector<std::weak_ptr<Unit>> Unit::units;


void Unit::_update_dest_rect() {}  //  I take control over that

Unit::Unit( const int x, const int y, std::weak_ptr<Map> map ) : WorldEntity( x, y, map )
{
	animator.set_playing( false );
	change_state<UnitState_Idle>();

	dest.x = (float) x * Map::TILE_SIZE, dest.y = (float) y * Map::TILE_SIZE;
	dest.width = (float) size.x * Map::TILE_SIZE, dest.height = (float) size.y * Map::TILE_SIZE;

	DEFERED( units.push_back( _get_shared_from_this<Unit>() ); );
}

Unit::~Unit()
{
	//  free state
	if ( state )
		delete state;

	//  remove from list
	std::vector<std::weak_ptr<Unit>>::iterator it = units.begin();
	while ( !( it == units.end() ) )
	{
		std::weak_ptr<Unit> unit = *it;
		if ( auto unit_tmp = unit.lock() )
		{
			//  remove self
			if ( unit_tmp.get() == this )
			{
				units.erase( it );
				break;
			}

			it++;
		}
		//  remove invalid pointer
		else
			it = units.erase( it );
	}
}

void Unit::update( float dt )
{
	//  render pos
	if ( should_update_render_pos )
	{
		dest.x = utility::approach( dest.x, (float) pos.x * Map::TILE_SIZE, dt * data.move_speed );
		dest.y = utility::approach( dest.y, (float) pos.y * Map::TILE_SIZE, dt * data.move_speed );
	}

	//  animator
	if ( animator.update( dt ) )
		quad = animator.get_current_frame();

	//  debug draw
	if ( GameManager::is_debug_state( DEBUG_STATE::ENTITY ) && !selecting_cursor.expired() )
	{
		DRAW_DEBUG( TextFormat( "UNIT [%d]", get_id() ) );
		DRAW_DEBUG( "TEAM: " + std::to_string( team_id ) );
		DRAW_DEBUG( "STATE: " + state->str() );
	}

	//  burst firing
	if ( is_firing() )
	{
		//  target validity
		std::shared_ptr<WorldEntity> target_tmp = _firing_target.lock();
		if ( !target_tmp )
		{
			_firing_times = 0;
			return;
		}

		//  timer & shoot
		if ( ( _firing_timer -= dt ) <= 0.0f )
		{
			fire_bullet( target_tmp->get_pos() );

			_firing_timer = data.shoot.burst_delay;
			_firing_times--;
		}
	}
	//  next fire delay
	else if ( _next_fire_timer > 0.0f )
		_next_fire_timer -= dt;

	//  state
	if ( state )
		state->update( dt );
}

void Unit::render()
{
	WorldEntity::render();

	if ( state )
		state->render();
}

void Unit::on_right_click_selected()
{
	auto map_tmp = map.lock();
	if ( !map_tmp ) return;

	Int2 tile_mouse_pos = GameCamera::get_current()->get_tile_mouse_pos();
	if ( map_tmp->has_structure_at( tile_mouse_pos.x, tile_mouse_pos.y ) )
	{
		std::weak_ptr<WorldEntity> target = map_tmp->get_structure_at_pos( tile_mouse_pos.x, tile_mouse_pos.y );
		if ( auto target_tmp = target.lock() )
		{
			//  shoot hostile entity
			TEAM target_team = target_tmp->get_team();
			if ( target_team == TEAM_NONE )
			{
				move_to( tile_mouse_pos );  //  move near the resource
				return;
			}
			if ( target_team == get_team() )
				return;

			shoot_target( target_tmp );
		}
	}
	else
		//  move towards
		move_to( tile_mouse_pos );
}

void Unit::move_to( Int2 goal )
{
	UnitState_Move* move_state = nullptr;
	//  change goal if we are already moving
	if ( move_state = dynamic_cast<UnitState_Move*>( state ) )
		move_state->set_target( goal );
	//  move!
	else
		change_state<UnitState_Move>( goal );
}

void Unit::shoot_target( std::weak_ptr<WorldEntity> target )
{
	UnitState_Shoot* shoot_state = nullptr;
	//  change target if we are already shooting
	if ( shoot_state = dynamic_cast<UnitState_Shoot*>( state ) )
		shoot_state->set_target( target );
	//  start to shoot
	else
		change_state<UnitState_Shoot>( target );
}

void Unit::shoot_to( std::weak_ptr<WorldEntity> target )
{
	_firing_times = data.shoot.burst_count;
	_firing_timer = 0.0f;
	_firing_target = target;
	_next_fire_timer = data.shoot.fire_delay;
}

void Unit::fire_bullet( Int2 shoot_target )
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
		GameManager::create<Bullet>( map, bullet_pos, move_dir, dist_to_move );
	#pragma endregion

	//  knockback
	float knockback_amount = (float) Map::TILE_SIZE / 2;
	dest.x -= move_dir.x * knockback_amount;
	dest.y -= move_dir.y * knockback_amount;
	should_update_render_pos = false;
	TIMER( .1f, should_update_render_pos = true; );
}

bool Unit::is_firing() { return _firing_times > 0; }
bool Unit::can_fire() { return !is_firing() && _next_fire_timer <= 0.0f; }