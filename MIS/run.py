import re
import sys
import csv
import subprocess
import numpy as np
from config import *

def parse_output(text):
    blocks = text.split("### Application:")
    parsed = []

    for blk in blocks:
        blk = blk.strip()
        if not blk:
            continue

        # Running Time（必须有，否则跳过该块）
        m_rt = re.search(r"### Running Time:\s*([0-9.eE+-]+)", blk)
        if not m_rt:
            continue
        running_time = float(m_rt.group(1))

        # Counter initialization time（可选）
        m_init = re.search(r"## Counter initialization time\s*=\s*([0-9.eE+-]+)", blk)
        counter_init = [float(m_init.group(1))] if m_init else []

        # 各 round（可选）
        round_times = re.findall(r"## round\s*=\s*\d+\s*time\s*=\s*([0-9.eE+-]+)", blk)
        rounds = [float(r) for r in round_times] if round_times else []

        entry = [running_time] + counter_init + rounds
        parsed.append(entry)

    return parsed

def execute(command, cwd=""):
    print(" ".join(command))
    result = subprocess.run(
        command, cwd=cwd,
        stdout=subprocess.PIPE, stderr=subprocess.PIPE,
        universal_newlines=True
    )
    stdout = result.stdout
    print(stdout)
    if result.stderr:
        print(result.stderr)

    return parse_output(stdout)

def execute_live(command, cwd=""):
    print(" ".join(command))
    process = subprocess.Popen(command, cwd=cwd)
    process.wait()

def pad_times_with_zeros(times):
    # 找到最长行的长度
    max_len = max(len(row) for row in times)

    # 每行补齐到同样长度
    for i in range(len(times)):
        if len(times[i]) < max_len:
            times[i] += [0.0] * (max_len - len(times[i]))

if __name__ == "__main__":
    if len(sys.argv) != 2:
        exit(1)
    algo = str(sys.argv[1])
    execute_live(["bazel", "build", "//benchmarks/Counter/MIS/" + algo + ":MIS_main", "-c", "opt"], "../../..")
    with open(algo + "/benchmark.csv", 'w', newline='', encoding='utf-8') as f:
        writer = csv.writer(f)
        writer.writerow(["graph name", "Running Time", "Counter Initialization Time", "1", "2", "3"])
        for graph in graphs:
            times = []
            if graph == "grid_1000_100000_sym" and algo == "baseline_spec_for":
                times = [99999]
            # elif graph == "flickr_sym" and algo == "deterministic":
            #     times = [99999]
            else:
                times += execute(["bazel-bin/benchmarks/Counter/MIS/" + algo + "/MIS_main", "-s", "-b", GRAPH_PATH + graph + ".bin"], "../../..")[1:]
                times += execute(["bazel-bin/benchmarks/Counter/MIS/" + algo + "/MIS_main", "-s", "-b", GRAPH_PATH + graph + ".bin"], "../../..")[1:]
                pad_times_with_zeros(times)
                times = np.array(times)
                times = times.mean(axis=0).tolist()
            row = [graph] + times
            print(row)
            writer.writerow(row)

# python3 run.py baseline_random_greedy
# python3 run.py baseline_spec_for
# python3 run.py deterministic
# python3 run.py tas
# python3 run.py approximate
# python3 run.py deterministic_layered