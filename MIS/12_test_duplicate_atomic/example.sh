cd ../..
bazel build //MIS/12_test_duplicate_atomic:MIS_main -c opt
# bazel-bin/MIS/12_test_duplicate_atomic/MIS_main -s -b utils/small_graph.bin
bazel-bin/MIS/12_test_duplicate_atomic/MIS_main -s -b /home/csgrads/xjian140/Counter3/testcases/bin/friendster_sym.bin
cd MIS/12_test_duplicate_atomic