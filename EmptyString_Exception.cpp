#include "EmptyString_Exception.h"



EmptyString_Exception::EmptyString_Exception() : std::exception()
{
}



const char* EmptyString_Exception::what() const noexcept
{
	return "Error: Input string is empty!";
}