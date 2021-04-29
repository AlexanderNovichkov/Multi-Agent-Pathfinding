import sys
import subprocess

def main():
    if len(sys.argv) < 7:
        print("Not enough arguments!")
        print("Arguments should be")
        print("Map name")
        print("Path to directory with xml files")
        print("Path to executable file")
        print("start_agents_cnt")
        print("stop_agents_cnt")
        print("step_agents_cnt")
        print("Number of tests for every agent_cnt")
        exit(2)

    map_name = sys.argv[1]
    path_to_dir = sys.argv[2]
    path_to_executable = sys.argv[3]
    start_agents_cnt = int(sys.argv[4])
    stop_agents_cnt = int(sys.argv[5])
    step_agents_cnt = int(sys.argv[6])
    tests_cnt = int(sys.argv[7])

    for agents_cnt in range(start_agents_cnt, stop_agents_cnt, step_agents_cnt):
        for iteration in range(0, tests_cnt):
            print(f"Run test {iteration+1} with {agents_cnt} agents")
            path_to_xml_file = f"{path_to_dir}/{map_name}_{agents_cnt}_{iteration}.xml"
            result = subprocess.run([path_to_executable, path_to_xml_file], stdout=subprocess.DEVNULL).returncode
            if result != 0:
                print(f"Error! Return code is {result}!")
                exit(1)

if __name__ == '__main__':
    main()