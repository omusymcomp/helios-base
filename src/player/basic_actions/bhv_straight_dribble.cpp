// -*-c++-*-

/*!
  \file body_kick_one_step.cpp
  \brief one step kick behavior.
*/

/*
 *Copyright:

 Copyright (C) Hidehisa AKIYAMA

 This code is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

 *EndCopyright:
 */

/////////////////////////////////////////////////////////////////////

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "bhv_straight_dribble.h"

#include "body_stop_ball.h"
//#include "body_kick_to_relative.h"
#include "body_hold_ball2008.h"
#include "kick_table.h"

#include <rcsc/player/player_agent.h>
#include <rcsc/common/logger.h>
#include <rcsc/common/server_param.h>
#include <rcsc/geom/circle_2d.h>
#include <rcsc/geom/ray_2d.h>

#include "body_turn_to_ball.h"

#include <algorithm>

using namespace rcsc;

/*-------------------------------------------------------------------*/
/*!

*/
bool
Bhv_StraightDribble::execute( PlayerAgent * agent )
{
    // dlog.addText( Logger::KICK,
    //               __FILE__": Body_KickOneStep" );

    const WorldModel & wm = agent->world();

    if( wm.gameMode().type() != GameMode::PlayOn){
        return false;
    }

    // doDash() setDash()
    // Vector2D player_velocity = wm.self().vel();
    
    dlog.addText( Logger::TEAM,
                  __FILE__": (doStraightDribble)" );
    
    const double angle_ball_diff = ( wm.ball().angleFromSelf() - wm.self().body() ).degree();
    agent->doDash( ServerParam::i().maxDashPower(), angle_ball_diff );
    // agent->doDash( ServerParam::i().maxPower(), 0 );
    // if ( ( agent->world().ball().angleFromSelf() - agent->world().self().body() ).abs()
    //      > 3.0 )
    // {
    //     dlog.addText( Logger::ACTION,
    //               __FILE__": agent->world().ball().angleFromSelf() = %f", agent->world().ball().angleFromSelf().degree());
    //     dlog.addText( Logger::ACTION,
    //               __FILE__": agent->world().self().body() = %f", agent->world().self().body().degree()); 
    //     Body_TurnToBall().execute( agent );
    // }

    Vector2D ball_vel = wm.ball().vel();
    dlog.addText( Logger::TEAM,
                  __FILE__ ": ball_vel = (%f, %f)", ball_vel);


    // if ( ! wm.self().isKickable() )
    // {
        // std::cerr << __FILE__ << ": " << __LINE__
        //           << " not ball kickable!"
        //           << std::endl;
    //     dlog.addText( Logger::ACTION,
    //                   __FILE__":  not kickable" );
    //     return true;
    // }
    

    // Vector2D ball_vel = wm.ball().vel();

    // if ( ! wm.ball().velValid() )
    // {
    //     if ( ! M_force_mode )
    //     {
    //         dlog.addText( Logger::KICK,
    //                       __FILE__". unknown ball vel" );
    //         return Body_StopBall().execute( agent );
    //     }

    //     ball_vel.assign( 0.0, 0.0 );
    // }

    // M_first_speed = std::min( M_first_speed, ServerParam::i().ballSpeedMax() );

    // const AngleDeg target_angle = ( M_target_point - wm.ball().pos() ).th();

    // // Vector2D first_vel = get_max_possible_vel( target_angle,
    // //                                            wm.self().kickRate(),
    // //                                            ball_vel );
    // Vector2D first_vel = KickTable::calc_max_velocity( target_angle,
    //                                                    wm.self().kickRate(),
    //                                                    ball_vel );
    // double first_speed = first_vel.r();

    // if ( first_speed > M_first_speed )
    // {
    //     first_vel.setLength( M_first_speed );
    //     first_speed = M_first_speed;
    // }
    // else
    // {
    //     dlog.addText( Logger::KICK,
    //                   __FILE__": cannot get required vel. only angle adjusted" );
    // }

    // // first_vel.r() may be less than M_first_speed ...


    // const Vector2D kick_accel = first_vel - ball_vel;

    // double kick_power = kick_accel.r() / wm.self().kickRate();
    // const AngleDeg kick_dir = kick_accel.th() - wm.self().body();

    // if ( kick_power > ServerParam::i().maxPower() + 0.01 )
    // {
    //     // std::cerr << agent->config().teamName() << ' '
    //     //           << wm.self().unum() << ": "
    //     //           << wm.time() << " Body_KickOneStep unexpected reach."
    //     //           << " kick_power=" << kick_power << std::endl;
    //     if ( first_speed < 0.001 )
    //     {
    //         dlog.addText( Logger::KICK,
    //                       __FILE__": could not stop the ball completely, but try to stop. kick_power=%f",
    //                       kick_power );
    //         return Body_StopBall().execute( agent );
    //     }

    //     if ( ! M_force_mode )
    //     {
    //         dlog.addText( Logger::KICK,
    //                       __FILE__": could not stop the ball completely. hold ball. kick_power=%f" ,
    //                       kick_power );
    //         return Body_HoldBall2008( true,
    //                                   M_target_point,
    //                                   M_target_point
    //                                   ).execute( agent );
    //     }

    //     kick_power = ServerParam::i().maxPower();
    // }

    // dlog.addText( Logger::KICK,
    //               __FILE__": first_speed=%.3f, angle=%.1f, power=%.1f, dir=%.1f ",
    //               first_vel.r(), first_vel.th().degree(),
    //               kick_power, kick_dir.degree() );

    // M_ball_result_pos = wm.ball().pos() + first_vel;
    // M_ball_result_vel = first_vel * ServerParam::i().ballDecay();
    // M_kick_step = 1;
    
    std::cerr << "check1" << std::endl;
    static int t = 10000;
    std::cerr << "check2" << std::endl;
    dlog.addText( Logger::TEAM,
                  __FILE__ ": t = %d", t);
    dlog.addText( Logger::TEAM,
                  __FILE__ ": bodyAngle = %f", wm.self().body());
    std::cerr << "check3" << std::endl;
    std::cerr << "t=" << t << std::endl;
    if( t >= 3 ){
      double player_velocity = wm.self().vel().x;
      double player_maxDashAccel = wm.self().playerType().dashPowerRate() * ServerParam::i().maxDashPower();
      double player_decay = wm.self().playerType().playerDecay();
      double player_distance = player_velocity+(player_velocity*player_decay+player_maxDashAccel)+((player_velocity*player_decay+player_maxDashAccel)*player_decay+player_maxDashAccel);
      
      // Vector2D player_velocity = wm.self().vel();

      // double player_velocity = wm.self().vel().r();
      // double player_maxDashAccel = wm.self().playerType().dashPowerRate() * ServerParam::i().maxDashPower() * ServerParam::i().dashDirRate( wm.self().body().degree() );

      // double player_maxDashAccel = ServerParam::i().maxDashPower() * wm.self().playerType().dashPowerRate() * wm.self().playerType().effortMax() * ServerParam::i().dashDirRate( angle_ball_diff );
      
      // double player_decay = wm.self().playerType().playerDecay();
      // double player_distance = player_velocity + (player_velocity*player_decay+player_maxDashAccel) + ((player_velocity*player_decay+player_maxDashAccel)*player_decay+player_maxDashAccel) + wm.self().playerType().playerSize() + wm.self().kickRate();
      
      double initial_ball_vel = wm.ball().vel().r();
      // double ball_decay = ServerParam::i().ballDecay();
      double first_ball_speed = ServerParam::i().firstBallSpeed(player_distance, 3);

      double kick_power = (first_ball_speed - initial_ball_vel) / wm.self().kickRate();

      // const double angle_goal_diff = - ( wm.self().body() ).degree();
      // agent->doKick( 10, angle_goal_diff );

      const Vector2D target_point = Vector2D(52.5, 0.0);
      const AngleDeg target_angle = (target_point - wm.self().pos()).th();

      dlog.addText( Logger::TEAM,
                  __FILE__ ": kick_power = %f", kick_power);
      // if( agent->doKick( kick_power, 0 ) )
      if( agent->doKick( kick_power, target_angle ) )
      {
        dlog.addText( Logger::TEAM,
                  __FILE__ ": doKick = true", kick_power);
        t = 0;
      }
    }
    t++;
    dlog.addText( Logger::TEAM,
                  __FILE__ ": t = %d", t);
    return true;
    // return agent->doKick( kick_power, wm.self().body() );
    
}
