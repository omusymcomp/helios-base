// -*-c++-*-

#ifndef BHV_STRAIGH_DRIBBLE_H
#define BHV_STRAIGH_DRIBBLE_H

#include <rcsc/player/soccer_action.h>


class Bhv_StraighDribble
    : public rcsc::SoccerBehavior {
private:

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
};

#endif
