#include <iostream>
#include <cstring>
#include "Serializer.hpp"

/**
 * @brief Prints Data struct fields.
 * @param ptr - pointer to Data instance.
 */
static void printData(const Data *ptr)
{
	std::cout << "Data address: " << ptr << "\n";
	std::cout << "id: " << ptr->id << "\n";
	std::cout << "name: " << ptr->name << "\n";
	std::cout << "score: " << ptr->score << "\n";
}

/**
 * @brief Entry point for serialization demo.
 * @return process exit code.
 */
int main(void)
{
	Data dataInstance;

	dataInstance.id = 42;
	dataInstance.score = 1337.5;

	std::strncpy(dataInstance.name, "Username", sizeof(dataInstance.name));
	dataInstance.name[sizeof(dataInstance.name) - 1] = '\0';

	Data *originalPtr = &dataInstance;

	std::cout << "=== Original ===\n";
	printData(originalPtr);

	uintptr_t rawValue = Serializer::serialize(originalPtr);
	std::cout << "\nSerialized (uintptr_t): " << rawValue << "\n";

	Data *restoredPtr = Serializer::deserialize(rawValue);

	std::cout << "\n=== Restored ===\n";
	printData(restoredPtr);

	std::cout << "\nPointer equality check: ";
	if (restoredPtr == originalPtr)
		std::cout << "OK (same address)\n";
	else
		std::cout << "FAIL (different address)\n";

	return (0);
}
