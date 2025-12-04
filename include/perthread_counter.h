#pragma once
#include <atomic>
#include <cstddef>
#include "gbbs/gbbs.h"

struct Counter {
    int value;
    Counter(int value_) : value(value_) {}
    Counter(const Counter& other) : value(other.value){}
    inline bool decrement() noexcept { return gbbs::fetch_and_add(&value, -1) == 1; }
    inline bool is_zero() const noexcept { return value == 0; }
    inline bool set_zero() noexcept { 
        auto v = value;
        if (v > 0 && gbbs::atomic_compare_and_swap(&value, v, 0)) return true;
        return false;
    }
};

/*struct Counter {
    std::atomic<int> value;
    Counter(int value_) : value(value_) {}
    Counter(const Counter& other) : value(other.value.load(std::memory_order_relaxed)){}
    inline void decrement() noexcept { value.fetch_sub(1, std::memory_order_relaxed); }
    inline void operator--(int) noexcept { decrement(); }
    inline bool is_zero() const noexcept { return value.load(std::memory_order_acquire) == 0; }
    inline void set_zero() noexcept { value = 0; }
};

struct Counter {
    std::atomic<int> shard;
    Counter() : shard(0){}
    Counter(int shard_) : shard(shard_) {}
    Counter(const Counter& other) : shard(other.shard.load(std::memory_order_relaxed)){}
    Counter(Counter&& other) noexcept : shard(other.shard.load(std::memory_order_relaxed)){}
    Counter& operator=(const Counter& other) {
        shard.store(other.shard.load(std::memory_order_relaxed),std::memory_order_relaxed); return *this;
    }
    Counter& operator=(Counter&& other) noexcept {
        shard.store(other.shard.load(std::memory_order_relaxed),std::memory_order_relaxed); return *this;
    }
    inline void decrement() noexcept { shard.fetch_sub(1, std::memory_order_relaxed); }
    inline void operator--(int) noexcept { decrement(); }
    inline bool is_zero() const noexcept { return shard.load(std::memory_order_acquire) == 0; }
    inline void set_zero() noexcept { shard = 0; }
};
};*/