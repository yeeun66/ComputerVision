#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    Mat image = imread("/Users/oyeeun/Desktop/installation/test/lena.png", 0);
    if (image.empty())
    {
        cout << "Could not open or find the image" << endl;
        return -1;
    }

    Mat result_image = image.clone();

    int width = image.cols;
    int height = image.rows;

    float gamma = 10;
    unsigned char pix[256];

    for(int i = 0; i < 256; i++) {
        pix[i] = saturate_cast<uchar>(pow((float)(i/255.0), gamma)*255.0f); 
    }
    for (int j = 0; j < image.rows; j++){
        for (int i = 0; i < image.cols; i++){
            if (image.at<uchar>(j,i) < 127) result_image.at<uchar>(j, i) = 255 - image.at<uchar>(j,i); // negative
            else result_image.at<uchar>(j, i) = pix[result_image.at<uchar>(j,i)]; // gamma
        }
    }

    // 90 degree (counterclockwise) rotation of the image 
    Mat rotatedImage(width, height, result_image.type());
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            rotatedImage.at<uchar>(i, j) = result_image.at<uchar>(j, height - i - 1);
        }
    }

    imshow("gray image", image);
    imshow("result", rotatedImage);

    moveWindow("gray image", 700, 50);
    moveWindow("result", 50, 50);
    
    waitKey(0);
    return 0;
}