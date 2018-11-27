#pragma once

template < class T >
class UniquePtr {
 public:
    explicit UniquePtr( T* new_ptr = nullptr ):
    ptr( new_ptr )
    {
    }

    UniquePtr( UniquePtr&& u_ptr ) noexcept {
        ptr = u_ptr.ptr;
        u_ptr.ptr = nullptr;
    }

    UniquePtr& operator=( UniquePtr&& u_ptr ) noexcept {
        if (&u_ptr == this) {
            return *this;
        }
        delete ptr;
        ptr = u_ptr.ptr;
        u_ptr.ptr = nullptr;
    }

    UniquePtr( const UniquePtr& other ) = delete;
    UniquePtr& operator=( UniquePtr& u_ptr ) = delete;

    ~UniquePtr() {
        delete ptr;
    }

    T*operator->() {
        return ptr;
    }

    void reset( T* new_ptr ) {
        delete ptr;
        ptr = new_ptr;
    }

    T* release() {
        T* return_ptr = ptr;
        ptr = nullptr;
        return return_ptr;
    }


 private:
  T* ptr;
};
