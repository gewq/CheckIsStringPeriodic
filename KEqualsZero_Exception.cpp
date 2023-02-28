#include "KEqualsZero_Exception.h"



KEqualsZero_Exception::KEqualsZero_Exception() : std::exception()
{
}



const char* KEqualsZero_Exception::what() const noexcept
{
	return "Error: K = 0!";
}