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

#include <cmath>

#include <HRVO.h>
#include <iostream>

using namespace hrvo;

const float HRVO_TWO_PI = 6.283185307179586f;
const int NUM_ROBOTS = 4;
const float ROBOT_RADIUS = 0.09f;
const float RADIUS_SCALE = 1.0f;

int main()
{
	Simulator simulator;

	simulator.setTimeStep(1.f/30);
	simulator.setAgentDefaults(1.f, 10, ROBOT_RADIUS * RADIUS_SCALE, ROBOT_RADIUS * RADIUS_SCALE, /*prefSpeed=*/3.5f, /*maxSpeed=*/4.825f, /*uncertaintyOffset=*/0.f, /*maxAccel=*/3.28f);

    // TODO: Can use Agent.SetAgentRadius to set custom radius for robots.
    //       Could have a randomly assigned radius with in a range

    /** Add robots around circle with one in the center **/
    // float robot_starting_angle_dif = HRVO_TWO_PI / NUM_ROBOTS;
    // float circle_radius = std::max(float(NUM_ROBOTS) / 10, 2.f);
    // simulator.addAgent(Vector2(0.f, 0.f), simulator.addGoal(Vector2(0.f, 0.f)));
    // for (std::size_t i = 0; i < NUM_ROBOTS; ++i) {
	// 	const Vector2 position = circle_radius * Vector2(std::cos(i * robot_starting_angle_dif), std::sin(i * robot_starting_angle_dif));
	// 	simulator.addAgent(position, simulator.addGoal(-position));
	// }

    /** Add robots in a vertical line where they all have to move down **/
    // const Vector2 goal_offset = Vector2(0.f, -6.f);
    // const Vector2 robot_offset = Vector2(0.f, -ROBOT_RADIUS * 2.5f);
    // for (std::size_t i = 0; i < NUM_ROBOTS; ++i) {
	// 	const Vector2 position = static_cast<float>(i) * robot_offset;
	// 	simulator.addAgent(position, simulator.addGoal(position + goal_offset));
	// }


    /** Add robots in a rectangle (recreating div-B field) **/
    float field_width = 9.f;
    float field_height = 6.f;
    float robot_offsets = 2.5 * ROBOT_RADIUS * RADIUS_SCALE;

    // Setup robots    
    const Vector2 goal_offset = Vector2(8.f, 0);
    const Vector2 robot_offset = Vector2(0.f, -ROBOT_RADIUS * 2.5f);
    for (std::size_t i = 0; i < NUM_ROBOTS; ++i) {
		const Vector2 position = -1*(goal_offset / 2) + Vector2(0.f, 2.8f) + (static_cast<float>(i) * robot_offset);
		simulator.addAgent(position, simulator.addGoal(position + goal_offset));
	}

    // Horizontal field lines
    for (float x = -(field_width / 2); x <= (field_width / 2); x += robot_offsets)
    {
        for (float y : {-(field_height / 2), field_height / 2})
        {
            const Vector2 position(x, y);
            simulator.addAgent(position, simulator.addGoal(position));
            
        }
    }

    // Vertical field lines
    float max_y = (field_height / 2) - robot_offsets;
    for (float y = -max_y; y <= max_y; y += robot_offsets)
    {
        for (float x : {-(field_width / 2), field_width / 2})
        {
            const Vector2 position(x, y);
            simulator.addAgent(position, simulator.addGoal(position));
        }
    }

    // Column Names
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
            int has_collided = -1;
            for (unsigned int other_robot_id = 0; other_robot_id < num_robots; other_robot_id++)
            {
                Vector2 other_robot_pos = simulator.getAgentPosition(other_robot_id);
                float other_robot_rad = ROBOT_RADIUS; 
                if (robot_id != other_robot_id)
                {
                    if (absSq(curr_robot_pos - other_robot_pos) < std::pow(curr_robot_rad + other_robot_rad, 2.f))
                    {
                        has_collided = other_robot_id;
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
            
            std::cout << frame << "," << time << "," << robot_id << "," << curr_robot_pos.getX() << "," << curr_robot_pos.getY() << "," << velocity_x << "," << velocity_y << "," << speed << "," << has_collided << std::endl;
        }
        frame++;
        prev_frame_time = time;
		simulator.doStep();
	}
	while (prev_frame_time < 10.f); //!simulator.haveReachedGoals());

	return 0;
}
