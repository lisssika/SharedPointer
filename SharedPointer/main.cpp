#include "SharedPtr.h"
#include <memory>
#include <iostream>

int main()
{
	int* a = new int{5};
	//int a[5] = { 1, 2, 3,4, 5 };
	SharedPtr<int[]> b(a);
	auto c = *b;
	(*b)++;
	std::cout << c << " " << *b;
		//std::shared_ptr<int[]> b(a);
}