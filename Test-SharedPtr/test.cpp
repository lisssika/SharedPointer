#include "pch.h"
#include <stdexcept>
#include "../SharedPointer/SharedPtr.h"

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

