#include "SharedPtr.h"
#include <memory>
#include <iostream>

int main()
{
	int* a = new int{5};
	//int a[5] = { 1, 2, 3,4, 5 };
	SharedPtr<int> b(a);
	int* c = new int[6];
	SharedPtr<int[]>d(c);
}