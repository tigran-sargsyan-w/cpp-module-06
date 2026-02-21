#include "Identify.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

int	main(void)
{
	Base *basePtr;

	std::srand(std::time(0));

	basePtr = generate();

	std::cout << "identify(pointer): ";
	identify(basePtr);

	std::cout << "identify(reference): ";
	identify(*basePtr);

	delete basePtr;
	return (0);
}