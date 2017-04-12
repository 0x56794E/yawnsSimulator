#ifndef CANTOR_H
#define CANTOR_H

#include <vector>

/**
 * Implementation of Cantor Paring
 * for simple mapping of two non-neg ints to unique int
 */

//Get the int rep the two given ints
//Assuming num1 >= 0, num2 >= 0
//Negative input will result in unspecified output
int getCode(int num1, int num2);

//The inverse
void getNums(int code, std::vector<int> &nums);

#endif
