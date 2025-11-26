#pragma once
#include <atomic>

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
    inline void decrement() noexcept { 
        if (shard.load(std::memory_order_relaxed) < 10) { shard.fetch_sub(1, std::memory_order_relaxed); return; }
        thread_local static uint32_t x = 0x12345678u;
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        if (x & 1) { shard.fetch_sub(2, std::memory_order_relaxed); }
    }
    inline void operator--(int) noexcept { decrement(); }
    inline bool is_zero() const noexcept { return shard.load(std::memory_order_acquire) <= 0; }
    inline void set_zero() noexcept { shard = 0; }
};