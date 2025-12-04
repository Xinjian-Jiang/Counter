#pragma once
struct Counter {
    int value;
    Counter(int value_): value(value_) {}
    Counter(const Counter& other): value(other.value) {}
    virtual bool decrement()        noexcept = 0;
    virtual bool decrement_atomic() noexcept = 0;
    virtual bool not_zero()   const noexcept = 0;
    virtual bool set_zero()         noexcept = 0;
    virtual bool set_zero_atomic()  noexcept = 0;
};

struct ZeroCounter: Counter {
    ZeroCounter(int value_): Counter(value_) {}
    ZeroCounter(const ZeroCounter& other): Counter(other) {}
    inline bool decrement()        noexcept override { return value-- == 1; }
    inline bool decrement_atomic() noexcept override { return __atomic_fetch_sub(&value, 1, __ATOMIC_RELAXED) == 1; }
    inline bool not_zero()   const noexcept override { return __atomic_load_n(&value, __ATOMIC_RELAXED) != 0; }
    inline bool set_zero()         noexcept override { return (value > 0) ? (value = 0, true) : false; }
    inline bool set_zero_atomic()  noexcept override { return __atomic_exchange_n(&value, 0, __ATOMIC_ACQ_REL) != 0; }
};
