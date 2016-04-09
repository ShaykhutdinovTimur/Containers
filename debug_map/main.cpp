
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

std::string gen_rand_str(int length)
{
    int r = rand() % 1000;
    std::string result;
    while(length > 0)
    {
        result += 'a' + r % 10;
        r /= 10;
        length--;
    }
    std::reverse(result.begin(), result.end());
    return result;
}

std::string gen_by_number(int number)
{
    std::string result;
    number++;
    while(number > 0)
    {
        result += 'a' + number % 10;
        number /= 10;
    }
    return result;
}



int main() {
    bool inserted;
    bool half_contain = false;
    int insert_count = 0;
    int erase_count = 0;
    my::debug_map<int, int> debug_map;
    map<int, int> left_to_right;
    map<int, int> right_to_left;
    for(int i = 0; i < 5000; i++)
    {
        if(!debug_map.check_tree())
        {
            return 1337;
        }
        int random1 = rand() % 100;
        int random2 = rand() % 100;
        std::cout << "iteration: " << random1 << " " << random2 << std::endl;
        bool contains1;
        bool contains2;
        auto my_l = debug_map.find(random1);
        auto my_r = debug_map.find(random2);
        auto ltr = left_to_right.find(random1);
        auto rtl = right_to_left.find(random2);



        contains1 = debug_map.find(random1) != debug_map.end();
        contains2 = debug_map.find(random2) != debug_map.end();
        if(contains1)
        {
            std::cout << "key repeat" << random1 << std::endl;
            debug_map.erase(random1);
            erase_count++;
            inserted = false;
            continue;
        }
        if(contains2)
        {

            std::cout << "value repeat" << random2 << std::endl;
            debug_map.erase(random2);
            erase_count++;
            inserted = false;
            continue;
        }
        debug_map.insert(random1, random2);
        left_to_right[random1] = random2;
        right_to_left[random2] = random1;
        inserted = true;
        insert_count++;
    }

    //part2

    my::debug_map<int, int> second_debug_map;

    set<int> random_values;

    for(int i = 0; i < 100; i++)
    {
        random_values.insert(rand() % 30);
    }

    while(random_values.size() % 2 == 1)
    {
        random_values.insert(rand() % 30);
    }

    for(auto it = random_values.begin(); it != random_values.end(); it++)
    {
        int random1 = *it;
        it++;
        int random2 = *it;
        second_debug_map.insert(random1, random2);
    }

    std::cout << "iterating left" << std::endl;
    for(auto it = second_debug_map.begin(); it != second_debug_map.end(); it++)
    {
        std::cout << *it << std::endl;
    }
    std::cout << "iterating right" << std::endl;
    for(auto it = second_debug_map.begin(); it != second_debug_map.end(); it++)
    {
        std::cout << *it << std::endl;
    }



    return 0;
}




