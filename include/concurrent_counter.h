#pragma once
#include "gbbs/gbbs.h"
#include "aggregating-funnels/structures/counter/aggregatingFunnelCounter.hpp"

using CounterType = SIMPLE_AGG_FUNNEL::AggFunnelCounter<int>;
inline int __pc_thr(int v){int t=parlay::num_workers();return t>0?t:v;}

struct ParCounter {
    CounterType* value;
    ParCounter(int v) : value(new CounterType(v,__pc_thr(v))) {}
    ParCounter(const ParCounter& other) : value(new CounterType(other.value->load(),__pc_thr(0))) {}
    inline void decrement() noexcept { value->fetch_add(-1,parlay::worker_id()); }
    inline void operator--(int) noexcept { decrement(); }
    inline bool is_zero() const noexcept { return value->load()==0; }
    inline bool set_zero() noexcept { if(value->load()>0){ value->fetch_add(-value->load(),parlay::worker_id()); return true;} return false; }
};

/*
#pragma once
#include <atomic>
#include "gbbs/gbbs.h"
#include "aggregating-funnels/structures/counter/aggregatingFunnelCounter.hpp"
inline int __pc_thr(int v){int t=parlay::num_workers();return t>0?t:v;}

using Big = SIMPLE_AGG_FUNNEL::AggFunnelCounter<int>;
struct ParCounter {
    void* p;
    bool big;
    ParCounter(int v):big(v>100000),p(big?(void*)new Big(v,__pc_thr(v)):(void*)new std::atomic<int>(v)){}
    ParCounter(const ParCounter& o):big(o.big),p(o.p){}  // <<< 直接复制指针
    inline void decrement() noexcept { if(big)((Big*)p)->fetch_add(-1,parlay::worker_id()); else ((std::atomic<int>*)p)->fetch_sub(1,std::memory_order_relaxed); }
    inline void operator--(int) noexcept { decrement(); }
    inline bool is_zero() const noexcept { return big?((Big*)p)->load()==0:((std::atomic<int>*)p)->load(std::memory_order_acquire)==0; }
    inline bool set_zero() noexcept { if(big){int v=((Big*)p)->load(); if(v>0){((Big*)p)->fetch_add(-v,parlay::worker_id()); return true;} return false;} int o=((std::atomic<int>*)p)->load(std::memory_order_relaxed); while(o>0) if(((std::atomic<int>*)p)->compare_exchange_weak(o,0,std::memory_order_acq_rel,std::memory_order_relaxed)) return true; return false; }
};
*/