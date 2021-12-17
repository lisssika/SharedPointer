#pragma once
#include "PointerCounter.h"
#include "DefaultDeleter.h"

template<class Type_, class TDeleter = DefaultDeleter<Type_>>
class SharedPtr {
    typedef SharedPtr<Type_, TDeleter> t_SharedPTR;
    typedef std::remove_extent_t<Type_> Type;
public: // Constructors and destructor.
    SharedPtr() = default;
    SharedPtr(Type* pObj)
    {
        *this = pObj;
    }
    SharedPtr(t_SharedPTR&& shared_ptr) 
    {
        *this = shared_ptr;
    }
    SharedPtr(const t_SharedPTR& shared_ptr)
    {
        *this = shared_ptr;
    }
    ~SharedPtr()
    {
        release();
    }
public: // Assignment.
    t_SharedPTR& operator=(t_SharedPTR&& shared_ptr)
    {
        set_ptr(shared_ptr.get(), shared_ptr.pointer_counter_, false);
        shared_ptr.release();
        return *this;
    }
    t_SharedPTR& operator=(Type* pObject)
    {
	    if (this->ptr_!=pObject)
	    {
            set_ptr(pObject, new PointerCounter, false);
	    }
        return *this;
    }
    t_SharedPTR& operator=(const t_SharedPTR& shared_ptr)
    {
        if (this != &shared_ptr)
        {
            set_ptr(shared_ptr.get(), shared_ptr.pointer_counter_, true);
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
        return get();
    }
    Type* get() const // Return the stored pointer.
    {
        return ptr_;
    }
    TDeleter& get_deleter() // Return a reference to the stored deleter.
    {
        return deleter_;
    }
    explicit operator bool() const // Return false if the stored pointer is null.
    {
        return ptr_;
    }
public: // Modifiers.
    void release() // Release ownership of any stored pointer.
    {
        if(ptr_)
        {
        	if (pointer_counter_)
	        {
		        --(*pointer_counter_);
				if (!(*pointer_counter_))
				{
					deleter_(ptr_);
					delete pointer_counter_;
				}
	            ptr_ = nullptr;
	        }
	        else if (ptr_)
	        {
	            throw std::runtime_error("Pointer without counter");
	        }
        }
    }
    void reset(Type* pObject = nullptr) // Replace the stored pointer.
    {
        *this = pObject;
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
    void set_ptr(Type* ptr, PointerCounter* counter, bool add)
    {
        release();
        ptr_ = ptr;
        pointer_counter_ = counter;
        if(counter&&add)
        {
            pointer_counter_->operator++();
        }
    }
};