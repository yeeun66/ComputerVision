// Project1
#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

void white_balancing(Mat img);
void determine_mode(int index);
bool mode[9] = {false};

int main() {
    Mat frame;
    VideoCapture cap;
    int fps, delay, key;

    if(cap.open("/Users/oyeeun/Desktop/installation/test/video.mp4") == 0){
        cout << "no such file!" << endl;
        waitKey(0);
    }
    
    fps = cap.get(CAP_PROP_FPS);
    delay = 1000 / fps;

    while (1) {
        cap >> frame;

        if (frame.empty()) {
            break;
        }

        if (mode[0]) { // negativeMode
            Mat hsvFrame;
            cvtColor(frame, hsvFrame, COLOR_BGR2HSV);

            for (int j = 0; j < hsvFrame.rows; j++) {
                Vec3b* hsvPixel = hsvFrame.ptr<Vec3b>(j);
                for (int i = 0; i < hsvFrame.cols; i++) {
                    hsvPixel[i][2] = 255 - hsvPixel[i][2];
                }
            }
            cvtColor(hsvFrame, frame, COLOR_HSV2BGR);
        }
        else if (mode[1]) { // gammaMode
            float gamma = 2.5;
            Mat hsvFrame;
            cvtColor(frame, hsvFrame, COLOR_BGR2HSV);

            for (int j = 0; j < hsvFrame.rows; j++) {
                Vec3b* hsvPixel = hsvFrame.ptr<Vec3b>(j);
                for (int i = 0; i < hsvFrame.cols; i++) {
                    hsvPixel[i][2] = pow(hsvPixel[i][2]/255.0, gamma) *255.0;
                }
            }
            cvtColor(hsvFrame, frame, COLOR_HSV2BGR);
        }
        else if (mode[2]) { // histoMode
            Mat HSV;
            vector<Mat> ic(3);
            cvtColor(frame, HSV, COLOR_BGR2HSV); 
            split(HSV, ic);
            equalizeHist(ic[2], ic[2]);
            merge(ic, frame);
            cvtColor(frame, frame, COLOR_HSV2BGR); 
        }
        else if (mode[3]) { // colorSlicing
            Mat hsvFrame;
            cvtColor(frame, hsvFrame, COLOR_BGR2HSV);

            for (int j = 0; j < hsvFrame.rows; j++) {
                Vec3b* hsvPixel = hsvFrame.ptr<Vec3b>(j);
                for (int i = 0; i < hsvFrame.cols; i++) {
                    int h = hsvPixel[i][0]; // Hue value
                    if (h > 9 && h < 23) {
                        // Keep the original saturation value
                    } else {
                        hsvPixel[i][1] = 0; // Set saturation to 0 for non-orange pixels
                    }   
                }
            }
            cvtColor(hsvFrame, frame, COLOR_HSV2BGR);
        }
        else if (mode[4]) { // colorConversion
            Mat hsvFrame;
            cvtColor(frame, hsvFrame, COLOR_BGR2HSV);

            for (int j = 0; j < hsvFrame.rows; j++) {
                Vec3b* hsvPixel = hsvFrame.ptr<Vec3b>(j);
                for (int i = 0; i < hsvFrame.cols; i++) {
                    int h = hsvPixel[i][0]; // Hue value
                    if(h + 50 > 179) hsvPixel[i][0] += 50 - 179;
                    else hsvPixel[i][0] += 50;
                }
            }
            cvtColor(hsvFrame, frame, COLOR_HSV2BGR);  
        }
        else if(mode[5]){ // avgFiltering
            blur(frame, frame, Size(9, 9));
        }
        else if(mode[6]){ // sharpening
            Mat blurredFrame;
            float k = 2.0;
            blur(frame, blurredFrame, Size(9, 9));
            Mat gmask = frame - blurredFrame;
            Mat sharpFrame = frame + k * gmask;
            sharpFrame.copyTo(frame);
        }
        else if(mode[7]){ // whiteBalancing
            white_balancing(frame);
        }

        imshow("video", frame);

        key = waitKey(delay);

        if(key == 110) { // n
            determine_mode(0);
        }
        else if (key == 103) { // g
            determine_mode(1);
        }
        else if (key == 104) { // h
            determine_mode(2);
        }
        else if (key == 115) { // s
            determine_mode(3);
        }
        else if (key == 99) { // c
            determine_mode(4);
        }
        else if (key == 97) { // a
            determine_mode(5);
        }
        else if (key == 117) { // u
            determine_mode(6);
        }
        else if (key == 119) { // w
            determine_mode(7);
        }
        else if (key == 114) { // r
            determine_mode(8);
        }
        else if (key == 27) break; // esc
       
    }
    cap.release();
    return 0;
}

void white_balancing(Mat img) {
    Mat bgr_channels[3];
    split(img, bgr_channels);

    for (int c = 0; c < img.channels(); c++) {
        int sum = 0;
        double avg = 0.0;

        // Calculate sum of pixel values
        for (int i = 0; i < img.rows; i++) {
            for (int j = 0; j < img.cols; j++) {
                sum += bgr_channels[c].at<uchar>(i, j);
            }
        }
        // Calculate average
        avg = static_cast<double>(sum) / (img.rows * img.cols);

        // Apply white balancing
        for (int i = 0; i < img.rows; i++) {
            for (int j = 0; j < img.cols; j++) {
                int temp = static_cast<int>((128.0 / avg) * bgr_channels[c].at<uchar>(i, j));
                if (temp > 255) {
                    bgr_channels[c].at<uchar>(i, j) = 255;
                } else {
                    bgr_channels[c].at<uchar>(i, j) = static_cast<uchar>(temp);
                }
            }
        }
    }
    merge(bgr_channels, 3, img);
}

void determine_mode(int index) {
    mode[index] = true;
    for(int i = 0; i<9; i++) {
        if(i == index) continue;
        mode[i] = false;
    }
}