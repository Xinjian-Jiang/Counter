cd ../..
bazel build //MIS/14_test_initialize:MIS_main -c opt
# bazel-bin/MIS/14_test_initialize/MIS_main -s -b utils/small_graph.bin
bazel-bin/MIS/14_test_initialize/MIS_main -s -b /home/csgrads/xjian140/Counter3/testcases/bin/friendster_sym.bin
cd MIS/14_test_initialize