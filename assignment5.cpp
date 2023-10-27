#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    Mat finger_print = imread("/Users/oyeeun/Desktop/installation/test/finger_print.png");
    Mat adaptive_1 = imread("/Users/oyeeun/Desktop/installation/test/adaptive_1.jpg");
    Mat adaptive = imread("/Users/oyeeun/Desktop/installation/test/adaptive.png");
    
    cvtColor(finger_print, finger_print, COLOR_BGR2GRAY);
    cvtColor(adaptive_1, adaptive_1, COLOR_BGR2GRAY);
    cvtColor(adaptive, adaptive, COLOR_BGR2GRAY);

    threshold(finger_print, finger_print, 0, 255, THRESH_BINARY | THRESH_OTSU);
    adaptiveThreshold(adaptive_1, adaptive_1, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 21, 10);
    adaptiveThreshold(adaptive, adaptive, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 7, 10);

    imshow("finger_print", finger_print);
    imshow("adaptive_1", adaptive_1);
    imshow("adaptive", adaptive);

    waitKey(0);

}