// -*-c++-*-

/*
 *Copyright:

 Copyright (C) Hidehisa AKIYAMA

 This code is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3, or (at your option)
 any later version.

 This code is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this code; see the file COPYING.  If not, write to
 the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

 *EndCopyright:
 */

/////////////////////////////////////////////////////////////////////

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "bhv_straight_dribble.h"

#include "basic_actions/basic_actions.h"

#include <rcsc/player/player_agent.h>
#include <rcsc/player/debug_client.h>
#include <rcsc/player/intercept_table.h>
#include <rcsc/common/logger.h>
#include <rcsc/common/server_param.h>

#include "neck_offensive_intercept_neck.h"

using namespace rcsc;

/*-------------------------------------------------------------------*/
/*!

 */
bool
Bhv_StraightDribble::execute( PlayerAgent * agent )
{   
    const ServerParam & SP = ServerParam::i();
    const WorldModel & wm = agent->world();

    Vector2D player_pos = wm.self().pos();
    Vector2D player_vel = wm.self().vel();
    Vector2D ball_vel = wm.ball().vel();
    const Vector2D goal( SP.pitchHalfLength(), 0.0);
    AngleDeg goal_angle = ( goal - wm.self().pos() ).th();
    AngleDeg dash_angle = (wm.ball().pos() - wm.self().pos()).th();
    double accel_mag = wm.self().dashRate() * SP.dashDirRate( dash_angle.degree() ) * SP.maxDashPower();
    Vector2D dash_accel = Vector2D::polar2vector( accel_mag, dash_angle );

    static int s_t = 0;
    constexpr double kickable_area_rate = 0.1;
    constexpr int dash_cycle = 4;

    Vector2D reach_player_pos = player_pos + player_vel;

    for ( int i = 0; i < dash_cycle; i++)
    {
        player_vel = player_vel * wm.self().playerType().playerDecay() + dash_accel;
        reach_player_pos = reach_player_pos + player_vel;
    }

    reach_player_pos.x = reach_player_pos.x + wm.self().playerType().playerSize() + ServerParam::i().ballSize() + wm.self().playerType().kickableArea() * kickable_area_rate;
    reach_player_pos.y = reach_player_pos.y + wm.self().playerType().playerSize() + ServerParam::i().ballSize() + wm.self().playerType().kickableArea() * kickable_area_rate;

    double first_ball_mag = SP.firstBallSpeed( wm.ball().pos().dist(reach_player_pos),
                                                dash_cycle + 1 
    );
    
    Vector2D first_ball_vel = Vector2D::polar2vector( first_ball_mag, goal_angle ); 

    Vector2D ball_accel = first_ball_vel - ball_vel;

    double power = ball_accel.r() / wm.self().kickRate();

    // for ( int i = 0; i < dash_cycle; i++ )
    // {
    //     player_vel = player_vel * wm.self().playerType().playerDecay() + dash_vel;
    //     reach_player_pos = reach_player_pos + player_vel
    // }

    // reach_player_pos = reach_player_pos + wm.self().playerType().playerSize() + ServerParam::i().ballSize() + wm.self().playerType().kickableArea() * kickable_area_rate;

    // AngleDeg dash_angle = (wm.ball().pos() - wm.self().pos()).th();

    // double power = ball_accel / wm.self().kickRate();

    std::cerr << "Server Practice : ["
                << s_t
                << "]"
                << std::endl;

    dlog.addText( Logger::WORLD,
                "Server Practice=(%lf)",
                s_t);

    if ( s_t > 0 )
    {            
        if ( s_t % dash_cycle == 0)
        {
            if ( agent->doKick( power, goal_angle ) )
            {
                s_t++;
                return true;
            }
            else
            {
                agent->doDash( SP.maxDashPower(), dash_angle );
                return true;
            }
        }

        agent->doDash( SP.maxDashPower(), dash_angle );
        s_t++;
        return true;
    }

    agent->doDash( SP.maxDashPower(), dash_angle );

    if ( agent->doKick( power, goal_angle ) )
    {   
        s_t++;
        return true;  
    }   
    return true;                     
}