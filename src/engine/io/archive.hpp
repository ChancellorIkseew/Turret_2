#pragma once
#include <cstring> // std::memcpy
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace archive {
    ///@brief Throws std::runtime_error.
    std::string pack(std::string_view source);
    ///@brief Throws std::runtime_error.
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

    class Reader {
        std::string_view data;
    public:
        Reader(std::string_view source) : data(source) {}

        ///@brief Throws std::runtime_error.
        template<class T>
        void get(T& dest) {
            if (data.size() < sizeof(T))
                throw std::runtime_error("Unexpected end of data.");
            archive::get(data, dest);
        }

        ///@brief Throws std::runtime_error.
        template<class T>
        void get(std::vector<T>& dest, size_t count) {
            if (data.size() < sizeof(T) * count)
                throw std::runtime_error("Unexpected end of data.");
            archive::get(data, dest, count);
        }
    };
}
