#include <iostream>

int my_pow(int currA, int currN) {
    if (currA == 0) { return 0; }
    int product = 1;

    while (currN) {
        if (currN % 2) {
            product *= currA;
        }

        currA = currA * currA;
        currN >>= 1;
    }

    return product;
}

int my_pow1(int a, int n) {
    if (a == 0) { return 0; }
    
    int product = 1;
    while (n--) {
        product *= a;
    }

    return product;
}

int main() {
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < 1000; ++j) {
            int p1 = my_pow(i, j);
            int p2 = my_pow1(i, j);
            if (p1 != p2) {
                std::cout << i << "^" << j << " = " << p1 << " / " << p2 << std::endl;
            }
        }
    }

    std::cout << "Done" << std::endl;
    return 0;
}
