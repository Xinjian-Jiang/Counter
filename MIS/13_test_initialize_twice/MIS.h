#pragma once
#include "gbbs/gbbs.h"
#include "deterministic_counter.h"

namespace gbbs {
namespace MaximalIndependentSet_rootset {

template <class P, class W>
struct GetNghs {
    P& p;
    GetNghs(P& p) : p(p) {}
    inline bool updateAtomic(const uintE& s, const uintE& d, const W& wgh) { return p[d].set_zero(); }
    inline bool update(const uintE& s, const uintE& d, const W& w) { return updateAtomic(s, d, w); }
    inline bool cond(uintE d) { return p[d].not_zero(); }
};

template <class W>
struct mis_f {
    Counter* counters;
    uintE* perm;
    mis_f(Counter* _counters, uintE* _perm) : counters(_counters), perm(_perm) {}
    inline bool updateAtomic(const uintE& s, const uintE& d, const W& wgh) {
        if (perm[s] < perm[d]) { return counters[d].decrement(); }
        return false;
    }
    inline bool update(const uintE& s, const uintE& d, const W& w) { return updateAtomic(s, d, w); }
    inline bool cond(uintE d) { return counters[d].not_zero(); }
};


template <class Graph>
inline sequence<bool> MaximalIndependentSet(Graph& G) {
    using W = typename Graph::weight_type;

    // 初始化计数器
    timer t1; t1.start();
    size_t n = G.n;
    auto perm = parlay::random_permutation<uintE>(n);
    auto counters = parlay::tabulate<Counter>(n, [&](size_t i){
        uintE our_pri = perm[i];
        auto count_f = [&](uintE src, uintE ngh, const W& wgh) { return perm[ngh] < our_pri;};
        int cnt = static_cast<int>(G.get_vertex(i).out_neighbors().count(count_f));
        return Counter(cnt);
    });
    std::cout << "## Counter initialization time = " << t1.stop() << std::endl;

    timer t11; t11.start();
    auto counters1 = parlay::tabulate<Counter>(n, [&](size_t i){
        uintE our_pri = perm[i];
        auto count_f = [&](uintE src, uintE ngh, const W& wgh) { return perm[ngh] < our_pri;};
        int cnt = static_cast<int>(G.get_vertex(i).out_neighbors().count(count_f));
        return Counter(cnt);
    });
    std::cout << "## Counter initialization time2 = " << t11.stop() << std::endl;

    // 初始化frontier(rootset): counter为0的点
    auto roots = vertexSubset(n, std::move(parlay::pack_index<uintE>(
        parlay::delayed_seq<bool>(n, [&](size_t i) { return !counters[i].not_zero(); })
    )));

    // parallel MIS
    auto in_mis = sequence<bool>(n, false);
    size_t rounds = 0, finished = 0;
    while (finished != n && roots.size() > 0) {
        timer nr; nr.start();
        vertexMap(roots, [&](uintE v) { in_mis[v] = true; });                            // roots加入MIS
        auto removed = neighbor_map(G, roots, GetNghs<decltype(counters), W>(counters)); // 获得 roots 的邻居，并把这些邻居的计数器清零
        auto new_roots = edgeMap(G, removed, mis_f<W>(counters.begin(), perm.begin()), -1, sparse_blocked); // 对 removed 的邻居做 “计数器减一”，减到 0 的成为新的 roots        
        rounds++; finished += (roots.size() + removed.size());
        roots = std::move(new_roots);
        std::cout << "## round = " << rounds << " time = " << nr.stop() << "\n";
    }
    return in_mis;
}


}  // namespace MaximalIndependentSet_rootset
}  // namespace gbbs