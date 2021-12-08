#include "SharedPtr.h"
#include <memory>

int main()
{
	int* a = new int[5];
	//int a[5] = { 1, 2, 3,4, 5 };
		SharedPtr<int[]> b(a);
		//std::shared_ptr<int[]> b(a);
}