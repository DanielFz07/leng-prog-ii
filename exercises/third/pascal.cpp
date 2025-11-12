#include <iostream>

long long factorial(int n) 
{
    long long res = 1;
    for (int i = 2; i <= n; ++i) res *= i;
    return res;
}

long long combinatorio(int n, int k) 
{
    long long num = factorial(n);
    long long den = factorial(k) * factorial(n - k);
    return num / den;
}

int main() 
{
    int n;
    std::cout << "Ingrese el número de filas de la pirámide de Pascal: ";
    std::cin >> n;
    std::cout << '\n';

    for (int i = 0; i < n; ++i) 
    {
        for (int s = 0; s < n - i - 1; ++s) std::cout << ' ';

        for (int k = 0; k <= i; ++k) 
        {
            long long c = combinatorio(i, k);
            std::cout << c << ' ';
        }
        std::cout << '\n';
    }

    return 0;
}
