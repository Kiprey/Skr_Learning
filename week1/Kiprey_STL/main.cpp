#include "stl_vector.h"
#include <iostream>
#include <string>

using namespace std;
using __STL_NAME::vector;

class test
{
private:
    string* _ptr;
public:
    test(string id = "-1")
    {
        _ptr = new string[10];
        for (int i = 0; i < 10; i++)
            _ptr[i] = id;
        cout << _ptr[0] << " constructed." << endl;
    }
    void print()
    {
        // UAF
        cout << _ptr[0] << " printed." << endl;
    }
    ~test()
    {
        cout << _ptr[0] << " destroyed." << endl;
        delete[] _ptr;
    }
};

int main()
{
    vector<test> test1;
    test1.push_back(test("test"));
    test1.begin()->print();

    return 0;
}