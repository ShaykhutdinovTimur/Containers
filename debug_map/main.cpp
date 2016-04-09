
#include <iostream>
#include <string>
#include "debug_map.h"
#include <cstdlib>
#include <time.h>
#include <map>
#include <algorithm>
#include <map>
#include <assert.h>
#include <set>

using namespace std;

std::string gen_rand_str(int length) {
    int r = rand() % 1000;
    std::string result;
    while(length > 0) {
        result += 'a' + r % 10;
        r /= 10;
        length--;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::string gen_by_number(int number) {
    std::string result;
    number++;
    while(number > 0) {
        result += 'a' + number % 10;
        number /= 10;
    }
    return result;
}



int main() {
    my::debug_map<int, int> debug_map;
    std::map<int, int> map;
    for(int i = 0; i < 5000; i++) {
        if(!debug_map.check_tree()) {
            return 1337;
        }
        int random1 = rand() % 100;
        int random2 = rand() % 100;
        std::cout << "iteration: " << random1 << " " << random2 << std::endl;
        bool contains1 = debug_map.find(random1) != debug_map.end();
        if(contains1) {
            int res = (debug_map.find(random1)).node->value;
            if (map[random1] != res) {
                std::cout << "FAIL";
                return 1;
            }
            debug_map.erase(random1);
            continue;
        }
        debug_map.insert(random1, random2);
        map[random1] = random2;
    }
    std::cout << "OK";
    return 0;
}




