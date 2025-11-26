cd ../..
bazel build //MIS/03_baseline_random_greedy:MIS_main -c opt
bazel-bin/MIS/03_baseline_random_greedy/MIS_main -s -b utils/small_graph.bin
cd MIS/03_baseline_random_greedy