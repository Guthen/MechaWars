#include "unit.h"

#include "states/_unit_state_target.hpp"
#include "states/unit_state_idle.hpp"
#include "states/unit_state_attack.hpp"
#include "states/unit_state_move.hpp"

//  static
std::vector<std::weak_ptr<Unit>> Unit::units;


void Unit::_update_dest_rect() {}  //  I take control over that

Unit::Unit( const int x, const int y, std::weak_ptr<Map> map ) : WorldEntity( x, y, map )
{
	animator.set_playing( false );
	change_state( true, new_state<UnitState_Idle>() );

	dest.x = (float) x * Map::TILE_SIZE, dest.y = (float) y * Map::TILE_SIZE;
	dest.width = (float) size.x * Map::TILE_SIZE, dest.height = (float) size.y * Map::TILE_SIZE;

	DEFERED( units.push_back( _get_shared_from_this<Unit>() ); );
}

Unit::~Unit()
{
	//  free states
	if ( state )
		delete state;
	clear_states();

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

void Unit::_shoot_update( float dt )
{
	//  decrease shoot timers
	if ( _setup_timer > 0.0f )
		_setup_timer -= dt;

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

		//  check setup
		if ( _setup_timer > 0.0f )
			return;

		//  timer & shoot
		if ( ( _firing_timer -= dt ) <= 0.0f )
		{
			Int2 shot_pos = target_tmp->get_pos();

			//  predict target
			if ( data.shoot.should_predict_movement )
				if ( auto target_unit = dynamic_cast<Unit*>( target_tmp.get() ) )
				{
					Vector2 target_vel = Vector2Scale( target_unit->get_move_direction(), target_unit->get_data().move_speed );
					Vector2 predicted = utility::get_linear_predicted_position( shot_pos.to_v2(), target_vel, pos.to_v2(), data.shoot.bullet_speed );
					shot_pos = Int2::from_v2( predicted );

					//printf( "pos: x=%d; y=%d; predicted: x=%d; y=%d!\n", target_tmp->get_pos().x, target_tmp->get_pos().y, shot_pos.x, shot_pos.y );
				}

			//  fire
			fire_bullet( shot_pos );

			//  next fire delay
			_firing_timer = data.shoot.burst_delay;
			_firing_times--;
		}
	}
	//  next fire delay
	else if ( _next_fire_timer > 0.0f )
		_next_fire_timer -= dt;
}

void Unit::update( float dt )
{
	//  render pos
	dest.x = utility::approach( dest.x, (float) pos.x * Map::TILE_SIZE, dt * data.move_speed );
	dest.y = utility::approach( dest.y, (float) pos.y * Map::TILE_SIZE, dt * data.move_speed );

	//  animator
	if ( animator.update( dt ) )
		quad = animator.get_current_frame();

	//  debug draw
	if ( GameManager::is_debug_state( DEBUG_STATE::ENTITY ) && is_selected() )
	{
		DRAW_DEBUG( TextFormat( "UNIT [%d]", get_id() ) );
		DRAW_DEBUG( "TEAM: " + std::to_string( team_id ) );
		DRAW_DEBUG( "STATE: " + state->str() );
		DRAW_DEBUG( TextFormat( "SETUP TIMER: %02f", _setup_timer ) );
		DRAW_DEBUG( TextFormat( "NEXT FIRE TIMER: %02f", _next_fire_timer ) );
		DRAW_DEBUG( TextFormat( "BURST TIMES: %d", _firing_times ) );
		DRAW_DEBUG( TextFormat( "BURST TIMER: %02f", _firing_timer ) );

		//  states queue
		int queue_size = states_queue.size();
		if ( queue_size > 0 )
		{
			DRAW_DEBUG( "STATES_QUEUE: " );
			for ( int i = 0; i < queue_size; i++ )
				DRAW_DEBUG( " " + std::to_string( i ) + ": " + states_queue[i]->str());
		}
	
		//  DEBUG: destroy unit if pressing SUPPR
		if ( IsKeyPressed( KEY_DELETE ) )
			safe_destroy();
	}

	//  update shooting behaviour
	_shoot_update( dt );

	//  state
	if ( state )
		state->update( dt );
}

void Unit::render()
{
	if ( state )
		state->render();

	WorldEntity::render();
}

void Unit::on_right_click_selected()
{
	auto map_tmp = map.lock();
	if ( !map_tmp ) return;

	//  should the future state be queued?
	bool is_queued = IsKeyDown( KEY_LEFT_SHIFT );

	Int2 tile_mouse_pos = GameCamera::get_current()->get_tile_mouse_pos();
	if ( map_tmp->has_structure_at( tile_mouse_pos.x, tile_mouse_pos.y ) )
	{
		std::weak_ptr<WorldEntity> target = map_tmp->get_structure_at_pos( tile_mouse_pos.x, tile_mouse_pos.y );
		if ( auto target_tmp = target.lock() )
		{
			//  check if resource
			TEAM target_team = target_tmp->get_team();
			if ( target_team == TEAM_NONE )
			{
				//  move near the resource
				move_to( is_queued, tile_mouse_pos );
				return;
			}
			//  check if ally
			if ( target_team == get_team() )
				return;

			//  attack hostile entity
			attack_target( is_queued, target_tmp );
		}
	}
	else
	{
		//  DEBUG: instant move
		if ( GameManager::is_debug_state( DEBUG_STATE::ENTITY ) && IsKeyDown( KEY_LEFT_CONTROL ) )
		{
			unreserve_pos();
			set_pos( tile_mouse_pos );
			dest.x = (float) pos.x * Map::TILE_SIZE, dest.y = (float) pos.y * Map::TILE_SIZE;
			reserve_pos();

			change_state( false, new_state<UnitState_Idle>() );
			return;
		}

		//  move towards
		move_to( is_queued, tile_mouse_pos );
	}
}

void Unit::change_state( bool no_delete, UnitState* _state )
{
	//  delete old one
	if ( !no_delete && state )
		delete state;

	//  apply next state
	state = _state;
	state->init();
}

void Unit::next_state()
{
	//  default to idle
	if ( !has_next_state() )
	{
		change_state( false, new_state<UnitState_Idle>() );
		return;
	}

	//  move to next state
	change_state( false, states_queue.front() );
	states_queue.pop_front();
}

void Unit::clear_states()
{
	//  free states queue
	for ( UnitState* _state : states_queue )
		delete _state;

	states_queue.clear();
}

void Unit::move_to( bool is_queued, Int2 goal )
{
	//  queue movement
	if ( is_queued )
		push_state( false, new_state<UnitState_Move>( goal ) );
	else
	{
		//  change goal if we are already moving (not targeting someone)
		UnitState_Move* move_state = nullptr;
		if ( ( move_state = dynamic_cast<UnitState_Move*>( state ) ) && move_state->can_change_goal )
			move_state->set_goal( goal );
		//  move!
		else
		{
			clear_states();
			change_state( false, new_state<UnitState_Move>( goal ) );
		}
	}
}

void Unit::attack_target( bool is_queued, std::weak_ptr<WorldEntity> target )
{
	//  queue attack
	if ( is_queued )
		push_state( false, new_state<UnitState_Attack>( target ) );
	else
	{
		//  change target if we are already shooting
		UnitState_Attack* attack_state = nullptr;
		if ( attack_state = dynamic_cast<UnitState_Attack*>( state ) )
			attack_state->set_target( target );
		//  attack!
		else
		{
			clear_states();
			change_state( false, new_state<UnitState_Attack>( target ) );
		}
	}
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
	float dir_ang = Vector2Angle( pos.to_v2(), shoot_target.to_v2() );
	Vector2 dir = ( shoot_target - pos ).to_v2();

	//  accuracy (spread in a circle shape)
	float angle = dir_ang;
	float dist = sqrtf( 
		(float) (
			( pos.x - shoot_target.x ) * ( pos.x - shoot_target.x ) 
			 + ( pos.y - shoot_target.y ) * ( pos.y - shoot_target.y ) 
		)
	);

	//  check accuracy
	float accuracy = data.shoot.accuracy;
	if ( accuracy > 0.0f && accuracy < 1.0f )  //  avoid generating numbers when accuracy is an extreme
		accuracy = rand() / static_cast<float>( RAND_MAX );  //  random number in range: [0.0f; 1.0f]

	if ( accuracy == 0.0f || accuracy > data.shoot.accuracy )
	{
		//  get random spread in range [-data.shoot.spread; -1] & [1; data.shoot.spread] (avoid 0)
		float spread = (float) GetRandomValue( 1, data.shoot.spread );
		if ( rand() % 2 == 0 )
			spread *= -1.0f;

		//  apply angle spread
		angle += ( spread * dist / data.shoot.attack_range );

		//  apply distance offset: spread_ratio * dist_ratio * offset
		dist += ( spread / data.shoot.spread ) * ( dist / data.shoot.attack_range ) * ( data.shoot.spread / 6.0f );
	}

	//  apply angle randomness
	if ( data.shoot.spread > 0 )
		angle += GetRandomValue( -data.shoot.spread, data.shoot.spread ) / 3.0f;
	//  convert angle to rad
	angle *= DEG2RAD;

	//  compute new direction
	dir.x = cosf( angle ) * dist;
	dir.y = sinf( angle ) * dist;

	//  normalize direction
	Vector2 move_dir = Vector2 { dir.x / dist, dir.y / dist };

	//  spawn bullet
	GameManager::create<Bullet>( 
		map,  //  map
		Vector2 {  //  bullet's origin
			dest.x + dest.width / 2, 
			dest.y + dest.height / 2 
		}, 
		move_dir,  //  direction to move
		dist * Map::TILE_SIZE,  //  distance to move
		data.shoot.damage,  //  damage
		data.shoot.explosion_radius,  //  explosion radius
		data.shoot.bullet_speed  //  bullet speed
	);

	//  knockback
	float knockback_amount = (float) ( Map::TILE_SIZE / 2.0f ) / (float) data.shoot.burst_count / (float) data.shoot.burst_delay;
	dest.x -= move_dir.x * knockback_amount;
	dest.y -= move_dir.y * knockback_amount;
}

bool Unit::is_firing() { return _firing_times > 0; }
bool Unit::can_fire() { return !is_firing() && _next_fire_timer <= 0.0f; }