#include "ScalarConverter.hpp"
#include <iostream>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <cerrno>
#include <cctype>
#include <cmath>

/**
 * @brief Checks whether a literal is a pseudo floating value.
 * @param s - input literal.
 * @return true if literal is nan/inf variant.
 */
static bool isPseudoLiteral(const std::string &s)
{
	return (s == "nan" || s == "+inf" || s == "-inf"
		|| s == "nanf" || s == "+inff" || s == "-inff");
}

/**
 * @brief Checks whether literal is in quoted char form.
 * @param s - input literal.
 * @return true if literal looks like 'c'.
 */
static bool isQuotedCharLiteral(const std::string &s)
{
	return (s.size() == 3 && s[0] == '\'' && s[2] == '\'');
}

/**
 * @brief Checks whether literal is a single non-digit character.
 * @param s - input literal.
 * @return true if literal is a single character.
 */
static bool isSingleCharLiteral(const std::string &s)
{
	return (s.size() == 1 && !std::isdigit(static_cast<unsigned char>(s[0])));
}

/**
 * @brief Validates integer literal syntax.
 * @param s - input literal.
 * @return true if literal is a signed/unsigned integer.
 */
static bool isIntLiteral(const std::string &s)
{
	std::size_t i = 0;

	if (s.empty())
		return false;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (i >= s.size())
		return false;
	for (; i < s.size(); i++)
	{
		if (!std::isdigit(static_cast<unsigned char>(s[i])))
			return false;
	}
	return true;
}

/**
 * @brief Validates float literal syntax with trailing f.
 * @param s - input literal.
 * @return true if literal is a valid float token.
 */
static bool isFloatLiteral(const std::string &s)
{
	if (s.size() < 2)
		return false;
	if (s[s.size() - 1] != 'f')
		return false;

	std::string core = s.substr(0, s.size() - 1);
	if (core.empty())
		return false;

	std::size_t i = 0;
	bool hasDot = false;
	bool hasDigit = false;

	if (core[i] == '+' || core[i] == '-')
		i++;
	if (i >= core.size())
		return false;

	for (; i < core.size(); i++)
	{
		unsigned char ch = static_cast<unsigned char>(core[i]);
		if (std::isdigit(ch))
		{
			hasDigit = true;
			continue;
		}
		if (core[i] == '.' && !hasDot)
		{
			hasDot = true;
			continue;
		}
		return false;
	}
	return (hasDigit && hasDot);
}

/**
 * @brief Validates double literal syntax.
 * @param s - input literal.
 * @return true if literal is a valid double token.
 */
static bool isDoubleLiteral(const std::string &s)
{
	if (s.empty())
		return false;

	std::size_t i = 0;
	bool hasDot = false;
	bool hasDigit = false;

	if (s[i] == '+' || s[i] == '-')
		i++;
	if (i >= s.size())
		return false;

	for (; i < s.size(); i++)
	{
		unsigned char ch = static_cast<unsigned char>(s[i]);
		if (std::isdigit(ch))
		{
			hasDigit = true;
			continue;
		}
		if (s[i] == '.' && !hasDot)
		{
			hasDot = true;
			continue;
		}
		return false;
	}
	return (hasDigit && hasDot);
}

/**
 * @brief Prints char conversion result.
 * @param value - numeric value to convert.
 * @return nothing.
 */
static void printChar(double value)
{
	std::cout << "char: ";
	if (std::isnan(value) || std::isinf(value))
	{
		std::cout << "impossible" << std::endl;
		return;
	}

	if (value < static_cast<double>(std::numeric_limits<unsigned char>::min())
		|| value > static_cast<double>(std::numeric_limits<unsigned char>::max()))
	{
		std::cout << "impossible" << std::endl;
		return;
	}

	unsigned char ch = static_cast<unsigned char>(value);
	if (ch < 32 || ch > 126)
	{
		std::cout << "Non displayable" << std::endl;
		return;
	}
	std::cout << "'" << static_cast<char>(ch) << "'" << std::endl;
}

/**
 * @brief Prints int conversion result.
 * @param value - numeric value to convert.
 * @return nothing.
 */
static void printInt(double value)
{
	std::cout << "int: ";
	if (std::isnan(value) || std::isinf(value))
	{
		std::cout << "impossible" << std::endl;
		return;
	}
	if (value < static_cast<double>(std::numeric_limits<int>::min())
		|| value > static_cast<double>(std::numeric_limits<int>::max()))
	{
		std::cout << "impossible" << std::endl;
		return;
	}
	std::cout << static_cast<int>(value) << std::endl;
}

/**
 * @brief Prints float conversion result.
 * @param value - numeric value to convert.
 * @return nothing.
 */
static void printFloat(double value)
{
	std::cout << "float: ";
	if (std::isnan(value))
	{
		std::cout << "nanf" << std::endl;
		return;
	}
	if (std::isinf(value))
	{
		if (value < 0)
			std::cout << "-inff" << std::endl;
		else
			std::cout << "+inff" << std::endl;
		return;
	}

	double maxFloat = static_cast<double>(std::numeric_limits<float>::max());
	if (value < -maxFloat || value > maxFloat)
	{
		std::cout << "impossible" << std::endl;
		return;
	}

	std::cout << std::fixed << std::setprecision(1)
			  << static_cast<float>(value) << "f" << std::endl;
}

/**
 * @brief Prints double conversion result.
 * @param value - numeric value to convert.
 * @return nothing.
 */
static void printDouble(double value)
{
	std::cout << "double: ";
	if (std::isnan(value))
	{
		std::cout << "nan" << std::endl;
		return;
	}
	if (std::isinf(value))
	{
		if (value < 0)
			std::cout << "-inf" << std::endl;
		else
			std::cout << "+inf" << std::endl;
		return;
	}
	std::cout << std::fixed << std::setprecision(1) << value << std::endl;
}

/**
 * @brief Prints impossible conversion for all scalar types.
 * @return nothing.
 */
static void printAllImpossible()
{
	std::cout << "char: impossible" << std::endl;
	std::cout << "int: impossible" << std::endl;
	std::cout << "float: impossible" << std::endl;
	std::cout << "double: impossible" << std::endl;
}

/**
 * @brief Parses string to double using strtod.
 * @param s - source string.
 * @param outValue - parsed result.
 * @return true on successful parse.
 */
static bool parseToDouble(const std::string &s, double &outValue)
{
	errno = 0;
	char *endPtr = 0;

	outValue = std::strtod(s.c_str(), &endPtr);
	if (errno == ERANGE)
		return false;
	if (endPtr == s.c_str())
		return false;

	while (*endPtr)
	{
		if (!std::isspace(static_cast<unsigned char>(*endPtr)))
			return false;
		endPtr++;
	}
	return true;
}

/**
 * @brief Parses a value or prints impossible output.
 * @param s - source literal.
 * @param outValue - parsed result.
 * @return true if parse succeeded.
 */
static bool tryParseOrFail(const std::string &s, double &outValue)
{
	if (!parseToDouble(s, outValue))
	{
		printAllImpossible();
		return false;
	}
	return true;
}

/**
 * @brief Default constructor.
 */
ScalarConverter::ScalarConverter() {}

/**
 * @brief Copy constructor.
 * @param other - source instance.
 */
ScalarConverter::ScalarConverter(const ScalarConverter &other) { (void)other; }

/**
 * @brief Copy assignment operator.
 * @param other - source instance.
 * @return assigned instance.
 */
ScalarConverter &ScalarConverter::operator=(const ScalarConverter &other)
{
	(void)other;
	return *this;
}

/**
 * @brief Destructor.
 */
ScalarConverter::~ScalarConverter() {}

/**
 * @brief Converts a literal and prints all scalar representations.
 * @param literal - input literal string.
 * @return nothing.
 */
void ScalarConverter::convert(const std::string &literal)
{
	double value = 0.0;

	if (literal.empty())
	{
		printAllImpossible();
		return;
	}

	if (isQuotedCharLiteral(literal))
	{
		value = static_cast<double>(literal[1]);
	}
	else if (isSingleCharLiteral(literal))
	{
		value = static_cast<double>(literal[0]);
	}
	else if (isPseudoLiteral(literal))
	{
		if (literal == "nan" || literal == "nanf")
			value = std::numeric_limits<double>::quiet_NaN();
		else if (literal == "-inf" || literal == "-inff")
			value = -std::numeric_limits<double>::infinity();
		else if (literal == "+inf" || literal == "+inff")
			value = std::numeric_limits<double>::infinity();
	}
	else if (isIntLiteral(literal))
	{
		if (!tryParseOrFail(literal, value))
			return;
	}
	else if (isFloatLiteral(literal))
	{
		std::string core = literal.substr(0, literal.size() - 1);
		if (!tryParseOrFail(core, value))
			return;
	}
	else if (isDoubleLiteral(literal))
	{
		if (!tryParseOrFail(literal, value))
			return;
	}
	else
	{
		printAllImpossible();
		return;
	}

	printChar(value);
	printInt(value);
	printFloat(value);
	printDouble(value);
}
