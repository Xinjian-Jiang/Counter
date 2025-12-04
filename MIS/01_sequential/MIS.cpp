#include "graph_utils/graph.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <numeric>
#include <unordered_set>
#include <vector>
#include <filesystem>
using namespace parlay;

template <class Graph>
std::vector<typename Graph::NodeId> MIS(const Graph &G) {
    using NodeId = typename Graph::NodeId;
    size_t n = G.n;

    // Create a permutation to randomize vertex processing order (like GBBS)
    auto perm = parlay::random_permutation<NodeId>(n);
    std::vector<size_t> priority(n);
    for (size_t i = 0; i < n; ++i) { priority[perm[i]] = i;}

    std::vector<bool> in_MIS(n, false);
    std::vector<bool> removed(n, false);

    // in default permuted order
    for (size_t i = 0; i < n; i++) {
        NodeId u = priority[i];
        if (!removed[u]) {
            // Add this vertex 
            in_MIS[u] = true;
            removed[u] = true;
            // Mark neighbors as removed
            for (size_t e = G.offsets[u]; e < G.offsets[u + 1]; e++) {
                NodeId v = G.edges[e].v;
                removed[v] = true;
            }
        }
    }


    std::vector<NodeId> result;
    for (NodeId u = 0; u < n; u++) {
        if (in_MIS[u]) result.push_back(u);
    }
    return result;
}

bool is_undirected_graph(const Graph<uint32_t, uint64_t> &G) {
    using NodeId = uint32_t;
    using EdgeId = uint64_t;

    std::unordered_set<uint64_t> edge_set;
    edge_set.reserve(G.m * 2);

    auto encode = [&](NodeId u, NodeId v) -> uint64_t {
        return (uint64_t(u) << 32) | uint64_t(v);
    };

    // 先把所有 u->v 边加入 hash set
    for (NodeId u = 0; u < G.n; u++) {
        for (EdgeId e = G.offsets[u]; e < G.offsets[u + 1]; e++) {
            NodeId v = G.edges[e].v;
            edge_set.insert(encode(u, v));
        }
    }

    // 检查每一条 u->v 是否存在 v->u
    for (NodeId u = 0; u < G.n; u++) {
        for (EdgeId e = G.offsets[u]; e < G.offsets[u + 1]; e++) {
            NodeId v = G.edges[e].v;
            if (edge_set.find(encode(v, u)) == edge_set.end()) {
                std::cerr << "Missing reverse edge: " << u << " -> " << v 
                          << " but no " << v << " -> " << u << "\n";
                return false;
            }
        }
    }

    return true;
}

template <class NodeId>
void save_mis_to_file(const std::vector<NodeId>& mis_set, const std::string& filename) {
    auto sorted_mis = mis_set;
    std::sort(sorted_mis.begin(), sorted_mis.end());

    // Create directory if needed
    size_t last_slash = filename.find_last_of('/');
    if (last_slash != std::string::npos) {
        std::string dir = filename.substr(0, last_slash);
        system(("mkdir -p " + dir).c_str());
    }

    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Error: Cannot open output file " << filename << std::endl;
        return;
    }

    out << sorted_mis.size();
    for (const auto& v : sorted_mis) {
        out << "," << v;
    }
    out.close();
    // std::cout << "MIS result saved to " << filename << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) { std::cerr << "Usage: ./mis input_graph [verify]" << std::endl; return 1; }
    const char* filename = argv[1];
    Graph<uint32_t, uint64_t> G;
    G.read_graph(filename);
    std::string graphname = std::filesystem::path(filename).stem().string();
    //std::cout << "无向图：" << is_undirected_graph(G) << std::endl;
    if (!G.symmetrized) { G = make_symmetrized(G); }
    // Warm up
    { auto tmp = MIS(G); }
    // Test
    
    std::vector<double> times;
    //std::cout << graphname << "    ";
    for (int run = 1; run <= 3; run++) {
        internal::timer t;
        auto mis_set = MIS(G);
        t.stop();
        times.push_back(t.total_time());
    }
    double avg_time = std::accumulate(times.begin(), times.end(), 0.0) / times.size();
    std::cout << avg_time << "\n";
    // Verify
    bool verify = false;
    if (argc == 3) verify = (std::atoi(argv[2]) != 0);
    if (verify) {
        auto mis_set = MIS(G);
        std::string output_file = "./output/" + graphname + ".txt";
        save_mis_to_file(mis_set, output_file);
    }
    return 0;
}