/*
 * Goal.h
 * HRVO Library
 *
 * Copyright 2009 University of North Carolina at Chapel Hill
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Please send all bug reports to <geom@cs.unc.edu>.
 *
 * The authors may be contacted via:
 *
 * Jamie Snape, Jur van den Berg, Stephen J. Guy, and Dinesh Manocha
 * Dept. of Computer Science
 * 201 S. Columbia St.
 * Frederick P. Brooks, Jr. Computer Science Bldg.
 * Chapel Hill, N.C. 27599-3175
 * United States of America
 *
 * <https://gamma.cs.unc.edu/HRVO/>
 */

/**
 * \file   Goal.h
 * \brief  Declares the Goal class.
 */

#ifndef HRVO_GOAL_H_
#define HRVO_GOAL_H_

#include <vector>
#include "Vector2.h"

namespace hrvo {
	class Simulator;

	/**
	 * \class  Goal
	 * \brief  A goal in the simulation.
	 */
	class Goal {
	private:
		/**
		 * \brief      Constructor.
		 * \param[in]  position  The position of this goal.
		 */
		explicit Goal(const Vector2 &position);
		explicit Goal(std::vector<Vector2> positions);

    public:
		Vector2 position_;
        // Goal positions in order Could be Queue
        std::vector<Vector2> positions_;
        int currGoalIndex = 0;

        Vector2 getNextGoalPostion();
        Vector2 getCurrentGoalPosition();
        bool isGoingToFinalGoal();


		friend class Agent;
		friend class Simulator;
	};
}

#endif /* HRVO_GOAL_H_ */
