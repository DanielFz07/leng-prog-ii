

#include <iostream>

int main() 
{
	int n;
	std::cout << "Ingrese la altura del triángulo: ";
	std::cin >> n;

	for (int i = n; i >= 1; --i) 
    {
		int spaces = n - i;
		int stars = 2 * i - 1;

		for (int c = 0; c < spaces + stars; ++c) 
        {
			if (c < spaces) std::cout << ' ';
			else std::cout << '*';
		}
		std::cout << '\n';
	}

	return 0;
}

