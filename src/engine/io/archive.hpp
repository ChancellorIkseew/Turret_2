#pragma once
#include <string>
#include <string_view>
#include <vector>

namespace archive {
    std::string pack(std::string_view source);
    std::string unpack(std::string_view packed);

    template<class T>
    inline void add(std::string& blob, const std::vector<T>& vec) {
        static_assert(std::is_trivially_copyable_v<T>);
        blob.append(reinterpret_cast<const char*>(vec.data()), vec.size() * sizeof(T));
    }

    template<typename T>
    inline void add(std::string& blob, const T& data) {
        static_assert(std::is_trivially_copyable_v<T>);
        blob.append(reinterpret_cast<const char*>(&data), sizeof(T));
    }

    template<class T>
    inline void get(std::string_view& blob, std::vector<T>& vec, size_t count) {
        static_assert(std::is_trivially_copyable_v<T>);
        if (count == 0)
            return;
        const size_t byteSize = sizeof(T) * count;
        const T* ptr = reinterpret_cast<const T*>(blob.data());
        vec.assign(ptr, ptr + count);
        blob.remove_prefix(byteSize);
    }

    template<class T>
    inline void get(std::string_view& blob, T& data) {
        static_assert(std::is_trivially_copyable_v<T>);
        std::memcpy(&data, blob.data(), sizeof(T));
        blob.remove_prefix(sizeof(T));
    }
}
