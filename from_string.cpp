#include <iostream>
#include <sstream>

using namespace std;

class bad_from_string : public exception
{
public:
    bad_from_string(string from = "") : exception(string("Bad conversion from string " + from).c_str()) {}
};

template <typename T>
T from_string(const string& from)
{
    istringstream stream(from);

    T to;
    stream >> std::noskipws >> to;

    if (!stream.eof())
        throw bad_from_string(from);

    return to;
}
