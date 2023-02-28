#include "KExceedsStringSize_Exception.h"



KExceedsStringSize_Exception::KExceedsStringSize_Exception(size_t K,
	size_t stringSize) : std::exception(),
						 errorMessage_("Error: K (" + std::to_string(K) +
						 ") > string_size (" + std::to_string(stringSize) + ")")
{
}



const char* KExceedsStringSize_Exception::what() const noexcept
{
	return errorMessage_.c_str();
}