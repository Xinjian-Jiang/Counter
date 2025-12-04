#pragma once
struct Counter {
    int value;
    Counter(int value_) : value(value_) {}
    Counter(const Counter& other): value(other.value) { }
    inline bool decrement()        noexcept { return value-- == 1; }
    inline bool decrement_atomic() noexcept { return __atomic_fetch_sub(&value, 1, __ATOMIC_RELAXED) == 1; }
    inline bool not_zero() const   noexcept { return __atomic_load_n(&value, __ATOMIC_RELAXED) != 0; }
    inline bool set_zero()         noexcept { return (value > 0) ? (value = 0, true) : false; }
    inline bool set_zero_atomic()  noexcept { return __atomic_exchange_n(&value, 0, __ATOMIC_ACQ_REL) != 0; }
};


/*
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