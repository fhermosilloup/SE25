#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
	// Matriz 3x3 con elementos tipo int (CV_32S)
	int data_int[] = {1,2,3,4,5,6,7,8,9};
	cv :: Mat  m1(3,3,CV_32S, data_int);

	// Matriz 1x3 con elementos tipo uint8_t (CV_8UC)
	unsigned char data[] = {200,120,30};
	cv :: Mat  m2(1,3,CV_8U,data);

	std::cout << "M3x3:" << std::endl << m1 << std::endl;
	std::cout << "M1x3:" << std::endl << m2 << std::endl;

	return 0;
}
