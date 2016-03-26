#include <iostream>
#include <sstream>
#include <stdlib.h>

#include "myvector.h"

#include <vector>

using namespace std;

void Test1() {
    std::cout << "Starting Test1" << std::endl;
    vector<int> vv;
    my::my_vector<int> v;
    for (int i = 0; i < 50; ++i) {
        v.push_back(i);
        vv.push_back(i);
    }
    std::cout << "Test1 results";

    for (size_t i = 0; i < v.size(); ++i) {
        if (v[i] != vv[i]) {
            std::cout << " FAIL" << std::endl;
            return;
        }
    }
    std::cout << " OK" << std::endl;
}

void Test2() {
    std::cout << "Starting Test2" << std::endl;
    my::my_vector<int> v;
    for (int i = 0; i < 50; ++i) {
        v.push_back(i);
    }
    for (int i = 0; i < 50; ++i) {
        v.push_front(i);
    }
    std::cout << "Test2 results";

    for (int i = 0; i < 100; i++) {
        v.pop_back();
    }
    if (v.empty()) {
        std::cout << " OK" << std::endl;
    } else {
        std::cout << " FAIL" << std::endl;
    }
}

void Test3() {
    std::cout << "Starting Test3" << std::endl;
    my::my_vector<int> v;
    for (int i = 0; i < 50; ++i) {
        v.push_back(i);
    }

    std::cout << "Test3 results";

    for (int i = 0; i < 50; i++) {
        v.pop_front();
    }

    if (v.empty()) {
        std::cout << " OK" << std::endl;
    } else {
        std::cout << " FAIL" << std::endl;
    }
}

void Test4() {
    std::cout << "Starting Test4" << std::endl;

    std::cout << "Test4 results";
    my::my_vector<int> v1;
    v1.reserve(10);
    for (int i = 0; i < 51; ++i) {
        v1.push_back(i);
    }
    v1.pop_back();
    if (v1.back() == 49) {
        std::cout << " OK" << std::endl;
    } else {
        std::cout << " FAIL" << std::endl;
    }
}

int main() {
    Test1();
    Test2();
    Test3();
    Test4();
}
