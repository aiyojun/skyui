#include "utils.h"
#include <random>
#include <ctime>

std::string generate_uuid(int n) {
    std::string uuid;
    static std::default_random_engine e(time(nullptr));
    static std::uniform_int_distribution<int> u(0,15);
    char mapper[] = {
            '0', '1', '2', '3', '4', '5', '6', '7',
            '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    for (int i = 0; i < n; i++) {
        uuid += mapper[u(e)];
    }
    return uuid;
}