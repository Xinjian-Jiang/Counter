#pragma once
#include <atomic>
#include <cstddef>

struct Counter {
    std::atomic<int>  shard1;
    std::atomic<int>  shard2;
    std::atomic<unsigned char> zero_flag;

    thread_local static uint32_t rng;

    Counter()
        : shard1(0),
          shard2(0),
          zero_flag(0)
    {}

    Counter(int init)
        : shard1(0),
          shard2(0)
    {
        if (init <= 10) {
            shard1.store(init, std::memory_order_relaxed);
            shard2.store(0,   std::memory_order_relaxed);
            zero_flag.store(init > 0, std::memory_order_relaxed);
        } else {
            int half = init / 2;
            shard1.store(half,             std::memory_order_relaxed);
            shard2.store(init - half,      std::memory_order_relaxed);
            zero_flag.store(3,         std::memory_order_relaxed);
        }
    }

    Counter(const Counter& other)
    {
        shard1.store(other.shard1.load(std::memory_order_relaxed),
                     std::memory_order_relaxed);
        shard2.store(other.shard2.load(std::memory_order_relaxed),
                     std::memory_order_relaxed);
        zero_flag.store(other.zero_flag.load(std::memory_order_relaxed),
                        std::memory_order_relaxed);
    }

    Counter(Counter&& other) noexcept
    {
        shard1.store(other.shard1.load(std::memory_order_relaxed),
                     std::memory_order_relaxed);
        shard2.store(other.shard2.load(std::memory_order_relaxed),
                     std::memory_order_relaxed);
        zero_flag.store(other.zero_flag.load(std::memory_order_relaxed),
                        std::memory_order_relaxed);
    }

    Counter& operator=(const Counter& other)
    {
        shard1.store(other.shard1.load(std::memory_order_relaxed),
                     std::memory_order_relaxed);
        shard2.store(other.shard2.load(std::memory_order_relaxed),
                     std::memory_order_relaxed);
        zero_flag.store(other.zero_flag.load(std::memory_order_relaxed),
                        std::memory_order_relaxed);
        return *this;
    }

    Counter& operator=(Counter&& other) noexcept
    {
        shard1.store(other.shard1.load(std::memory_order_relaxed),
                     std::memory_order_relaxed);
        shard2.store(other.shard2.load(std::memory_order_relaxed),
                     std::memory_order_relaxed);
        zero_flag.store(other.zero_flag.load(std::memory_order_relaxed),
                        std::memory_order_relaxed);
        return *this;
    }

    inline void decrement() noexcept
    {
        int flag = zero_flag.load(std::memory_order_relaxed);
        if (flag == 1) {
            int old = shard1.fetch_sub(1, std::memory_order_relaxed);
            if (old == 1) {
                zero_flag.store(0, std::memory_order_release);
            }
            return;
        }
        else if (flag == 2) {
            int old = shard2.fetch_sub(1, std::memory_order_relaxed);
            if (old == 1) {
                zero_flag.store(0, std::memory_order_release);
            }
            return;
        }
        else {
            rng ^= rng << 13; rng ^= rng >> 17; rng ^= rng << 5;
            if (rng & 1) {
                int old = shard1.fetch_sub(1, std::memory_order_relaxed);
                if (old == 1) {
                    zero_flag.store(2, std::memory_order_release);
                }
            } else {
                int old = shard2.fetch_sub(1, std::memory_order_relaxed);
                if (old == 1) {
                    zero_flag.store(1, std::memory_order_release);
                }
            }
        }
    }

    inline void operator--(int) noexcept { decrement(); }

    inline bool is_zero() const noexcept
    {
        return zero_flag.load(std::memory_order_acquire) == 0;
    }

    inline void set_zero() noexcept
    {
        zero_flag.store(0, std::memory_order_release);
    }
};

thread_local uint32_t Counter::rng = 0x12345678;
