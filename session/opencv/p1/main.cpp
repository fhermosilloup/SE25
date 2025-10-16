#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main() {
    Mat img = imread("test.jpg");
    if (img.empty()) {
        std::cout << "Image not found!" << std::endl;
        return -1;
    }

    imshow("Display", img);
    waitKey(0);

    return 0;
}
