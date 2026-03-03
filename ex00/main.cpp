#include "ScalarConverter.hpp"
#include <iostream>

/**
 * @brief Entry point for scalar converter program.
 * @param argc - argument count.
 * @param argv - argument vector.
 * @return process exit code.
 */
int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: ./convert <literal>" << std::endl;
		return 1;
	}
	ScalarConverter::convert(argv[1]);
	return 0;
}
