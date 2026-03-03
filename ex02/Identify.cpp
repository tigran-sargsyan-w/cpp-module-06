#include "Identify.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

/**
 * @brief Creates a random derived instance of Base.
 * @return pointer to A, B, or C.
 */
Base	*generate(void)
{
	int randomValue;

	randomValue = std::rand() % 3;
	if (randomValue == 0)
		return (new A());
	if (randomValue == 1)
		return (new B());
	return (new C());
}

/**
 * @brief Identifies dynamic type from pointer.
 * @param p - pointer to base object.
 */
void	identify(Base *p)
{
	if (p == 0)
		return ;

	if (dynamic_cast<A *>(p) != 0)
		std::cout << "A" << std::endl;
	else if (dynamic_cast<B *>(p) != 0)
		std::cout << "B" << std::endl;
	else if (dynamic_cast<C *>(p) != 0)
		std::cout << "C" << std::endl;
}

/**
 * @brief Identifies dynamic type from reference.
 * @param p - reference to base object.
 */
void	identify(Base &p)
{
	try
	{
		A &refA = dynamic_cast<A &>(p);
		(void)refA;
		std::cout << "A" << std::endl;
		return ;
	}
	catch (...) {}
	try
	{
		B &refB = dynamic_cast<B &>(p);
		(void)refB;
		std::cout << "B" << std::endl;
		return ;
	}
	catch (...) {}
	try
	{
		C &refC = dynamic_cast<C &>(p);
		(void)refC;
		std::cout << "C" << std::endl;
		return ;
	}
	catch (...) {}
}