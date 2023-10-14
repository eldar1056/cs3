#include <iostream>
#include <sstream>

using namespace std;

class error_t
{
public:
    error_t(string err = "") : message(err) {}

    string what() noexcept
    {
        return message;
    }

private:
    string message;
};

int divide(int x, int y) 
{
    if (y == 0) throw error_t("OVF_ERROR");
    return x / y;
}
