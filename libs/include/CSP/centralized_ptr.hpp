#pragma once
#include <atomic>
#include <functional>
#include <type_traits>
#include "csp_config.hpp"

START_NAMESPACE_CSP
START_NAMESPACE_CSP_PRIVATE

template<class DataType>
class _CentralizedControlBlock {
    std::function<void()> _freeData;
    std::atomic_size_t count;
public:
    DataType* data;
    _CentralizedControlBlock(DataType* data, const std::function<void()>& freeData) :
        _freeData(freeData), data(data), count(1) { }
    ~_CentralizedControlBlock() { reset(); }
    void increment() { count.fetch_add(1, std::memory_order_relaxed); }
    bool decrement() { return count.fetch_sub(1, std::memory_order_acq_rel) == 1; }
    void reset() {
        if (data) {
            if (_freeData) {
                _freeData();
                _freeData = nullptr;
            } 
            delete data;
            data = nullptr;
        }
    }
};

END_NAMESPACE_CSP_PRIVATE

template<class DataType>
class centralized_ptr {
    template<class T> friend class centralized_ptr;
    priv::_CentralizedControlBlock<DataType>* _controlBlock = nullptr;
public:
    centralized_ptr() = default;
    template<class OtherType>
    centralized_ptr(const centralized_ptr<OtherType>& other) noexcept :
        _controlBlock(reinterpret_cast<priv::_CentralizedControlBlock<DataType>*>(other._controlBlock)) {
        static_assert(std::is_convertible_v<OtherType*, DataType*>, "Inconvertible types.");
        _increment();
    }
    centralized_ptr(DataType* data, const std::function<void()>& freeData = nullptr) :
        _controlBlock(new priv::_CentralizedControlBlock(data, freeData)) { }
    centralized_ptr(centralized_ptr const& other) :
        _controlBlock(other._controlBlock) {
        _increment();
    }
    centralized_ptr(centralized_ptr&& other) noexcept :
        _controlBlock(other._controlBlock) {
        other._controlBlock = nullptr;
    }
    ~centralized_ptr() {
        _decrementAndCheck();
    }

    centralized_ptr& operator=(const centralized_ptr& other) {
        if (this != &other) {
            _decrementAndCheck();
            _controlBlock = other._controlBlock;
            _increment();
        }
        return *this;
    }
    centralized_ptr& operator=(centralized_ptr&& other) noexcept {
        if (this != &other) {
            _decrementAndCheck();
            _controlBlock = other._controlBlock;
            other._controlBlock = nullptr;
        }
        return *this;
    }

    CSP_FINLINE const DataType* get() const { return _controlBlock->data; }
    CSP_FINLINE DataType* get() { return _controlBlock->data; }

    explicit CSP_FINLINE operator bool() { return _controlBlock && _controlBlock->data; }

    CSP_FINLINE DataType& operator*() const { return *_controlBlock->data; }
    CSP_FINLINE DataType* operator->() const { return _controlBlock->data; }
    inline bool operator==(const centralized_ptr<DataType>& other) const {
        return _controlBlock == other._controlBlock;
    }
    inline bool operator!=(const centralized_ptr<DataType>& other) const {
        return _controlBlock != other._controlBlock;
    }
private:
    CSP_FINLINE void _decrementAndCheck() {
        if (_controlBlock && _controlBlock->decrement())
            delete _controlBlock;
    }
    CSP_FINLINE void _increment() {
        if (_controlBlock)
            _controlBlock->increment();
    }
};

template<typename DataType, typename... Args>
centralized_ptr<DataType> make_centralized(Args&&... args) {
    return centralized_ptr<DataType>(new DataType(std::forward<Args>(args)...));
}

END_NAMESPACE_CSP
