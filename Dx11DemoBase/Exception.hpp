#pragma once

#include <exception>
#include <string>

class Exception : public std::exception {
public:
	// Copy message into string.
	Exception(std::string message)
		: errorMessage(message) { }

	virtual ~Exception() noexcept { }

	virtual const char * what() const noexcept {
		return errorMessage.c_str();
	}

private:
	std::string errorMessage;

};
