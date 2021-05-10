import xml.etree.ElementTree as ET

class Point:
    def __init__(self, x: int = None, y: int = None):
        self.x = x
        self.y = y

    def __sub__(self, other):
        return Point(self.x - other.x, self.y - other.y)

    def __add__(self, other):
        return Point(other.x + self.x, other.y + self.y)

    def __mul__(self, coef):
        return Point(self.x * coef, self.y * coef)

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y


class Agent:
    def __init__(self):
        self.start: Point = Point()
        self.goal: Point = Point()


class Map:
    def __init__(self):
        self.width = None
        self.height = None
        self.grid: list[list] = []
        self.agents: list[Agent] = []


class AgentAction:
    def __init__(self):
        self.start_time = None
        self.goal = Point()


class Trajectory(list[AgentAction]):
    def __init__(self):
        super().__init__()

    def get_position(self, time, start_position) -> Point:
        if len(self) == 0 or time < self[0].start_time:
            return start_position

        left = 0
        right = len(self)
        while right-left > 1:
            mid = (left + right)//2
            if self[mid].start_time <= time:
                left = mid
            else:
                right = mid

        ind = left
        # ind = -1
        # for action in self:
        #     if action.start_time > time:
        #         break
        #     ind += 1

        pos_from = start_position if ind == 0 else self[ind - 1].goal
        pos_to = self[ind].goal
        travel_time = time - self[ind].start_time

        if travel_time >= 1:
            return pos_to
        else:
            return pos_from + (pos_to - pos_from) * travel_time


class Parser:
    def __init__(self, xml_file_name):
        self.xml_file_name = xml_file_name

    def get_map(self):
        tree = ET.parse(self.xml_file_name)
        root = tree.getroot()

        world_map = Map()

        map_section = root.find('map')
        world_map.width = int(map_section.find('width').text)
        world_map.height = int(map_section.find('height').text)

        grid_section = map_section.find('grid')
        for row in grid_section:
            world_map.grid.append(list(map(int, row.text.split())))

        agents_section = map_section.find('agents')
        for agent_data in agents_section:
            agent = Agent()
            agent.start.x = int(agent_data.get('start.x'))
            agent.start.y = int(agent_data.get('start.y'))
            agent.goal.x = int(agent_data.get('finish.x'))
            agent.goal.y = int(agent_data.get('finish.y'))
            world_map.agents.append(agent)

        return world_map

    def get_trajectories(self) -> list[Trajectory]:
        tree = ET.parse(self.xml_file_name)
        root = tree.getroot()

        agents_trajectories = []

        log_section = root.find('log')
        for agent_section in log_section.iterfind('agent'):
            trajectory_section = agent_section.find('trajectory')
            trajectory = Trajectory()
            for action_section in trajectory_section:
                action = AgentAction()
                action.start_time = int(action_section.get('start_time'))
                action.goal.x = int(action_section.get('finish.x'))
                action.goal.y = int(action_section.get('finish.y'))
                trajectory.append(action)
            agents_trajectories.append(trajectory)

        return agents_trajectories


def get_duration(agents_trajectories: list[Trajectory]):
    duration = 0
    for trajectory in agents_trajectories:
        if len(trajectory) > 0:
            duration = max(duration, trajectory[-1].start_time + 1)
    return duration

