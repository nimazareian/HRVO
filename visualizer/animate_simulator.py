import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation, PillowWriter
from matplotlib.patches import Circle
import pandas as pd

# Constants
robot_radius = 0.09
frame_length_ms = 1000 / 30
play_back_speed = 1.0
div_a_field_width = 9.0
div_a_field_length = 12.0


def animate_robots(robot_pos_x_df, robot_pos_y_df, gif_output_file=None):
    num_frames = len(robot_pos_x_df.index)
    num_robots = len(robot_pos_x_df.columns)

    def setup_plot():
        """Initial drawing of the scatter plot."""
        ax.axis([-3, 3, -3, 3])

        for robot in robot_list:
            ax.add_patch(robot)

        for line in line_list:
            line.set_data([], [])

        return robot_list + line_list

    def update(i):
        """Update plot for new frame"""
        # Update robot positions
        for j in range(len(robot_list)):
            robot_list[j].center = (robot_pos_x_df.iat[i, j], robot_pos_y_df.iat[i, j])

        # Update lines tracking the robot movement
        for robot_id, line in enumerate(line_list):
            line.set_data(robot_pos_x_df.iloc[:i, robot_id], robot_pos_y_df.iloc[:i, robot_id])

        return robot_list + line_list

    # set up plot
    fig = plt.figure()
    ax = fig.add_subplot(111, autoscale_on=False, xlim=(-3, 3), ylim=(-3, 3))
    ax.set_aspect('equal')
    ax.grid()
    line, = ax.plot([], [], '--r')

    # set-up robot circles
    robot_list = []
    line_list = []
    for _ in range(num_robots):
        robot = Circle((0, 0), robot_radius, facecolor='aqua', edgecolor='black')
        robot_list.append(robot)
        lobj = ax.plot([], [], linestyle='--', alpha=0.5)[0]
        line_list.append(lobj)

    # Animate
    robot_anim = FuncAnimation(fig, func=update, interval=frame_length_ms / play_back_speed,
                               init_func=setup_plot, frames=num_frames, blit=True)
    plt.show()

    if gif_output_file is not None:
        # save gif
        robot_anim.save(gif_output_file, writer=PillowWriter(fps=30))


robot_pos_path = '../robot_pos_2.2.csv'
df = pd.read_csv(robot_pos_path, skiprows=3)
x_df = df.iloc[::2, :]
y_df = df.iloc[1::2, :]
animate_robots(x_df, y_df, 'robot_anim_2.2.gif')
