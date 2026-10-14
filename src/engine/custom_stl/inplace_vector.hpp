#pragma once
#include <array>
#include <cstddef>
#include <utility>

// === minimal InplaceVetor, replace with std::inplace_vector in C++26 ===

template<class T>
concept Trivial = std::is_trivially_copyable_v<T> && std::is_trivially_destructible_v<T>;

template<Trivial T, size_t CAPACITY>
class InplaceVector {
    std::array<T, CAPACITY> storage_{};
    size_t count_ = 0;
public:
    // === construction ===
    constexpr InplaceVector() = default;

    // === mutation ===
    constexpr bool push_back(const T& value) noexcept {
        if (full()) return false;
        storage_[count_++] = value;
        return true;
    }
    constexpr bool push_back(T&& value) noexcept {
        if (full()) return false;
        storage_[count_++] = std::move(value);
        return true;
    }

    constexpr void pop_back() noexcept { if (count_ > 0) --count_; }
    constexpr void clear()    noexcept { count_ = 0; }

    // === access ===
    constexpr T&       operator[](size_t i)       { return storage_[i]; }
    constexpr const T& operator[](size_t i) const { return storage_[i]; }
    constexpr T*       data()                     { return storage_.data(); }
    constexpr const T* data()               const { return storage_.data(); }

    // === status ===
    constexpr size_t size()     const noexcept { return count_; }
    constexpr size_t capacity() const noexcept { return CAPACITY; }
    constexpr bool   empty()    const noexcept { return count_ == 0; }
    constexpr bool   full()     const noexcept { return count_ >= CAPACITY; }

    // === iteration (range-based for) ===
    constexpr T*       begin()        { return storage_.data(); }
    constexpr T*       end()          { return storage_.data() + count_; }
    constexpr const T* begin()  const { return storage_.data(); }
    constexpr const T* end()    const { return storage_.data() + count_; }
    constexpr const T* cbegin() const { return storage_.data(); }
    constexpr const T* cend()   const { return storage_.data() + count_; }
};
