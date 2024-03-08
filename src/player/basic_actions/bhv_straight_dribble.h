// -*-c++-*-

/*!
  \file bhv_straight_dribble.h
  \brief straight dribble behavior.
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

#ifndef BHV_STRAIGHT_DRIBBLE_H
#define BHV_STRAIGHT_DRIBBLE_H

#include <rcsc/player/soccer_action.h>
#include <rcsc/geom/vector_2d.h>

class Bhv_StraightDribble
    : public rcsc::BodyAction {
private:
    //! target point where ball should readh or pass through
    rcsc::Vector2D M_target_point;
    //! ball first speed when ball is released
    double M_first_speed;

    //! force mode flag
    bool M_force_mode;

    //! result ball position
    rcsc::Vector2D M_ball_result_pos;
    //! result ball velocity
    rcsc::Vector2D M_ball_result_vel;
    //! estimated kick step
    int M_kick_step;
public:
    /*!
      \brief construct with all parameters
      \param target_point global coordinate of target poisition
      \param first_speed ball first speed when ball is released
      \param force_mode enforce to kick out
    */
    Bhv_StraightDribble()
      { }

    /*!
      \brief execute action
      \param agent pointer to the agent itself
      \return true if action is performed
    */
    bool execute( rcsc::PlayerAgent * agent );

    /*!
      \brief get the result ball position
      \return ball position after kick
     */
    const rcsc::Vector2D & ballResultPos() const
      {
          return M_ball_result_pos;
      }

    /*!
      \brief get the result ball velocity
      \return ball velocity after kick
     */
    const rcsc::Vector2D & ballResultVel() const
      {
          return M_ball_result_vel;
      }


    /*!
      \brief calculate possible velocity by one kick.
      NOTE: ball info may be future estimation result.
      \param target_angle kick target global angle
      \param kick_rate current kick rate
      \param ball_vel current ball velocity
    */
    // static
    // rcsc::Vector2D get_max_possible_vel( const AngleDeg & target_angle,
    //                                      const double & kick_rate,
    //                                      const Vector2D & ball_vel );

};

#endif
