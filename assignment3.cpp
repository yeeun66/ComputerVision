#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    Mat image1, image2, origin_image1;
    image1 = imread("/Users/oyeeun/Desktop/installation/test/Moon.jpeg", 0);
    image2 = imread("/Users/oyeeun/Desktop/installation/test/saltnpepper.png", 0);

    if (image1.empty() || image2.empty()) {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    Mat result_image1 = image1.clone();
    Mat result_image2 = image2.clone();

    Mat laplacian, abs_cian, sharpening;
    Rect rightHalf(image1.cols / 2, 0, image1.cols / 2, image1.rows);
    Mat rightImage = image1(rightHalf);

    Mat blurredImage;
    float k = 2.0;
    GaussianBlur(rightImage, blurredImage, Size(0, 0), 2);
    Mat gmask = rightImage - blurredImage;
    Mat sharpImage = rightImage + k * gmask;
    sharpImage.copyTo(result_image1(rightHalf));

    imshow("moon", image1);
    imshow("moon_filtered", result_image1);

    Mat median;
    Rect leftHalf(0, 0, image2.cols / 2, image2.rows);
    Mat leftImage = image2(leftHalf);
    medianBlur(leftImage, median, 9); // 9x9 mask
    median.copyTo(result_image2(leftHalf));

    imshow("saltnpepper", image2);
    imshow("saltnpepper_filtered", result_image2);
    
    waitKey(0);
    return 0;
}