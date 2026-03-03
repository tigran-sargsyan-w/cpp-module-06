#include "Serializer.hpp"

/**
 * @brief Default constructor.
 */
Serializer::Serializer() {}

/**
 * @brief Copy constructor.
 * @param other - source instance.
 */
Serializer::Serializer(const Serializer &other) { (void)other; }

/**
 * @brief Destructor.
 */
Serializer::~Serializer() {}

/**
 * @brief Copy assignment operator.
 * @param other - source instance.
 * @return assigned instance.
 */
Serializer &Serializer::operator=(const Serializer &other)
{
	(void)other;
	return (*this);
}

/**
 * @brief Converts Data pointer to raw integer value.
 * @param ptr - pointer to serialize.
 * @return serialized uintptr_t value.
 */
uintptr_t Serializer::serialize(Data *ptr)
{
	return (reinterpret_cast<uintptr_t>(ptr));
}

/**
 * @brief Converts raw integer value back to Data pointer.
 * @param raw - raw integer representation.
 * @return deserialized Data pointer.
 */
Data *Serializer::deserialize(uintptr_t raw)
{
	return (reinterpret_cast<Data *>(raw));
}
