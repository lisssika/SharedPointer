#pragma once
#include <stdexcept>

class PointerCounter
{
    size_t n_;
public:
    PointerCounter() :n_(1) {}
    void reload()
    {
        n_ = 1;
    }
    size_t operator()() const { return n_; }
    const PointerCounter& operator++()
    {
        n_++;
        return *this;
    }
    const PointerCounter& operator--()
    {
        if (n_ == 0)
        {
            throw std::runtime_error(" There is no any pointer. It is impossible to reduce the PointerCounter");
        }
        n_--;
        return *this;
    }

    explicit operator bool() const
    {
        return (n_);
    }
};