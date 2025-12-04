#pragma once
struct Counter {
    int* value;
    Counter(int value_) : value(new int(value_)) {}
    Counter(const Counter& other): value(other.value) {}
    inline bool decrement()        noexcept { return (*value)-- == 1; }
    inline bool decrement_atomic() noexcept { return __atomic_fetch_sub(value, 1, __ATOMIC_RELAXED) == 1; }
    inline bool not_zero() const   noexcept { return __atomic_load_n(value, __ATOMIC_RELAXED) != 0; }
    inline bool set_zero()         noexcept { return (*value > 0) ? (*value = 0, true) : false; }
    inline bool set_zero_atomic()  noexcept { return __atomic_exchange_n(value, 0, __ATOMIC_ACQ_REL) != 0; }
};
