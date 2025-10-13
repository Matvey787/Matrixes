#ifndef MYVECTOR_HPP
#define MYVECTOR_HPP

#include <cstddef>
#include <utility>
#include <initializer_list>
#include <stdexcept>

namespace myVector {

template <typename T>
class myVector
{
    using iterator = T*;
    T* data_;
    size_t cap_;
    size_t size_;

public:
    myVector() : data_(nullptr), cap_(0), size_(0) {}

    myVector(size_t size_) : cap_(size_), size_(size_)
    {
        if (size_ > 0)
            data_ = new T[cap_];
    }

    myVector(size_t size_, const T arr[])
    {
        cap_ = size_;
        size_ = size_;
        data_ = new T[cap_];
        for (size_t i = 0; i < size_; ++i) data_[i] = arr[i];
    }

    myVector(std::initializer_list<T> ilist)
    {
        cap_ = size_ = ilist.size();
        data_ = new T[cap_];
        size_t i = 0;
        for (const auto& val : ilist) data_[i++] = val;
    }

    myVector(const myVector<T>& v)
        : cap_(v.cap_), size_(v.size_)
    {
        if (size_ == 0)
        {
            data_ = nullptr;
        }
        else
        {
            data_ = new T[cap_];
            for (size_t i = 0; i < size_; ++i) data_[i] = v.data_[i];
        }
    }

    myVector(myVector<T>&& v) : data_(v.data_), cap_(v.cap_), size_(v.size_)
    {
        v.data_ = nullptr;
        v.cap_ = 0;
        v.size_ = 0;
    }

    myVector<T>& operator=(const myVector<T>& v)
    {
        if (this == &v) return *this;

        delete[] data_;

        cap_ = v.cap_;
        size_ = v.size_;

        if (size_ == 0)
        {
            data_ = nullptr;
        }   
        else
        {
            data_ = new T[cap_];
            for (size_t i = 0; i < size_; ++i) data_[i] = v.data_[i];
        }

        return *this;
    }

    myVector<T>& operator=(myVector<T>&& v)
    {
        if (this == &v) return *this;

        delete[] data_;

        data_ = v.data_;
        cap_ = v.cap_;
        size_ = v.size_;

        v.data_ = nullptr;
        v.cap_ = 0;
        v.size_ = 0;

        return *this;
    }

    ~myVector()
    {
        delete[] data_;
    }

    void push_back(const T& val)
    {
        if (size_ == cap_) 
        {
            cap_ = (cap_ == 0) ? 1 : cap_ * 2;
            T* newData = new T[cap_];

            for (size_t i = 0; i < size_; ++i) newData[i] = std::move(data_[i]);

            delete[] data_;
            data_ = newData;
        }
        data_[size_++] = val;
    }

    void pop_back()
    {
        if (size_ > 0) --size_;
    }

    size_t size()
    {
        return size_;
    }

    T& operator[](size_t index)
    {
        if (index >= size_) throw std::out_of_range("Index out of range");

        return data_[index];
    }

    const T& operator[](size_t index) const
    {
        if (index >= size_) throw std::out_of_range("Index out of range");

        return data_[index];
    }

    iterator begin() { return data_; }
    iterator end() { return data_ + size_; }
    const iterator begin() const { return data_; }
    const iterator end() const { return data_ + size_; }
};

};


#endif // MYVECTOR_HPP