import sys
import xml.etree.ElementTree as ET
import copy
import random


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
    def __init__(self, start, goal):
        self.start: Point = start
        self.goal: Point = goal


def generate_free_point(grid: list[list]):
    for iteration in range(0, 1000):
        y = random.randint(0, len(grid) - 1)
        x = random.randint(0, len(grid[0]) - 1)
        if grid[y][x] == 0:
            return Point(x, y)
    return None


def generate_agents(grid: list[list], cnt):
    mod_grid = copy.deepcopy(grid)
    agents: list[Agent] = []
    for i in range(0, cnt):
        start = generate_free_point(mod_grid)
        if start is None:
            return None
        mod_grid[start.y][start.x] = 1
        goal = generate_free_point(mod_grid)
        if goal is None:
            return None
        mod_grid[goal.y][goal.x] = 1
        agents.append(Agent(start, goal))

    return agents


def write(file_name, tree: ET.ElementTree, agents):
    mod_tree = copy.deepcopy(tree)
    root = mod_tree.getroot()
    map_section = root.find('map')

    agents_section = map_section.find('agents')
    if agents_section is not None:
        map_section.remove(agents_section)
    agents_section = ET.Element("agents")
    map_section.insert(2, agents_section)

    for agent in agents:
        ET.SubElement(agents_section, "agentdata", {
            "start.x": str(agent.start.x),
            "start.y": str(agent.start.y),
            "finish.x": str(agent.goal.x),
            "finish.y": str(agent.goal.y),
        })

    mod_tree.write(file_name)

def main():
    if len(sys.argv) < 8:
        print("Not enough arguments!")
        exit(-1)
    map_name = sys.argv[1]
    path_to_xml_with_map = sys.argv[2]
    path_to_result_dir = sys.argv[3]
    start_agents_cnt = int(sys.argv[4])
    stop_agents_cnt = int(sys.argv[5])
    step_agents_cnt = int(sys.argv[6])
    generate_cnt = int(sys.argv[7])

    # parse map
    tree = ET.parse(path_to_xml_with_map)
    root = tree.getroot()
    map_section = root.find('map')
    grid = []
    grid_section = map_section.find('grid')
    for row in grid_section:
        grid.append(list(map(int, row.text.split())))

    # generate
    for agents_cnt in range(start_agents_cnt, stop_agents_cnt, step_agents_cnt):
        for iteration in range(0, generate_cnt):
            agents = generate_agents(grid, agents_cnt)
            if agents == None:
                print(f"Can not generate {agents_cnt} agents!")
                exit(-1)
            write(f"{path_to_result_dir}/{map_name}_{agents_cnt}_{iteration}.xml", tree, agents)


if __name__ == '__main__':
    main()
