#include "pch.h"
#include <stdexcept>
#include <crtdbg.h>
#include <utility>
#include <iostream>
#include "../SharedPointer/SharedPtr.h"
#include <memory>

TEST(PointerCounterTest, construct) {
	const PointerCounter counter;
	EXPECT_EQ(counter(), 1);
}

TEST (PointerCounterTest, add)
{
	PointerCounter counter;
	++counter;
	EXPECT_EQ(counter(), 2);
}

TEST(PointerCounterTest, minus)
{
	PointerCounter counter;
	--counter;
	EXPECT_EQ(counter(), 0);
}

TEST(PointerCounterTest, reload)
{
	PointerCounter counter;
	++counter;
	++counter;
	counter.reload();
	EXPECT_EQ(counter(), 1);
}

TEST(PointerCounterTest, expect_if_negative)
{
	PointerCounter counter;
	--counter;
	EXPECT_THROW(--counter, std::runtime_error);
}

TEST(PointerCounterTest, bool_brackets)
{
	PointerCounter counter;
	EXPECT_TRUE(counter);
	--counter;
	EXPECT_TRUE(! counter);
}
class Array final
{
	int* a;
public:
	Array()
	{
		a = new int[1000];
	}
	~Array()
	{
		delete[] a;
	}
};

template<typename Type>
class SimpleDeleter final
{
public:
	void operator()(Type* pointer) const
	{
		delete[] pointer;
	}
};

class MemoryLeaksDetector :public ::testing::Test
{
protected:

	void SetUp()
	{
		_CrtMemCheckpoint(&sturtup_);
	}

	void TearDown()
	{
		_CrtMemState teardown, diff;
		_CrtMemCheckpoint(&teardown);
		ASSERT_EQ(0, _CrtMemDifference(&diff, &sturtup_, &teardown)) << "Memory leaks detected";
	}
	_CrtMemState sturtup_;
};
/*
class TESTMemLeakDetector : public MemoryLeaksDetector {};
TEST_F(TESTMemLeakDetector, new_wo_delete)
{
	int* c = new int[1000];
	c[0] = 1; c[2] = 2;
}
*/
class Constructors : public MemoryLeaksDetector{};
TEST_F(Constructors, void_constr)
{
	SharedPtr<int>a{};
}

TEST_F(Constructors, true_shared)
{
	Array* a = new Array[100];
	std::shared_ptr<Array[]> b(a);
}

TEST_F(Constructors, array_of_classArray)
{
	Array* a = new Array[100];
	SharedPtr<Array[]> b(a);
}

TEST_F(Constructors, several_smart_ptr)
{
	auto a = new Array[100];
	SharedPtr<Array[]> b(a);
	{
		SharedPtr<Array[]> c(b);
		SharedPtr<Array[]>d(b);
	}
}
TEST_F(Constructors, move_constr)
{
	auto a = new Array[100];
	SharedPtr<Array[]> b(a);
	SharedPtr<Array[]>(std::move(b));
}

class Observers: public MemoryLeaksDetector{};
TEST_F(Observers, stored_ptr)
{
	int* a = new int{ 5 };
	SharedPtr<int> b(a);
	auto c = *b;
	EXPECT_EQ(c, 5);
	(*b)++;
	EXPECT_EQ(*b, 6);
}

TEST_F(Observers, stored_ptr_by_get)
{
	int* a = new int{ 5 };
	SharedPtr<int> b(a);
	EXPECT_EQ(b.get(), a);
}

class A
{
public:
	int a_;
};

TEST_F(Observers, stored_ptr_by_operator)
{
	auto a = new A;
	a->a_ = 10;
	SharedPtr<A> b(a);
	b->a_++;
	EXPECT_EQ(a->a_, 11);
}

TEST_F(Observers, bool_brackets)
{
	SharedPtr<int>b;
	EXPECT_TRUE(!b);
	SharedPtr<int>c{ new int };
	EXPECT_TRUE(c);
}

class Modifiers: public MemoryLeaksDetector{};

TEST_F(Modifiers, release)
{
	auto a = new Array[100];
	SharedPtr<Array[]> b(a);
	b.release();
	EXPECT_TRUE(!b);
}

TEST_F(Modifiers, release_empty)
{
	SharedPtr<int>b{};
	b.release();
	EXPECT_TRUE(!b);
}
class Deleter : public MemoryLeaksDetector{};
TEST_F(Deleter, execute)
{
	auto a = new Array[100];
	SharedPtr<Array[], SimpleDeleter<Array>> b(a);
}

TEST_F(Deleter, get)
{
	auto a = new Array[100];
	SharedPtr<Array[], SimpleDeleter<Array>> b(a);
	SimpleDeleter<Array> simpl_del = {};
	EXPECT_EQ(typeid(b.get_deleter()).hash_code(), typeid(simpl_del).hash_code());
}