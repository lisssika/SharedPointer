#pragma once
#include <type_traits>

template<class Type>
class IDeleter
{
	typedef std::remove_extent_t<Type> T;
public:
	virtual ~IDeleter() = default;
	virtual void operator()(T* ptr)const = 0;
};

template<class Type>
class DefaultDeleter final : public IDeleter<Type>
{
	typedef std::remove_extent_t<Type> T;
public:
	virtual void operator()(T* ptr)const override
	{
		if (std::is_array<Type>::value)
		{
			delete[] ptr;
		}
		else
		{
			delete ptr;
		}
	}
};