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
const int NUM_ROBOTS = 5;
const float ROBOT_RADIUS = 0.09f;
const float RADIUS_SCALE = 2.0f;

int main()
{
	Simulator simulator;

	simulator.setTimeStep(1.f/30);
	simulator.setAgentDefaults(1.f, 10, ROBOT_RADIUS * RADIUS_SCALE, ROBOT_RADIUS * RADIUS_SCALE, 2.0f, 2.0f);

    /** Add robots around circle with one in the center **/
    // float robot_starting_angle_dif = HRVO_TWO_PI / NUM_ROBOTS;
    // float circle_radius = std::max(float(NUM_ROBOTS) / 10, 2.f);
    // simulator.addAgent(Vector2(0.f, 0.f), simulator.addGoal(Vector2(0.f, 0.f)));
    // for (std::size_t i = 0; i < NUM_ROBOTS; ++i) {
	// 	const Vector2 position = circle_radius * Vector2(std::cos(i * robot_starting_angle_dif), std::sin(i * robot_starting_angle_dif));
	// 	simulator.addAgent(position, simulator.addGoal(-position));
	// }

    /** Add robots in a vertical line where they all have to move down **/
    const Vector2 goal_offset = Vector2(0.f, -4.f);
    for (std::size_t i = 0; i < NUM_ROBOTS; ++i) {
		const Vector2 position = Vector2(0.f, static_cast<float>(i) / 5);
		simulator.addAgent(position, simulator.addGoal(position + goal_offset));
	}

    // Column Names
    // TODO: Can add velocity_x,velocity_y,speed columns
    std::cout << "frame,time,robot_id,x,y,velocity_x,velocity_y,speed,has_collided" << std::endl;

    const unsigned int num_robots = simulator.getNumAgents();
    std::vector<float> prev_x_pos_arr(num_robots);
    std::vector<float> prev_y_pos_arr(num_robots);
    float prev_frame_time = 0.f;
    unsigned int frame = 0;
	do {
		float time = simulator.getGlobalTime();
        for (unsigned int robot_id = 0; robot_id < num_robots; robot_id++)
        {
            Vector2 curr_robot_pos = simulator.getAgentPosition(robot_id);
            float curr_robot_rad = ROBOT_RADIUS;

            // Check for collision with other robots
            bool has_collided = false;
            for (unsigned int other_robot_id = 0; other_robot_id < num_robots; other_robot_id++)
            {
                Vector2 other_robot_pos = simulator.getAgentPosition(other_robot_id);
                float other_robot_rad = ROBOT_RADIUS; 
                if (robot_id != other_robot_id)
                {
                    if (absSq(curr_robot_pos - other_robot_pos) < std::pow(curr_robot_rad + other_robot_rad, 2.f))
                    {
                        has_collided = true;
                        break;
                    } 
                }
                
            }
            
            // Velocity and Speed measured in m/s
            float velocity_x = 0.f;
            float velocity_y = 0.f;
            float speed = 0.f;
            float delta_time = time - prev_frame_time;
            // Get current robots velocity
            if (frame != 0 && delta_time != 0)
            {
                float prev_x_pos = prev_x_pos_arr[robot_id];
                float prev_y_pos = prev_y_pos_arr[robot_id];

                float curr_x_pos = curr_robot_pos.getX();
                float curr_y_pos = curr_robot_pos.getY();

                velocity_x = (curr_x_pos - prev_x_pos) / delta_time;
                velocity_y = (curr_y_pos - prev_y_pos) / delta_time;
                speed = std::pow(std::pow(velocity_x, 2.f) + std::pow(velocity_y, 2.f), 0.5f);

                // update previous robot x and y position  
                prev_x_pos_arr[robot_id] = curr_x_pos;
                prev_y_pos_arr[robot_id] = curr_y_pos;
            }
            
            std::cout << frame << "," << time << "," << robot_id << "," << curr_robot_pos.getX() << "," << curr_robot_pos.getY() << "," << velocity_x << "," << velocity_y << "," << speed << "," << int(has_collided) << std::endl;
        }
        frame++;
        prev_frame_time = time;
		simulator.doStep();
	}
	while (!simulator.haveReachedGoals());

	return 0;
}
