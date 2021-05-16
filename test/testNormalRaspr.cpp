#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>

int main()
{
    int E, D, n;
    std::cin >> E >> D >> n;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> X(E, D);
    std::vector<int> data(n);
    std::cout << n << '\n';
    for (int& var : data) {
        var = X(gen);
        std::cout << var << '\n';
    }

}

