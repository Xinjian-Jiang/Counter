cd ../..
bazel build //MIS/05_deterministic:MIS_main -c opt
bazel-bin/MIS/05_deterministic/MIS_main -s -b utils/small_graph.bin
cd MIS/05_deterministic