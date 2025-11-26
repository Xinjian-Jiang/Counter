cd ../..
bazel build //MIS/04_baseline_spec_for:MIS_main -c opt
bazel-bin/MIS/04_baseline_spec_for/MIS_main -s -b utils/small_graph.bin
cd MIS/04_baseline_spec_for