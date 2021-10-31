/*
 * Circle.cpp
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
 * \file   Circle.cpp
 * \brief  Example with 250 agents navigating through a circular environment.
 */

#ifndef HRVO_OUTPUT_TIME_AND_POSITIONS
#define HRVO_OUTPUT_TIME_AND_POSITIONS 1
#endif

#include <cmath>

#if HRVO_OUTPUT_TIME_AND_POSITIONS
#include <iostream>
#endif

#include <HRVO.h>
#include <iostream>

using namespace hrvo;

const float HRVO_TWO_PI = 6.283185307179586f;
const int NUM_ROBOTS = 20;

int main()
{
	Simulator simulator;

	simulator.setTimeStep(1.f/30);
	simulator.setAgentDefaults(9.f, 10, 0.09f * 1.1f, 0.09f * 1.1f, 2.0f, 2.0f);

//	for (std::size_t i = 0; i < NUM_ROBOTS; ++i) {
//		const Vector2 position = 20.0f * Vector2(std::cos(0.004f * i * HRVO_TWO_PI), std::sin(0.004f * i * HRVO_TWO_PI));
//		simulator.addAgent(position, simulator.addGoal(-position));
//	}
    float robot_starting_angle_dif = HRVO_TWO_PI / NUM_ROBOTS;
    float circle_radius = 2.f;
    for (std::size_t i = 0; i < NUM_ROBOTS; ++i) {
		const Vector2 position = circle_radius * Vector2(std::cos(i * robot_starting_angle_dif), std::sin(i * robot_starting_angle_dif));
		simulator.addAgent(position, simulator.addGoal(-position));
	}

    // Column name
    for (int robot_id = 0; robot_id < NUM_ROBOTS; ++robot_id) {
        std::cout << "robot " << robot_id;// << " x" << ",robot " << robot_id << " y";
        if (robot_id != NUM_ROBOTS - 1)
        {
            std::cout << ",";
        }
    }
    std::cout << std::endl;

    // Position Data
//    int curr_frame = 0;
	do {
//#if HRVO_OUTPUT_TIME_AND_POSITIONS
//		std::cout << simulator.getGlobalTime();
//        std::cout << curr_frame;
//        curr_frame++;

        for (int j = 0; j < 2; j++)
        {
            for (std::size_t i = 0; i < simulator.getNumAgents(); ++i)
            {
                Vector2 curr_robot_pos = simulator.getAgentPosition(i);
                if (j == 0)
                {
                    std::cout << curr_robot_pos.getX();
                }
                else
                {
                    std::cout << curr_robot_pos.getY();
                }

                if (i != simulator.getNumAgents() - 1)
                {
                    std::cout << ",";
                }
            }
            std::cout << std::endl;
        }

//#endif /* HRVO_OUTPUT_TIME_AND_POSITIONS */

		simulator.doStep();
	}
	while (!simulator.haveReachedGoals());

	return 0;
}
