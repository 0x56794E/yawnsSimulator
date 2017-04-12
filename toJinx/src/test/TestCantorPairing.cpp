#include <stdio.h>      /* printf, fopen */
#include <vector>

#include "CantorPairing.h"

int main(int argc, char** argv)
{
	printf("Testing cantor pairing\n");
	int num1 = 47;
	int num2 = 32;

	int code = getCode(num1, num2);
	printf("code = %d\n", code);

	//Inverse
	std::vector<int> nums;
	getNums(code, nums);
	printf("num1 = %d, num2 = %d\n", nums.at(0), nums.at(1));


	return 0;
}
