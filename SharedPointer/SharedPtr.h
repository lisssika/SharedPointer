#pragma once
#include <stdexcept>
class PointerCounter
{
    size_t n_;
public:
    PointerCounter():n_(1){}
    void reload()
    {
        n_ = 1;
    }
    size_t operator()() const { return n_; }
    size_t operator++()
    {
        n_++;
        return n_;
    }
    size_t operator--()
    {
        if (n_ == 0)
        {
            throw std::runtime_error(" There is no any pointer. It is impossible to reduce the PointerCounter");
        }
        n_--;
        return n_;
    }

    explicit operator bool() const
    {
	    return (n_);
    }
};

template<class Type, class TDeleter>
class SharedPtr {
    typedef SharedPtr<Type, TDeleter> t_SharedPTR;
public: // Constructors and destructor.
    SharedPtr(): pointer_counter_(new PointerCounter), ptr_(nullptr){}
    SharedPtr(Type* pObj) :pointer_counter_(new PointerCounter), ptr_(pObj)
    {
        pointer_counter_->operator++();
	    //pObj = nullptr; /* std-шный так не делает, но мне почему-то кажетс€ логичным перемещать указатель. подумаю ещЄ*/
    }
    SharedPtr(t_SharedPTR&& uniquePTR) noexcept:pointer_counter_(uniquePTR.pointer_counter_), ptr_(uniquePTR.ptr_) // Move constructor.
    {
        uniquePTR.pointer_counter_ = nullptr; // аааа не понимаю. ћб тут нужен просто новый, равный 0???
        uniquePTR.ptr_ = nullptr;
    }
    SharedPtr(const t_SharedPTR& shared_ptr):pointer_counter_(shared_ptr.pointer_counter_), ptr_(shared_ptr.ptr_) {
        pointer_counter_->operator++();
    }
    ~SharedPtr()
    {
        pointer_counter_->operator--();
        if(!*pointer_counter_)
        {
            delete ptr_;
            delete pointer_counter_;
        }
    }
public: // Assignment.
    t_SharedPTR& operator=(t_SharedPTR&& sharedPTR)
    {
        pointer_counter_->operator--();
        if (!pointer_counter_)
        {
            delete ptr_;
        }
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
            pointer_counter_->operator--();
            if (!pointer_counter_)
            {
                delete ptr_;
            }
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
        	pointer_counter_->operator--();
	        if (!pointer_counter_)
	        {
	            delete ptr_;
	        }
	        ptr_ = sharedPTR.ptr_;
	        pointer_counter_ = sharedPTR.pointer_counter_;
            pointer_counter_->operator++();
        }
        
        return *this;
    }

public: // Observers.
    Type& operator*() const // Dereference the stored pointer.
    {
        return ptr_;
    }
    Type* operator->() const; // Return the stored pointer.
    Type* get() const; // Return the stored pointer.
    TDeleter& get_deleter(); // Return a reference to the stored deleter.
    operator bool() const; // Return false if the stored pointer is null.
public: // Modifiers.
    void release(); // Release ownership of any stored pointer.
    void reset(Type* pObject = nullptr); // Replace the stored pointer.
    void swap(t_SharedPTR& sharedPTR) // Exchange the pointer with another object.
    {
        Type tmp{ std::move(sharedPTR) };
        sharedPTR = std::move(*this);
        *this = std::move(tmp);
    }
private:
    PointerCounter* pointer_counter_; // так и т€нет тут сделать smart pointer, но это странно использовать их в этой задаче, наверное
    Type* ptr_;
    TDeleter deleter_; //?????????
};