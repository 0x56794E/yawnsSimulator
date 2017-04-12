#include "CantorPairing.h"

#include <math.h> //Floor function

int getCode(int num1, int num2)
{
	return (num1 + num2) * (num1 + num2 + 1) / 2 + num2;
}

void getNums(int code, std::vector<int> &nums)
{
	//w = x + y = fl( (sqrt(8*code + 1) - 1)/2)
	//t = (w^2 + w)/2
	//code = z = t + y

	//y = z - t
	//x = w - y

	int w = floor((sqrt(code * 8 + 1) - 1) / 2);
	int num2 = code - (pow(w, 2) + w) / 2;
	int num1 = w - num2;
	nums.push_back(num1);
	nums.push_back(num2);
}
