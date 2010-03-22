#ifndef __Exception__
#define __Exception__

#include <iostream>
#include <exception>
#include <string>

using namespace std;

#define EXCEPTION_HEADER    "Exception Raised:"

class Exception: public exception {

private:
	string error;

public:

	Exception() :
		exception() {
		this->error = "uncaught exception";
	}

	Exception(string e) :
		exception() {
		this->error = e;
	}

	~Exception() throw () {

	}

	const char* what() {
		return (this->error.c_str());
	}

	const char* toString() {
		return (this->error.c_str());
	}

	void printStackTrace() {

		std::cout << EXCEPTION_HEADER << this->error << std::endl;
		;

	}

};

#endif
