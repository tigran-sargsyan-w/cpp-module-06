#include "Identify.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

#include <typeinfo>

static void verify(Base *basePtr)
{
	std::cout << "[verify] ";
	if (typeid(*basePtr) == typeid(A))
		std::cout << "typeid says: A" << std::endl;
	else if (typeid(*basePtr) == typeid(B))
		std::cout << "typeid says: B" << std::endl;
	else if (typeid(*basePtr) == typeid(C))
		std::cout << "typeid says: C" << std::endl;
	else
		std::cout << "typeid says: Unknown" << std::endl;
}

int	main(void)
{
	Base *basePtr;

	std::srand(std::time(0));

	basePtr = generate();

	std::cout << "identify(pointer): ";
	identify(basePtr);

	std::cout << "identify(reference): ";
	identify(*basePtr);
    verify(basePtr);
    
	delete basePtr;
	return (0);
}