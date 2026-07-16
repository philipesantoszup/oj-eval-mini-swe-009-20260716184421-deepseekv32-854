#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <new>

namespace sjtu
{
template<typename T>
class vector;

/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector
{
private:
    T* data_;
    size_t size_;
    size_t capacity_;
    
    void reallocate(size_t new_capacity) {
        T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));
        for (size_t i = 0; i < size_; ++i) {
            new (new_data + i) T(data_[i]);
            data_[i].~T();
        }
        ::operator delete(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }
    
public:
    class const_iterator;
    class iterator
    {
    private:
        T* ptr_;
        vector* vec_;
        
    public:
        friend class vector<T>;
        
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::output_iterator_tag;

        iterator(T* p = nullptr, vector* v = nullptr) : ptr_(p), vec_(v) {}
        
        iterator operator+(const int &n) const {
            return iterator(ptr_ + n, vec_);
        }
        iterator operator-(const int &n) const {
            return iterator(ptr_ - n, vec_);
        }
        int operator-(const iterator &rhs) const {
            if (vec_ != rhs.vec_) throw invalid_iterator();
            return ptr_ - rhs.ptr_;
        }
        iterator& operator+=(const int &n) {
            ptr_ += n;
            return *this;
        }
        iterator& operator-=(const int &n) {
            ptr_ -= n;
            return *this;
        }
        iterator operator++(int) {
            iterator tmp = *this;
            ++ptr_;
            return tmp;
        }
        iterator& operator++() {
            ++ptr_;
            return *this;
        }
        iterator operator--(int) {
            iterator tmp = *this;
            --ptr_;
            return tmp;
        }
        iterator& operator--() {
            --ptr_;
            return *this;
        }
        T& operator*() const {
            return *ptr_;
        }
        bool operator==(const iterator &rhs) const {
            return ptr_ == rhs.ptr_;
        }
        bool operator==(const const_iterator &rhs) const;
        bool operator!=(const iterator &rhs) const {
            return ptr_ != rhs.ptr_;
        }
        bool operator!=(const const_iterator &rhs) const;
    };
    
    class const_iterator
    {
    private:
        const T* ptr_;
        const vector* vec_;
        
    public:
        friend class vector<T>;
        
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::output_iterator_tag;

        const_iterator(const T* p = nullptr, const vector* v = nullptr) : ptr_(p), vec_(v) {}
        
        const_iterator operator+(const int &n) const {
            return const_iterator(ptr_ + n, vec_);
        }
        const_iterator operator-(const int &n) const {
            return const_iterator(ptr_ - n, vec_);
        }
        int operator-(const const_iterator &rhs) const {
            if (vec_ != rhs.vec_) throw invalid_iterator();
            return ptr_ - rhs.ptr_;
        }
        const_iterator& operator+=(const int &n) {
            ptr_ += n;
            return *this;
        }
        const_iterator& operator-=(const int &n) {
            ptr_ -= n;
            return *this;
        }
        const_iterator operator++(int) {
            const_iterator tmp = *this;
            ++ptr_;
            return tmp;
        }
        const_iterator& operator++() {
            ++ptr_;
            return *this;
        }
        const_iterator operator--(int) {
            const_iterator tmp = *this;
            --ptr_;
            return tmp;
        }
        const_iterator& operator--() {
            --ptr_;
            return *this;
        }
        const T& operator*() const {
            return *ptr_;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr_ == rhs.ptr_;
        }
        bool operator==(const iterator &rhs) const {
            return ptr_ == rhs.ptr_;
        }
        bool operator!=(const const_iterator &rhs) const {
            return ptr_ != rhs.ptr_;
        }
        bool operator!=(const iterator &rhs) const {
            return ptr_ != rhs.ptr_;
        }
    };
    
    vector() : data_(nullptr), size_(0), capacity_(0) {}
    
    vector(const vector &other) : size_(other.size_), capacity_(other.size_) {
        if (capacity_ == 0) {
            data_ = nullptr;
            return;
        }
        data_ = static_cast<T*>(::operator new(capacity_ * sizeof(T)));
        for (size_t i = 0; i < size_; ++i) {
            new (data_ + i) T(other.data_[i]);
        }
    }
    
    ~vector() {
        clear();
        ::operator delete(data_);
    }
    
    vector &operator=(const vector &other) {
        if (this == &other) return *this;
        clear();
        ::operator delete(data_);
        size_ = other.size_;
        capacity_ = other.size_;
        if (capacity_ == 0) {
            data_ = nullptr;
            return *this;
        }
        data_ = static_cast<T*>(::operator new(capacity_ * sizeof(T)));
        for (size_t i = 0; i < size_; ++i) {
            new (data_ + i) T(other.data_[i]);
        }
        return *this;
    }
    
    T & at(const size_t &pos) {
        if (pos >= size_) throw index_out_of_bound();
        return data_[pos];
    }
    const T & at(const size_t &pos) const {
        if (pos >= size_) throw index_out_of_bound();
        return data_[pos];
    }
    
    T & operator[](const size_t &pos) {
        if (pos >= size_) throw index_out_of_bound();
        return data_[pos];
    }
    const T & operator[](const size_t &pos) const {
        if (pos >= size_) throw index_out_of_bound();
        return data_[pos];
    }
    
    const T & front() const {
        if (size_ == 0) throw container_is_empty();
        return data_[0];
    }
    
    const T & back() const {
        if (size_ == 0) throw container_is_empty();
        return data_[size_ - 1];
    }
    
    iterator begin() {
        return iterator(data_, this);
    }
    const_iterator begin() const {
        return const_iterator(data_, this);
    }
    const_iterator cbegin() const {
        return const_iterator(data_, this);
    }
    
    iterator end() {
        return iterator(data_ + size_, this);
    }
    const_iterator end() const {
        return const_iterator(data_ + size_, this);
    }
    const_iterator cend() const {
        return const_iterator(data_ + size_, this);
    }
    
    bool empty() const {
        return size_ == 0;
    }
    
    size_t size() const {
        return size_;
    }
    
    void clear() {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0;
    }
    
    iterator insert(iterator pos, const T &value) {
        if (pos.vec_ != this) throw invalid_iterator();
        size_t idx = pos.ptr_ - data_;
        if (size_ == capacity_) {
            reallocate(capacity_ ? capacity_ * 2 : 1);
        }
        for (size_t i = size_; i > idx; --i) {
            new (data_ + i) T(data_[i - 1]);
            data_[i - 1].~T();
        }
        new (data_ + idx) T(value);
        ++size_;
        return iterator(data_ + idx, this);
    }
    
    iterator insert(const size_t &ind, const T &value) {
        if (ind > size_) throw index_out_of_bound();
        return insert(begin() + ind, value);
    }
    
    iterator erase(iterator pos) {
        if (pos.vec_ != this) throw invalid_iterator();
        size_t idx = pos.ptr_ - data_;
        data_[idx].~T();
        for (size_t i = idx; i + 1 < size_; ++i) {
            new (data_ + i) T(data_[i + 1]);
            data_[i + 1].~T();
        }
        --size_;
        return iterator(data_ + idx, this);
    }
    
    iterator erase(const size_t &ind) {
        if (ind >= size_) throw index_out_of_bound();
        return erase(begin() + ind);
    }
    
    void push_back(const T &value) {
        if (size_ == capacity_) {
            reallocate(capacity_ ? capacity_ * 2 : 1);
        }
        new (data_ + size_) T(value);
        ++size_;
    }
    
    void pop_back() {
        if (size_ == 0) throw container_is_empty();
        data_[size_ - 1].~T();
        --size_;
    }
};

template<typename T>
bool vector<T>::iterator::operator==(const const_iterator &rhs) const {
    return ptr_ == rhs.ptr_;
}

template<typename T>
bool vector<T>::iterator::operator!=(const const_iterator &rhs) const {
    return ptr_ != rhs.ptr_;
}

}

#endif
