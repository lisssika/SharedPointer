#pragma once
#include "PointerCounter.h"
#include "DefaultDeleter.h"

template<class Type_, class TDeleter = DefaultDeleter<Type_>>
class SharedPtr {
    typedef SharedPtr<Type_, TDeleter> t_SharedPTR;
    typedef std::remove_extent_t<Type_> Type;
public: // Constructors and destructor.
    SharedPtr(): pointer_counter_(new PointerCounter), ptr_(nullptr){}
    SharedPtr(Type* pObj) :pointer_counter_(new PointerCounter), ptr_(pObj){}
    SharedPtr(t_SharedPTR&& uniquePTR) noexcept:pointer_counter_(uniquePTR.pointer_counter_), ptr_(uniquePTR.ptr_) // Move constructor.
    {
        uniquePTR.pointer_counter_ = nullptr; 
        uniquePTR.ptr_ = nullptr;
    }
    SharedPtr(const t_SharedPTR& shared_ptr):pointer_counter_(shared_ptr.pointer_counter_), ptr_(shared_ptr.ptr_) {
        pointer_counter_->operator++();
    }
    ~SharedPtr()
    {
        release();
    }
public: // Assignment.
    t_SharedPTR& operator=(t_SharedPTR&& sharedPTR)
    {
        release();
        ptr_ = sharedPTR.ptr_;
        pointer_counter_ = sharedPTR.pointer_counter_;
        sharedPTR.pointer_counter_ = new PointerCounter;
        sharedPTR.ptr_ = nullptr;
        return *this;
    }
    t_SharedPTR& operator=(Type* pObject)
    {
	    if (this->ptr_!=pObject)
	    {
            release();
            ptr_ = pObject;
            pointer_counter_->reload();
            //pObject = nullptr;
	    }
        return *this;
    }
    t_SharedPTR& operator=(const t_SharedPTR& sharedPTR)
    {
        if (this != &sharedPTR)
        {
            release();
	        ptr_ = sharedPTR.ptr_;
	        pointer_counter_ = sharedPTR.pointer_counter_;
            pointer_counter_->operator++();
        }
        
        return *this;
    }

public: // Observers.
    Type& operator*() const // Dereference the stored pointer.
    {
        return *ptr_;
    }
    Type* operator->() const // Return the stored pointer.
    {
        return ptr_;
    }
    Type* get() const // Return the stored pointer.
    {
        return ptr_;
    }
    TDeleter& get_deleter(); // Return a reference to the stored deleter.
    explicit operator bool() const // Return false if the stored pointer is null.
    {
        return (ptr_);
    }
public: // Modifiers.
    void release() // Release ownership of any stored pointer.
    {
        --(*pointer_counter_);
        if (!(*pointer_counter_))
        {
            deleter_(ptr_);
            delete pointer_counter_;
        }
    }
    void reset(Type* pObject = nullptr) // Replace the stored pointer.
    {
        release();
        ptr_ = pObject;
        if (pObject)
        {
            pointer_counter_->reload();
        }
        else
        {
            pointer_counter_ = nullptr;
        }
    }
    void swap(t_SharedPTR& sharedPTR) noexcept // Exchange the pointer with another object.
    {
        Type tmp{ std::move(sharedPTR) };
        sharedPTR = std::move(*this);
        *this = std::move(tmp);
    }
private:
    PointerCounter* pointer_counter_ = nullptr;
	Type* ptr_ = nullptr;
    TDeleter deleter_ = {}; 
};