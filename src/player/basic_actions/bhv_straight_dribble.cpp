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
    const WorldModel & wm = agent->world();
    const ServerParam & SP = ServerParam::i();

    constexpr int dash_cycle = 5;
    constexpr double kickable_area_rate = 0.5;

    dlog.addText( Logger::ACTION,
                  __FILE__, ": Bhv_StraightDribble" );

    static int s_cycle_after_kick = 10000;

    ++s_cycle_after_kick;
    dlog.addText( Logger::TEAM,
                  __FILE__": cycle: %d", s_cycle_after_kick );

    if ( wm.self().isKickable() && dash_cycle + 1 <= s_cycle_after_kick )
    {
        if ( doTurn( agent ) )
        {
            return true;
        }
    }

    if ( wm.self().isKickable() && s_cycle_after_kick >= 1 + dash_cycle )
    {
        const Vector2D after_player_pos = getPlayerMovePos( agent, dash_cycle );

        const Vector2D target_ball_pos = getPlayerToBallMargin( agent, after_player_pos, kickable_area_rate );

        const Vector2D their_goal_pos( SP.pitchLength() / 2.0, 0.0 );
        AngleDeg player_to_goal_angle = ( their_goal_pos - wm.ball().pos() ).dir() - wm.self().body();

        const double first_ball_speed =  SP.firstBallSpeed( target_ball_pos.dist( wm.ball().pos() ), dash_cycle + 1 );
        const Vector2D first_ball_vel = Vector2D::polar2vector( first_ball_speed, player_to_goal_angle.degree() );
        const Vector2D kick_accel = first_ball_vel - wm.ball().vel();
        const double kick_power = kick_accel.r() / wm.self().kickRate();

        dlog.addText( Logger::TEAM,
                      __FILE__": dist = %.2f, first_ball_speed = %.2f, (vx, vy) = (%.2f, %.2f)",
                      ( target_ball_pos - wm.self().pos() ).r(), first_ball_speed,
                      first_ball_vel.x, first_ball_vel.y );

        debugBallMove( wm.ball().pos(), first_ball_vel, dash_cycle );

        if ( agent->doKick( kick_power, player_to_goal_angle ) )
        {
            s_cycle_after_kick = 0;
            return true;
        }
    }
    agent->doDash( 100, ( wm.ball().pos() - wm.self().pos() ).dir() );
    return true;
}

/*-------------------------------------------------------------------*/
/*!

 */
bool
Bhv_StraighDribble::doTurn( PlayerAgent * agent )
{
    dlog.addText( Logger::TEAM,
                  __FILE__": first kickable" );

    const ServerParam & SP = ServerParam::i();
    const WorldModel & wm = agent->world();
    const Vector2D their_goal_pos( SP.pitchLength() / 2.0, 0.0 );

    const Vector2D player_to_goal = their_goal_pos - wm.self().pos();
    const AngleDeg player_to_goal_body_angle = player_to_goal.dir() - wm.self().body();

    dlog.addText( Logger::TEAM,
                  __FILE__": player_to_goal_body_angle = %.2f = %.2f - %.2f",
                  player_to_goal_body_angle.degree(),
                  player_to_goal.dir().degree(), wm.self().body().degree() );

    if ( 5.0 < player_to_goal_body_angle.abs() )
    {
        return agent->doTurn( player_to_goal_body_angle );
    }
    return false;
}

/*-------------------------------------------------------------------*/
/*!

 */
Vector2D
Bhv_StraighDribble::getPlayerMovePos( const PlayerAgent * agent,
                                      const int move_cycle )
{
    const WorldModel & wm = agent->world();
    const ServerParam & SP = ServerParam::i();
    const Vector2D their_goal_pos( SP.pitchLength() / 2.0, 0.0 );

    Vector2D after_player_pos = wm.self().pos();
    Vector2D player_vel = wm.self().vel();

    // 1st action(kick)
    after_player_pos += player_vel;
    dlog.addText( Logger::TEAM,
                  __FILE__": [1]kick action: (x, y) (%.2f, %.2f) => (%.2f, %.2f), (vx, vy) = (%.2f, %.2f)",
                  wm.self().pos().x, wm.self().pos().y,
                  after_player_pos.x, after_player_pos.y, player_vel.x, player_vel.y );
    dlog.addCircle( Logger::TEAM, after_player_pos, wm.self().playerType().playerSize(), "black" );

    AngleDeg player_to_goal_angle = ( their_goal_pos - after_player_pos ).dir() - wm.self().body();
    const double dash_rate = wm.self().dashRate() * SP.dashDirRate( player_to_goal_angle.degree() );
    Vector2D accel = Vector2D::polar2vector( SP.maxDashPower() * dash_rate, player_to_goal_angle );

    for ( int cycle = 1; cycle <= move_cycle; ++cycle )
    {
        // n cycle action(dash)
        player_vel = player_vel * wm.self().playerType().playerDecay() + accel;
        after_player_pos += player_vel;
        dlog.addText( Logger::TEAM,
                      __FILE__": [%d/%d]move action: dash_angle(%.1f) (x, y) = (%.2f, %.2f), (vx, vy) = (%.2f, %.2f)",
                      cycle, move_cycle,
                      player_to_goal_angle.degree(),
                      after_player_pos.x, after_player_pos.y, player_vel.x, player_vel.y );
        dlog.addCircle( Logger::TEAM, after_player_pos, wm.self().playerType().playerSize(), "black" );
    }
    dlog.addCircle( Logger::TEAM, after_player_pos, wm.self().playerType().kickableMargin(), "yellow" );
    dlog.addCircle( Logger::TEAM, after_player_pos, wm.self().playerType().kickableArea(), "red" );

    return after_player_pos;
}

/*-------------------------------------------------------------------*/
/*!

 */
Vector2D
Bhv_StraighDribble::getPlayerToBallMargin( const PlayerAgent * agent,
                                           const Vector2D & after_player_pos,
                                           const double kickable_area_rate )
{
    const WorldModel & wm = agent->world();
    const ServerParam & SP = ServerParam::i();
    const double must_margin = SP.ballSize();

    const double base_margin = wm.self().playerType().playerSize() + SP.ballSize();
    double margin = base_margin + wm.self().playerType().kickableMargin() * kickable_area_rate;
    margin = std::clamp( margin, base_margin + must_margin, wm.self().playerType().kickableArea() - must_margin );

    const Vector2D player_to_ball_margin = Vector2D::polar2vector( margin,
                                                                   ( after_player_pos - wm.self().pos() ).dir() );
    const Vector2D target_ball_pos = after_player_pos + player_to_ball_margin;
    dlog.addText( Logger::TEAM,
                  __FILE__": margin = %.2f = (%.2f - %.2f) * %.2f, (x, y) = (%.2f, %.2f) dir=%.2f",
                  margin, wm.self().playerType().kickableArea(), base_margin, kickable_area_rate,
                  player_to_ball_margin.x, player_to_ball_margin.y,
                  target_ball_pos.x, target_ball_pos.y, ( after_player_pos - wm.self().pos() ).dir() );
    dlog.addCircle( Logger::TEAM, target_ball_pos, SP.ballSize(), "red" );

    return target_ball_pos;
}

/*-------------------------------------------------------------------*/
/*!

 */
void
Bhv_StraighDribble::debugBallMove( const Vector2D & init_ball_pos,
                                   const Vector2D & init_ball_vel,
                                   const int dash_cycle )
{
    const ServerParam & SP = ServerParam::i();
    Vector2D ball_pos = init_ball_pos;
    Vector2D ball_vel = init_ball_vel;
    const double ball_decay = ServerParam::i().ballDecay();
    dlog.addText( Logger::TEAM,
                  __FILE__": [0/%d]ball pos test: (x, y) = (%.2f, %.2f), (vx, vy) = (%.2f, %.2f)",
                  dash_cycle + 1,
                  init_ball_pos.x, init_ball_pos.y,
                  init_ball_vel.x, init_ball_vel.y );

    for ( int cycle = 1; cycle <= dash_cycle + 1; ++cycle )
    {
        ball_pos += ball_vel;
        ball_vel *= ball_decay;
        dlog.addText( Logger::TEAM,
                      __FILE__": [%d/%d]ball pos test: (x, y) = (%.2f, %.2f), (vx, vy) = (%.2f, %.2f)",
                      cycle, dash_cycle + 1,
                      ball_pos.x, ball_pos.y,
                      ball_vel.x, ball_vel.y );
        dlog.addCircle( Logger::TEAM, ball_pos, SP.ballSize(), "blue" );
    }
}
