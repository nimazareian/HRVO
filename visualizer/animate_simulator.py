import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation, PillowWriter
from matplotlib.patches import Circle
import pandas as pd

# Constants
robot_radius = 0.09
frame_rate = 30
frame_length_ms = 1000.0 / frame_rate
play_back_speed = 0.25
div_a_field_width = 9.0
div_a_field_length = 12.0
default_robot_color = 'royalblue'
collided_robot_color = 'red'


def animate_robots(robot_pos_df, gif_output_file=None):
    num_frames = robot_pos_df['frame'].max() + 1
    num_robots = robot_pos_df['robot_id'].max() + 1
    default_line_alpha = 0.3
    is_paused = False

    # Calculate plot size
    max_robot_x_pos = robot_pos_df['x'].max()
    min_robot_x_pos = robot_pos_df['x'].min()
    max_robot_y_pos = robot_pos_df['y'].max()
    min_robot_y_pos = robot_pos_df['y'].min()
    scale_constant = 1.0 / 8
    x_offset = max(scale_constant * (max_robot_x_pos - min_robot_x_pos), 1.0)
    y_offset = max(scale_constant * (max_robot_y_pos - min_robot_y_pos), 1.0)


    def setup_plot():
        """Initial drawing of the scatter plot."""
        # TODO: Make robot and tracking line the same color
        for robot in robot_list:
            ax.add_patch(robot)

        for line in line_list:
            line.set_data([], [])

        return robot_list + line_list

    def update(frame):
        """Update plot for new frame"""
        if frame >= num_frames:
            return robot_list + line_list
            
        # Update robot positions
        for robot_id in range(len(robot_list)):
            robot = robot_pos_df[(robot_pos_df['robot_id'] == robot_id) & (robot_pos_df['frame'] == frame)]
            robot_list[robot_id].center = (float(robot['x'].head()), float(robot['y'].head()))
            # Update robot color if it collides with another robot
            if (robot['has_collided'] > 0).bool():
                robot_list[robot_id].set_facecolor(collided_robot_color)
            else:
                robot_list[robot_id].set_facecolor(default_robot_color)

        # Update lines tracking the robot movement
        robots_grouped = robot_pos_df.groupby('robot_id')
        for robot_id, line in enumerate(line_list):
            robot_df = robots_grouped.get_group(robot_id)
            line.set_data(robot_df[robot_df['frame'] <= frame]['x'], robot_df[robot_df['frame'] <= frame]['y'])

        return robot_list + line_list

    def toggle_pause(event):
        nonlocal is_paused
        if is_paused:
            robot_anim.resume()
        else:
            robot_anim.pause()
        is_paused = not is_paused

    def on_plot_hover(event):
        # Iterating over each data member plotted
        for line in line_list:
            # Searching which data member corresponds to current mouse position
            if line.contains(event)[0]:
                line.set_alpha(1.0)
            else:
                line.set_alpha(default_line_alpha)

    # set up plot
    fig = plt.figure()
    fig.suptitle('26 robots with 2.0x radius')
    ax = fig.add_subplot(111, autoscale_on=False, xlim=(min_robot_x_pos - x_offset, max_robot_x_pos + x_offset), 
                                                  ylim=(min_robot_y_pos - y_offset, max_robot_y_pos + y_offset))
    ax.set_aspect('equal')
    ax.grid()
    line, = ax.plot([], [], '--r')

    # set-up robot circles
    robot_list = []
    line_list = []
    for _ in range(num_robots):
        robot = Circle((0, 0), robot_radius, facecolor='aqua', edgecolor='black')
        robot_list.append(robot)
        lobj = ax.plot([], [], linestyle='--', alpha=default_line_alpha)[0]
        line_list.append(lobj)

    # Start/Stop on click
    fig.canvas.mpl_connect('button_press_event', toggle_pause)

    # TODO: Allow highlighting after animation is over
    # Highlight tracked line on hover
    fig.canvas.mpl_connect('motion_notify_event', on_plot_hover)

    # Animate
    print(f'Animating robots from {file_location}')
    robot_anim = FuncAnimation(fig, func=update, interval=frame_length_ms / play_back_speed,
                               init_func=setup_plot, frames=num_frames, blit=True, repeat=False)
    plt.show()

    if gif_output_file is not None:
        # save gif
        robot_anim.save(gif_output_file, writer=PillowWriter(fps=frame_rate))
        print(f'Saved gif of robots to {gif_output_file}')


file_name = '2_robots_2.0_rad'
file_location = f'visualizer/data/{file_name}.csv'
df = pd.read_csv(file_location, skiprows=3)
animate_robots(df, f'visualizer/gif/{file_name}.gif')
