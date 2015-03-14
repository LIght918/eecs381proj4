#ifndef UTILITIES_H
#define UTILITIES_H

#include <exception>
#include <string>

class Error : public std::exception {
public:
	Error(const char* msg_) : msg(msg_) {}
    const char* what() const noexcept override
        {return msg;}
private:
	const char* msg;
};

/* add any of your own declarations here */
const double REFUEL_MIN = .005;

// length used by model and view to represent names
const int SHORTEN_NAME_LENGTH = 2;

// checks if the string is long enough to be shortened
void check_string_length(std::string& a);

// returns a string of the first two characters in the given string
std::string shorten_string(std::string& a);

#endif
