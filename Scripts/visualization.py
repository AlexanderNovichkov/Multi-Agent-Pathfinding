import tkinter as tk
import sys
import xml.etree.ElementTree as ET
from utils import Point, Map, Agent, AgentAction, Trajectory, Parser, get_duration

class Application(tk.Frame):
    DEFAULT_WIDTH = 700
    DEFAULT_HEIGHT = 700
    FRAME_DURATION = 30
    AGENT_RADIUS = 1 / (2 ** (1 / 2))

    def __init__(self, master, world_map: Map, agents_trajectories):
        super().__init__(master)
        self.master = master
        self.pack(expand=True, fill="both")
        self.map = world_map
        self.agents_trajectories: list[Trajectory] = agents_trajectories
        self.duration = get_duration(agents_trajectories)

        self.cur_time = tk.DoubleVar()
        self.time_per_frame = 0.06
        self.play = True
        self.slider_saved = True
        self.path_hidden = True

        self.create_widgets()

        # Need to calculate canvas coordinates for new items after scaling
        self.transformation_rect = self.canvas.create_rectangle(0, 0, 1, 1, state='hidden')

        self.canvas.create_rectangle(0, 0, world_map.width, world_map.height, fill='white')
        self.create_grid(world_map.width, world_map.height)
        self.draw_map()

        self.init_agents()
        self.draw_trajectory()

        zoom_weight = min((self.DEFAULT_WIDTH - 3) / world_map.width, (self.DEFAULT_HEIGHT - 3) / world_map.height)
        self.canvas.scale("all", 0, 0, zoom_weight, zoom_weight)
        self.canvas.configure(scrollregion=self.canvas.bbox("all"))

    def create_widgets(self):
        self.grid_columnconfigure(0, weight=1)
        self.grid_rowconfigure(0, weight=1)
        # create canvas
        self.canvas = tk.Canvas(self, width=self.DEFAULT_WIDTH, height=self.DEFAULT_HEIGHT, bg='black')
        self.canvas.grid(row=0, column=0, sticky='nsew')

        # create slider
        self.slider = tk.Scale(self, from_=0, to=self.duration, orient=tk.HORIZONTAL,
                               resolution=0.01, variable=self.cur_time)
        self.slider.grid(row=1, column=0, sticky='nsew')
        self.slider.bind("<Button-1>", self.slider_press)
        self.slider.bind("<ButtonRelease-1>", self.slider_release)

        # Move canvas using mouse
        self.canvas.bind("<ButtonPress-1>", self.move_start)
        self.canvas.bind("<B1-Motion>", self.move_move)

        # linux zoom
        self.canvas.bind("<Button-4>", lambda event: self.zoom(event.x, event.y, 1.1))
        self.canvas.bind("<Button-5>", lambda event: self.zoom(event.x, event.y, 0.9))
        # windows/mac zoom
        self.canvas.bind("<MouseWheel>",
                         lambda event: self.zoom(event.x, event.y, 1 + (event.delta > 0) / 10 - (event.delta < 0) / 10))

        # create options
        self.options = tk.Frame(self)
        self.options.grid(row=0, column=1, sticky='nsew', rowspan=2)

        self.start_button = tk.Button(self.options, text='Start / Stop', width=14)
        self.start_button.grid(row=0, column=0, columnspan=2)
        self.start_button.bind("<Button-1>", self.start_stop)
        self.bind_all("<space>", self.start_stop)

        self.slower_button = tk.Button(self.options, text="Slower", width=5)
        self.slower_button.grid(row=2, column=0, sticky='E')
        self.slower_button.bind("<Button-1>", self.make_slower)

        self.faster_button = tk.Button(self.options, text="Faster", width=5)
        self.faster_button.grid(row=2, column=1)
        self.faster_button.bind("<Button-1>", self.make_faster)

        self.show_trajectory_button = tk.Button(self.options, text="Show / Hide path", width=14)
        self.show_trajectory_button.grid(row=3, column=0, columnspan=2)
        self.show_trajectory_button.bind("<Button-1>", self.show_hide_trajectory)

    def init_agents(self):
        offset = (1 - Application.AGENT_RADIUS) / 2
        self.canvas_agents = []
        for i in range(0, len(self.map.agents)):
            x0 = self.map.agents[i].start.x
            y0 = self.map.agents[i].start.y
            self.canvas_agents.append(
                self.canvas.create_oval(x0 + offset, y0 + offset, x0 + 1 - offset, y0 + 1 - offset, fill='red')
            )

    def draw_trajectory(self):
        for i in range(0, len(self.map.agents)):
            if len(self.agents_trajectories[i]) == 0:
                continue
            trajectory = self.agents_trajectories[i]
            start_pos = self.map.agents[i].start
            for t in range(1, trajectory[-1].start_time + 2):
                point_from = trajectory.get_position(t - 1, start_pos)
                point_to = trajectory.get_position(t, start_pos)
                self.canvas.create_line(point_from.x + 0.5, point_from.y + 0.5, point_to.x + 0.5, point_to.y + 0.5,
                                        arrow=tk.LAST, state='hidden', tag="trajectory")

    def slider_press(self, event):
        self.slider_saved = self.play
        self.play = False

    def slider_release(self, event):
        self.play = self.slider_saved

    def start_stop(self, event):
        self.play = not self.play
        self.slider_saved = self.play

    def make_slower(self, event):
        self.time_per_frame /= 1.5

    def make_faster(self, event):
        self.time_per_frame *= 1.5

    def move_start(self, event):
        self.canvas.scan_mark(event.x, event.y)

    def move_move(self, event):
        self.canvas.scan_dragto(event.x, event.y, gain=1)

    def show_hide_trajectory(self, event):
        self.path_hidden = not self.path_hidden
        self.canvas.itemconfig("trajectory", state='hidden' if self.path_hidden else 'normal')

    def zoom(self, x, y, weight):
        canvas_x = self.canvas.canvasx(x)
        canvas_y = self.canvas.canvasy(y)
        self.canvas.scale("all", canvas_x, canvas_y, weight, weight)
        self.canvas.configure(scrollregion=self.canvas.bbox("all"))

    def get_canvas_coords(self, x, y):
        trasform = self.canvas.coords(self.transformation_rect)
        return trasform[0] + (trasform[2] - trasform[0]) * x, \
               trasform[1] + (trasform[3] - trasform[1]) * y

    def create_grid(self, max_x, max_y):
        for x in range(0, max_x + 1):
            self.canvas.create_line(x, 0, x, max_y)

        for y in range(0, max_y + 1):
            self.canvas.create_line(0, y, max_x, y)

    def update_frame(self):
        self.after(self.FRAME_DURATION, self.update_frame)

        offset = (1 - Application.AGENT_RADIUS) / 2
        self.cur_time.set(self.cur_time.get() + self.time_per_frame * self.play)
        for i in range(0, len(self.map.agents)):
            trajectory = self.agents_trajectories[i]

            new_agent_pos = trajectory.get_position(self.cur_time.get(), self.map.agents[i].start)
            new_x, new_y = self.get_canvas_coords(new_agent_pos.x + offset, new_agent_pos.y + offset)
            old_x, old_y, _, _ = self.canvas.coords(self.canvas_agents[i])
            self.canvas.move(self.canvas_agents[i], new_x - old_x, new_y - old_y)

            if new_agent_pos == self.map.agents[i].goal:
                color = "green"
            elif new_agent_pos == self.map.agents[i].start:
                color = "red"
            else:
                color = 'blue'
            self.canvas.itemconfig(self.canvas_agents[i], fill=color)

    def draw_map(self):
        for x in range(0, self.map.width):
            for y in range(0, self.map.height):
                if self.map.grid[y][x]:
                    self.canvas.create_rectangle(x, y, x + 1, y + 1, fill='gray')


def main():
    if len(sys.argv) < 2:
        print("Need file name of log file as a first argument")
        exit(1)
    xml_file_name = sys.argv[1]

    parser = Parser(xml_file_name)
    world_map = parser.get_map()
    agents_trajectories = parser.get_trajectories()

    root = tk.Tk()
    app = Application(root, world_map, agents_trajectories)
    root.minsize(600, 400)
    app.after(0, app.update_frame)
    app.mainloop()


if __name__ == '__main__':
    main()
