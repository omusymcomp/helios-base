// -*-c++-*-

/*!
  \file bhv_straigh_dribble.cpp
  \brief behaviro straight dribble.
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "bhv_straight_dribble.h"

#include "bhv_scan_field.h"

#include <rcsc/player/player_agent.h>
#include <rcsc/common/logger.h>

using namespace rcsc;

/*-------------------------------------------------------------------*/
/*!

 */
bool
Bhv_StraighDribble::execute( PlayerAgent * agent )
{
    const ServerParam & SP = ServerParam::i();
    const WorldModel & wm = agent->world();

    constexpr int dash_cycle = 2;
    // constexpr double kickable_alea_rate = 0.5;

    dlog.addText( Logger::ACTION,
                  __FILE__, ": Bhv_StraightDribble" );

    static int s_cycle_after_kick = 10000;

    ++s_cycle_after_kick;
    dlog.addText( Logger::TEAM,
                  __FILE__": cycle: %d", s_cycle_after_kick );

    if ( s_cycle_after_kick >= 1 + dash_cycle )
    {
        const Vector2D player_move_pos = get_player_move_dist( agent, dash_cycle );

        const Vector2D player_to_ball_margin = Vector2D::polar2vector( wm.self().playerType().playerSize() + SP.ballSize() + 0.1,
                                                                       ( player_move_pos - wm.self().pos() ).dir() );
        const Vector2D ball_pos = player_move_pos + player_to_ball_margin;
        dlog.addCircle( Logger::TEAM, ball_pos, 0.05, "black" );
        dlog.addText( Logger::TEAM,
                      __FILE__": ball margin: (%.2f, %.2f), (x, y) = (%.2f, %.2f) dir=%.2f",
                      player_to_ball_margin.x, player_to_ball_margin.y,
                      ball_pos.x, ball_pos.y, ( player_move_pos - wm.self().pos() ).dir() );

        const Vector2D their_goal_pos( SP.pitchLength() / 2.0, 0.0 );
        AngleDeg player_to_goal_angle = ( their_goal_pos - wm.ball().pos() ).dir() - wm.self().body();

        const double first_ball_speed =  SP.firstBallSpeed( ball_pos.dist( wm.ball().pos() ), dash_cycle + 1 );
        const Vector2D first_ball_vel = Vector2D::polar2vector( first_ball_speed, player_to_goal_angle.degree() );
        const Vector2D kick_accel = first_ball_vel - wm.ball().vel();
        const double kick_power = kick_accel.r() / wm.self().kickRate();

        dlog.addText( Logger::TEAM,
                      __FILE__": dist = %.2f, first_ball_speed = %.2f, (vx, vy) = (%.2f, %.2f)",
                      ( ball_pos - wm.self().pos() ).r(), first_ball_speed,
                      first_ball_vel.x, first_ball_vel.y );


        Vector2D test_ball_pos = wm.ball().pos();
        Vector2D test_ball_vel = first_ball_vel;
        dlog.addText( Logger::TEAM,
                      __FILE__": [0]ball pos test: (x, y) = (%.2f, %.2f), (vx, vy) = (%.2f, %.2f)",
                      test_ball_pos.x, test_ball_pos.y,
                      test_ball_vel.x, test_ball_vel.y );
        test_ball_pos += test_ball_vel;
        test_ball_vel *= SP.ballDecay();
        dlog.addText( Logger::TEAM,
                      __FILE__": [1]ball pos test: (x, y) = (%.2f, %.2f), (vx, vy) = (%.2f, %.2f)",
                      test_ball_pos.x, test_ball_pos.y,
                      test_ball_vel.x, test_ball_vel.y );
        test_ball_pos += test_ball_vel;
        test_ball_vel *= SP.ballDecay();
        dlog.addText( Logger::TEAM,
                      __FILE__": [2]ball pos test: (x, y) = (%.2f, %.2f), (vx, vy) = (%.2f, %.2f)",
                      test_ball_pos.x, test_ball_pos.y,
                      test_ball_vel.x, test_ball_vel.y );
        test_ball_pos += test_ball_vel;
        test_ball_vel *= SP.ballDecay();
        dlog.addText( Logger::TEAM,
                      __FILE__": [3]ball pos test: (x, y) = (%.2f, %.2f), (vx, vy) = (%.2f, %.2f)",
                      test_ball_pos.x, test_ball_pos.y,
                      test_ball_vel.x, test_ball_vel.y );

        if ( agent->doKick( kick_power, player_to_goal_angle ) )
        {
            s_cycle_after_kick = 0;
            return true;
        }
    }
    agent->doDash(100, ( wm.ball().pos() - wm.self().pos() ).dir() );
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
rcsc::Vector2D
Bhv_StraighDribble::get_player_move_dist( const PlayerAgent * agent,
                                          const int move_cycle )
{
    const ServerParam & SP = ServerParam::i();
    const WorldModel & wm = agent->world();
    const Vector2D their_goal_pos( SP.pitchLength() / 2.0, 0.0 );

    Vector2D player_move_pos = wm.self().pos();
    Vector2D player_vel = wm.self().vel();

    // 1st action(kick)
    player_move_pos += player_vel;
    dlog.addText( Logger::TEAM,
                    __FILE__": [1]kick action: (x, y) (%.2f, %.2f) => (%.2f, %.2f), (vx, vy) = (%.2f, %.2f)",
                    wm.self().pos().x, wm.self().pos().y,
                    player_move_pos.x, player_move_pos.y, player_vel.x, player_vel.y );

    AngleDeg player_to_goal_angle = ( their_goal_pos - player_move_pos ).dir() - wm.self().body();
    const double dash_rate = wm.self().dashRate() * SP.dashDirRate( player_to_goal_angle.degree() );
    Vector2D accel = Vector2D::polar2vector( SP.maxDashPower() * dash_rate, player_to_goal_angle );

    for ( int cycle = 1; cycle <= move_cycle; ++cycle )
    {
        // n cycle action(dash)
        player_vel = player_vel * wm.self().playerType().playerDecay() + accel;
        player_move_pos += player_vel;
        dlog.addText( Logger::TEAM,
                      __FILE__": [%d/%d]move action: dash_angle(%.1f) (x, y) = (%.2f, %.2f), (vx, vy) = (%.2f, %.2f)",
                      cycle, move_cycle,
                      player_to_goal_angle.degree(),
                      player_move_pos.x, player_move_pos.y, player_vel.x, player_vel.y );
    }
    dlog.addCircle( Logger::TEAM, player_move_pos, 0.05, "black" );

    return player_move_pos;
}