#include <iostream>

using namespace std;

extern "C" {
    double average(double, double);
}

int main() {
    cout << average(6.5, 2.1) << endl;
}