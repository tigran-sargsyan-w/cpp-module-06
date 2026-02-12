#include "ScalarConverter.hpp"

#include <iostream>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <cerrno>
#include <cctype>
#include <cmath>

static bool isPseudoLiteral(const std::string &s)
{
	return (s == "nan" || s == "+inf" || s == "-inf"
		|| s == "nanf" || s == "+inff" || s == "-inff");
}

static bool isQuotedCharLiteral(const std::string &s)
{
	return (s.size() == 3 && s[0] == '\'' && s[2] == '\'');
}

static bool isSingleCharLiteral(const std::string &s)
{
	return (s.size() == 1 && !std::isdigit(static_cast<unsigned char>(s[0])));
}

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

ScalarConverter::ScalarConverter() {}
ScalarConverter::ScalarConverter(const ScalarConverter &other) { (void)other; }
ScalarConverter &ScalarConverter::operator=(const ScalarConverter &other)
{
	(void)other;
	return *this;
}
ScalarConverter::~ScalarConverter() {}

void ScalarConverter::convert(const std::string &literal)
{
	double value = 0.0;

	if (literal.empty())
	{
		std::cout << "char: impossible" << std::endl;
		std::cout << "int: impossible" << std::endl;
		std::cout << "float: impossible" << std::endl;
		std::cout << "double: impossible" << std::endl;
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
		else
			value = std::numeric_limits<double>::infinity();
	}
	else if (isIntLiteral(literal))
	{
		double parsed = 0.0;
		if (!parseToDouble(literal, parsed))
		{
			std::cout << "char: impossible" << std::endl;
			std::cout << "int: impossible" << std::endl;
			std::cout << "float: impossible" << std::endl;
			std::cout << "double: impossible" << std::endl;
			return;
		}
		value = parsed;
	}
	else if (isFloatLiteral(literal))
	{
		std::string core = literal.substr(0, literal.size() - 1);
		double parsed = 0.0;
		if (!parseToDouble(core, parsed))
		{
			std::cout << "char: impossible" << std::endl;
			std::cout << "int: impossible" << std::endl;
			std::cout << "float: impossible" << std::endl;
			std::cout << "double: impossible" << std::endl;
			return;
		}
		value = parsed;
	}
	else if (isDoubleLiteral(literal))
	{
		double parsed = 0.0;
		if (!parseToDouble(literal, parsed))
		{
			std::cout << "char: impossible" << std::endl;
			std::cout << "int: impossible" << std::endl;
			std::cout << "float: impossible" << std::endl;
			std::cout << "double: impossible" << std::endl;
			return;
		}
		value = parsed;
	}
	else
	{
		std::cout << "char: impossible" << std::endl;
		std::cout << "int: impossible" << std::endl;
		std::cout << "float: impossible" << std::endl;
		std::cout << "double: impossible" << std::endl;
		return;
	}

	printChar(value);
	printInt(value);
	printFloat(value);
	printDouble(value);
}
