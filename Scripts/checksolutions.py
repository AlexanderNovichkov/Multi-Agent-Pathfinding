import sys
import subprocess
import xml.etree.ElementTree as ET
from utils import Point, Map, Agent, AgentAction, Trajectory, Parser, get_duration


def check_solution(path_to_xml_file):
    parser = Parser(path_to_xml_file)
    world_map = parser.get_map()
    agents_trajectories = parser.get_trajectories()
    duration = get_duration(agents_trajectories)

    prev_grid_to_agents = [[-1] * world_map.width for i in range(world_map.height)]
    cur_grid_to_agents = [[-1] * world_map.width for i in range(world_map.height)]

    for t in range(0, duration + 1):
        for y in range(0, world_map.height):
            for x in range(0, world_map.width):
                cur_grid_to_agents[y][x] = -1

        for i, agent in enumerate(world_map.agents, start=0):
            pos = agents_trajectories[i].get_position(t, agent.start)
            if world_map.grid[pos.y][pos.x] == 1:
                return False, f"Time = t, agent {i}, static obstacle"
            if cur_grid_to_agents[pos.y][pos.x] != -1:
                return False, f"Time = t, agents {cur_grid_to_agents[pos.y][pos.x]} and {i}, vertex conflict"
            cur_grid_to_agents[pos.y][pos.x] = i

        if t != 0:
            for i, agent in enumerate(world_map.agents, start=0):
                prev_pos = agents_trajectories[i].get_position(t - 1, agent.start)
                cur_pos = agents_trajectories[i].get_position(t, agent.start)
                id1 = prev_grid_to_agents[cur_pos.y][cur_pos.x]
                id2 = cur_grid_to_agents[prev_pos.y][prev_pos.x]
                if id1 == id2 and id1 != -1 and id1 != i:
                    return False, f"Time = {t}, agents {id1} and {i}, swap conflict"

        prev_grid_to_agents, cur_grid_to_agents = cur_grid_to_agents, prev_grid_to_agents

    return True, "OK"


def main():
    if len(sys.argv) < 6:
        print("Not enough arguments!")
        print("Arguments should be")
        print("Task name")
        print("Path to directory with xml files")
        print("start_agents_cnt")
        print("stop_agents_cnt")
        print("step_agents_cnt")
        print("Number of tests for every agent_cnt")
        exit(2)

    map_name = sys.argv[1]
    path_to_dir = sys.argv[2]
    start_agents_cnt = int(sys.argv[3])
    stop_agents_cnt = int(sys.argv[4])
    step_agents_cnt = int(sys.argv[5])
    tests_cnt = int(sys.argv[6])

    solutions_found = 0
    total = 0

    for agents_cnt in range(start_agents_cnt, stop_agents_cnt, step_agents_cnt):
        for iteration in range(0, tests_cnt):
            total += 1
            path_to_xml_file = f"{path_to_dir}/{map_name}_{agents_cnt}_{iteration}_log.xml"
            print(f"Check {path_to_dir}/{map_name}_{agents_cnt}_{iteration}_log.xml")

            tree = ET.parse(path_to_xml_file)
            root = tree.getroot()
            log_section = root.find('log')
            summary_section = log_section.find('summary')
            solution_found = True if (summary_section.get('solution_found')).lower() == 'true' else False

            if not solution_found:
                print("solution is not found")
                continue

            solutions_found += 1
            status, description = check_solution(path_to_xml_file)
            print(description)
            if not status:
                exit(0)

    print(f"Found solutions: {solutions_found} / {total}")
    print("All solutions are correct")


if __name__ == '__main__':
    main()
