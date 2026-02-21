#include "Identify.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

// Comment this code to disable typeid verification
#include <typeinfo>
static void verify(Base *basePtr)
{
    if (basePtr == 0)
	{
		std::cout << "[TypeInfo] null pointer" << std::endl;
		return ;
	}
	std::cout << "[TypeInfo] ";
	if (typeid(*basePtr) == typeid(A))
		std::cout << "typeid says: A" << std::endl;
	else if (typeid(*basePtr) == typeid(B))
		std::cout << "typeid says: B" << std::endl;
	else if (typeid(*basePtr) == typeid(C))
		std::cout << "typeid says: C" << std::endl;
	else
		std::cout << "typeid says: Unknown" << std::endl;
}

static void runSingleTest()
{
	Base *basePtr;

	basePtr = generate();

	std::cout << "identify(pointer):   ";
	identify(basePtr);
	std::cout << "identify(reference): ";
	identify(*basePtr);
	verify(basePtr); // Comment this line to disable typeid verification
	delete basePtr;
}

int	main(void)
{
	std::srand(std::time(0));

    for (int i = 1; i <= 10; ++i)
    {
        std::cout << "===== Test #" << i << " =====" << std::endl;
        runSingleTest();
    }

	return (0);
}