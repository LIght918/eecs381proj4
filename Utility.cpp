#include "Utility.h"
#include <string>

using namespace std;

// checks if the string is long enough to be shortened
void check_string_length(std::string& a)
{
    if (a.size() < SHORTEN_NAME_LENGTH) throw Error("Name is too short!");
}

// returns a string of the first two characters in the given string
string shorten_string(string &a)
{
    check_string_length(a);
    return a.substr(0, SHORTEN_NAME_LENGTH);
}