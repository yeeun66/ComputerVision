#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    Mat frame;
    VideoCapture cap;
    int fps;
    int delay;
    int current_frames;
    int total_frames;
    int i = 1;

    if(cap.open("/Users/oyeeun/Desktop/installation/test/background.mp4") == 0){
        cout << "no such file!" << endl;
        waitKey(0);
    }

    fps = cap.get(CAP_PROP_FPS); // 24
    delay = 1000 / fps;
    total_frames = cap.get(CAP_PROP_FRAME_COUNT); // Total number of frames included in the video file

    while(1) { 

        current_frames = i; 
        cap >> frame;

        if(cap.get(CAP_PROP_POS_MSEC) >= 3000) { // Check the current time location
            // cout << "end of video" << endl;
            break;
        }
        cout << "frames: " << current_frames << " / " << total_frames << endl;

        imshow("video", frame);
        waitKey(delay);

        i ++;
    }
}