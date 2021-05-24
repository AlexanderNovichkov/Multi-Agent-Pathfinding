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


def generate_free_point(grid: list[list], mapf_should_find_solution):
    height = len(grid)
    width = len(grid[0])

    for iteration in range(0, 1000):
        y = random.randint(0, height - 1)
        x = random.randint(0, width - 1)
        fl = 0
        if mapf_should_find_solution:
            for x1 in range(x-1, x+2):
                for y1 in range(y-1, y+2):
                    if 0 <= x1 < width and 0 <= y1 < height and grid[y1][x1] == 1:
                        fl = 1
                        break
        if grid[y][x] == 1:
            fl = 1
        if fl == 0:
            return Point(x, y)
    return None


def generate_agents(grid: list[list], cnt, mapf_should_find_solution):
    mod_grid = copy.deepcopy(grid)
    agents: list[Agent] = []
    for i in range(0, cnt):
        start = generate_free_point(mod_grid, mapf_should_find_solution)
        if start is None:
            return None
        mod_grid[start.y][start.x] = 1
        goal = generate_free_point(mod_grid, mapf_should_find_solution)
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
    if len(sys.argv) < 10:
        print("Not enough arguments!")
        print("Arguments should be")
        print("Map name")
        print("Path to xml file with map")
        print("Path to result directory")
        print("start_agents_cnt")
        print("stop_agents_cnt")
        print("step_agents_cnt")
        print("Number of tests for every agent_cnt")
        print("1 if MAPF should always find a solution, 0 otherwise")
        print("Random seed")
        exit(2)
    map_name = sys.argv[1]
    path_to_xml_with_map = sys.argv[2]
    path_to_result_dir = sys.argv[3]
    start_agents_cnt = int(sys.argv[4])
    stop_agents_cnt = int(sys.argv[5])
    step_agents_cnt = int(sys.argv[6])
    generate_cnt = int(sys.argv[7])
    mapf_should_find_solution = True if sys.argv[8] == "1" else False
    seed = int(sys.argv[9])

    random.seed(seed)

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
            agents = generate_agents(grid, agents_cnt, mapf_should_find_solution)
            if agents == None:
                print(f"Can not generate {agents_cnt} agents!")
                exit(-1)
            write(f"{path_to_result_dir}/{map_name}_{agents_cnt}_{iteration}.xml", tree, agents)


if __name__ == '__main__':
    main()
