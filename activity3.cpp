#include <iostream>

// complex number namespace
namespace Complex 
{
    // complex number class - contains real and imaginary values
    class complex {
        int r, i;
    
        public: 
            // default constructor 
            complex() {
                r = 1;
                i = 1;
            }

            // parameterised constructor
            complex(int g, int h) {
                r = g;
                i = h;
            }

            // getter functions
            int getr() { return r; } 
            int geti() { return i;}
    };
}


int main() {
    using namespace Complex;

    complex c1; // implicit construction
    complex c2(5, 10); // explicit construction

    // output complex numbers
    std::cout << "Complex number 1: " << c1.getr() << " + " << c1.geti() << "i\n";
    std::cout << "Complex number 2: " << c2.getr() << " + " << c2.geti() << "i\n";

    return 0;
}
