#ifndef WAREANA_ALLOCATOR_HPP_
#define WAREANA_ALLOCATOR_HPP_
#include <cstdint>
#include <cstddef>
#include <cassert>
#include <type_traits>
#include <exception>
#include <iostream>

static_assert(__cplusplus >= 201103L, "The C++ version must be greater than C++11");

template<class T, size_t objCount>
struct warena_allocator {
    public:
    typedef T value_type;

    typedef T* pointer;
    typedef const T* const_pointer;

    typedef T& reference;
    typedef const T& const_reference;

    typedef void* void_pointer;
    typedef const void* const_void_pointer;

    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;
    
    private:
    typedef typename std::aligned_storage<sizeof(value_type), alignof(value_type)>::type storage_t;

    private:
    storage_t data_[objCount];
    size_t currentIndex_; // NOT A SIZE OF DATA IN BYTES

    public:
    warena_allocator() : data_{}, currentIndex_(0) {
        
    }
    // WHAT

    template<class U>
    warena_allocator(const warena_allocator<U, objCount>& other) : data_{}, currentIndex_(other.currentIndex_) {

    }
    ~warena_allocator() {
        currentIndex_ = 0;
    }

    public:
    template<class U>
    struct rebind {
        using other = warena_allocator<U, objCount>;
    };

    public:
    pointer allocate(size_type count) {
        if ((currentIndex_ + count) > objCount) {
            throw std::bad_alloc();
        }
        pointer result = reinterpret_cast<pointer>(&data_[currentIndex_]);
        currentIndex_ += count;
        return result;
    }
    void deallocate(pointer ptr, size_type count) noexcept {
        if (((difference_type)ptr + (difference_type)count) == ((difference_type)data_ + (difference_type)currentIndex_)) {
            currentIndex_ -= count;
        }
    }
    size_type max_size() const noexcept {
        return objCount - currentIndex_;
    }

    public:
    pointer address(reference r) const {
        return &r;
    }
    const_pointer address(const_reference r) const {
        return &r;
    }

    public:
    template<class U, class... ArgsT>
    void construct(U* ptr, ArgsT&&... args) const {
        new(ptr)U(std::forward<ArgsT>(args)...);
    }
    template<class U>
    void destroy(U* ptr) const {
        ptr->~U();
    }
    
};
#endif // ifndef WAREANA_ALLOCATOR_HPP_