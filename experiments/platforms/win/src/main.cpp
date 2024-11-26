#include <iostream>

#include "framework.h";
#include "engine/test.hpp"

int main()
{
	std::cout << "Hello Project" << std::endl;

	testFramework();
	testEngine();

	return 0;
}