#include <stdexcept>

class SerializerError : public std::runtime_error {
    using _MyBase = std::runtime_error;
public:
    explicit SerializerError(const std::string& message) noexcept
        : _MyBase(message) { }
    explicit SerializerError(const char* message) noexcept
        : _MyBase(message) { }
};
