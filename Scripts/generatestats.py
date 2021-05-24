import sys
import xml.etree.ElementTree as ET
import matplotlib.pyplot as plt


def get_stats(agents: list, tests_cnt, path_to_dir, task_name):
    result = {}

    for agents_cnt in agents:
        result[agents_cnt] = {}
        for iteration in range(0, tests_cnt):
            path_to_output_xml_file = f"{path_to_dir}/{task_name}_{agents_cnt}_{iteration}_log.xml"
            tree = ET.parse(path_to_output_xml_file)
            if not tree:
                print(f"Error!Can not read {path_to_output_xml_file}")
                exit(1)
            root = tree.getroot()
            log_section = root.find('log')
            summary_section = log_section.find('summary')
            solution_found = True if (summary_section.get('solution_found')).lower() == 'true' else False
            runtime = float(summary_section.get('runtime'))
            cost = int(summary_section.get('cost'))
            result[agents_cnt][iteration] = {'solution_found': solution_found, 'runtime': runtime, 'cost': cost}

    average_costs = []
    average_runtimes = []
    success_rates = []

    for agents_cnt in agents:
        sum_runtime = 0
        sum_cost = 0
        solutions_found = 0
        for iteration in range(0, tests_cnt):
            if result[agents_cnt][iteration]["solution_found"]:
                sum_cost += result[agents_cnt][iteration]["cost"]
            sum_runtime += result[agents_cnt][iteration]["runtime"]
            solutions_found += result[agents_cnt][iteration]["solution_found"]

        average_runtimes.append(sum_runtime / solutions_found)
        average_costs.append(sum_cost / tests_cnt)
        success_rates.append(solutions_found / tests_cnt * 100)

    return average_runtimes, success_rates, average_costs


def main():
    if len(sys.argv) < 8:
        print("Not enough arguments!")

        print("Arguments should be")
        print("Task names")
        print("Task names to use in plot")
        print("Path to directory with xml files")
        print("start_agents_cnt")
        print("stop_agents_cnt")
        print("step_agents_cnt")
        print("Number of tests for every agent_cnt")

        exit(2)

    task_names = sys.argv[1].split(' ')
    task_plot_names = sys.argv[2].split(' ')
    path_to_dir = sys.argv[3]
    start_agents_cnt = int(sys.argv[4])
    stop_agents_cnt = int(sys.argv[5])
    step_agents_cnt = int(sys.argv[6])
    tests_cnt = int(sys.argv[7])

    agents = list(range(start_agents_cnt, stop_agents_cnt, step_agents_cnt))

    task_to_average_runtimes = []
    task_to_success_rates = []
    task_to_average_cost = []

    for i in range(0, len(task_names)):
        average_runtimes, success_rates, average_cost = get_stats(agents, tests_cnt, path_to_dir, task_names[i])
        task_to_average_runtimes.append(average_runtimes)
        task_to_success_rates.append(success_rates)
        task_to_average_cost.append(average_cost)

    plt.subplot(1, 3, 1)

    plt.xlabel("Number of agents")
    plt.ylabel("Success(%)")
    for i in range(0, len(task_names)):
        plt.plot(agents, task_to_success_rates[i])
    plt.legend(task_plot_names)

    plt.subplot(1, 3, 2)

    plt.xlabel("Number of agents")
    plt.ylabel("Time(s)")
    for i in range(0, len(task_names)):
        plt.plot(agents, task_to_average_runtimes[i])
    plt.legend(task_plot_names)

    plt.subplot(1, 3, 3)

    plt.xlabel("Number of agents")
    plt.ylabel("Cost")
    for i in range(0, len(task_names)):
        plt.plot(agents, task_to_average_cost[i])
    plt.legend(task_plot_names)

    plt.show()


if __name__ == '__main__':
    main()
