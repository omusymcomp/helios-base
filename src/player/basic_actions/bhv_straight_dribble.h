// -*-c++-*-

#ifndef BHV_STRAIGHT_DRIBBLE_H
#define BHV_STRAIGHT_DRIBBLE_H

#include <rcsc/player/soccer_action.h>
#include <rcsc/geom/vector_2d.h>

class Bhv_StraighDribble
    : public rcsc::SoccerBehavior {
public:
    /*!
      \brief accessible from global.
     */
    Bhv_StraighDribble()
      { }

    /*!
      \brief execute action
      \param agent pointer to the agent itself
      \return true if action is performed
     */
    bool execute( rcsc::PlayerAgent* agent );

private:

    rcsc::Vector2D get_player_move_pos( const rcsc::PlayerAgent* agent,
                                        const int dash_cycle );

    rcsc::Vector2D get_player_to_ball_margin( const rcsc::PlayerAgent * agent,
                                              const rcsc::Vector2D & after_player_pos,
                                              const double kickable_area_rate );

    void debug_ball_move( const rcsc::Vector2D & ball_pos,
                          const rcsc::Vector2D & ball_vel,
                          const int dash_cycle );

};

#endif
