#include "MIS.h"
#include <fstream>
#include <iostream>
#include <string>

inline std::string get_graphname(const std::string& fullpath) {
    std::string name = fullpath;
    size_t pos1 = name.find_last_of('/'); if (pos1 != std::string::npos) name = name.substr(pos1 + 1);
    size_t pos2 = name.find_last_of('.'); if (pos2 != std::string::npos) name = name.substr(0, pos2);
    return name;
}

template <typename T>
void print_mis(parlay::sequence<T>& mis, std::string algo, std::string graphname) {
    std::ofstream out("MIS/" + algo + "/output/" + graphname + ".txt");
    int cnt = 0; for (size_t i = 0; i < mis.size(); i++) cnt += mis[i]; out << cnt;
    for (size_t i = 0; i < mis.size(); i++) { if (mis[i]) { out << "," << i; } }
    out.close();
}


namespace gbbs {

template <class Graph>
double MaximalIndependentSet_runner(Graph& G, commandLine P) {
    std::cout << "### ===================================================================" << std::endl;
    std::cout << "### Application: MIS" << std::endl;
    std::cout << "### Graph: " << P.getArgument(0) << std::endl;
    std::cout << "### Threads: " << num_workers() << std::endl;
    std::cout << "### n: " << G.n << std::endl;
    std::cout << "### m: " << G.m << std::endl;
    std::cout << "### Params: -verify = " << bool(P.getOption("-verify")) << std::endl;

    double tt = 0.0; timer t; t.start();
    auto MaximalIndependentSet = MaximalIndependentSet_rootset::MaximalIndependentSet(G);
    tt = t.stop(); std::cout << "### Running Time: " << tt << std::endl;

    if (P.getOption("-verify")) print_mis(MaximalIndependentSet, "12_test_all_atomic", get_graphname(P.getArgument(0)));
    return tt;
}

} // namespace gbbs

generate_main(gbbs::MaximalIndependentSet_runner, false);
