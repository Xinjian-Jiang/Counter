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
    inline void decrement() noexcept { shard.fetch_sub(1, std::memory_order_relaxed); }
    inline void operator--(int) noexcept { decrement(); }
    inline bool is_zero() const noexcept { return shard.load(std::memory_order_acquire) == 0; }
    inline void set_zero() noexcept { shard = 0; }
};
/*
struct Counter {
    int              verified_value;
    std::atomic<int> approxmt_count;
    Counter() : verified_value(0), approxmt_count(0) {}
    Counter(int verified) : verified_value(verified), approxmt_count(verified) {}
    Counter(const Counter& other) : verified_value(other.verified_value), approxmt_count(other.approxmt_count.load(std::memory_order_relaxed)) {}
    Counter(Counter&& other) noexcept : verified_value(other.verified_value), approxmt_count(other.approxmt_count.load(std::memory_order_relaxed)) {}
    Counter& operator=(const Counter& other) {
        if (this != &other) {
            verified_value = other.verified_value;
            approxmt_count.store(
                other.approxmt_count.load(std::memory_order_relaxed),
                std::memory_order_relaxed);
        }
        return *this;
    }
    Counter& operator=(Counter&& other) noexcept {
        if (this != &other) {
            verified_value = other.verified_value;
            approxmt_count.store(
                other.approxmt_count.load(std::memory_order_relaxed),
                std::memory_order_relaxed);
        }
        return *this;
    }
    inline int get_verified() const { return verified_value; }
    inline int get_approxmt() const { return approxmt_count.load(std::memory_order_relaxed); }
    inline void decrement() noexcept { approxmt_count.fetch_sub(1, std::memory_order_relaxed); }
    inline void operator--(int) noexcept { decrement(); }
    inline bool is_zero() const noexcept { return !approxmt_count.load(std::memory_order_acquire); }
    inline void set_zero() noexcept { approxmt_count.store(0, std::memory_order_relaxed);}
};*/