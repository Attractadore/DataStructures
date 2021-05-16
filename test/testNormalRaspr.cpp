#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>

int main()
{
    /*int E, D;
    cin >> E >> D;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<> X(E, D);
    std::vector<int> data(10);
    for (int& var : data) {
        var = X(gen);
        std::cout << var << '\n';
    }
    */
    int n = 15;
    for(i = 0; i < n; i++)
        std::cout<< PoissonKnuth(10)<<'\n';
}

double PoissonKnuth(double rate) {
    double k = 0, prod = Uniform(0, 1);
    while (prod > exp(-rate) {
        prod *= Uniform(0, 1);
        ++k;
    }
    return k;
}