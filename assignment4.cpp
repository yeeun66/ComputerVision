#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    Mat frame;
    VideoCapture cap;
    int fps;
    int delay;
    float a, b, x0, y0;
    Point p1, p2;
    vector<Vec2f> left_lines; // Stores left lines
    vector<Vec2f> right_lines; // Stores right lines
  
    if(cap.open("/Users/oyeeun/Desktop/installation/test/Road.mp4") == 0){
        cout << "no such file!" << endl;
        waitKey(0);
    }

    Rect left_roi(200, 400, 400, 200);
    Rect right_roi(600, 400, 400, 200);

    fps = cap.get(CAP_PROP_FPS);
    delay = 1000 / fps;

    while (1) {
        Mat frame;
        cap >> frame;

        if(cap.get(CAP_PROP_POS_MSEC) >= 20000) break;

        // 영상에서 두 개의 영역 자르기
        Mat roi1 = frame(left_roi);
        Mat roi2 = frame(right_roi);

        // Grayscale로 변환
        cvtColor(roi1, roi1, COLOR_BGR2GRAY);
        cvtColor(roi2, roi2, COLOR_BGR2GRAY);

        // Blur
        blur(roi1, roi1, Size(5, 5));
        blur(roi2, roi2, Size(5, 5));

        // GaussianBlur
        GaussianBlur(roi1, roi1, Size(5, 5), 2);
        GaussianBlur(roi2, roi2, Size(5, 5), 2);

        //Canny edge operator
        Canny(roi1, roi1, 10, 60, 3);
        Canny(roi2, roi2, 10, 60, 3);

        // Apply Hough Line Transform
        HoughLines(roi1, left_lines, 1, CV_PI / 180, 100,0,0, CV_PI / 6, CV_PI / 3); 
        HoughLines(roi2, right_lines, 1, CV_PI / 180, 100,0,0, 2* CV_PI / 3, 5* CV_PI / 6);

        if (!left_lines.empty()) {
            float avg_rho = 0.0f;
            float avg_theta = 0.0f;
            for (int i = 0; i < left_lines.size(); i++) {
                avg_rho += left_lines[i][0];
                avg_theta += left_lines[i][1];
            }
            avg_rho /= left_lines.size();
            avg_theta /= left_lines.size();
            a = cos(avg_theta);
            b = sin(avg_theta);
            x0 = a * avg_rho; 
            y0 = b * avg_rho;
            Point pt1(cvRound(x0 + 2000 * (-b)) + 200, cvRound(y0 + 2000 * a)+400);
            Point pt2(cvRound(x0 - 2000 * (-b)) + 200, cvRound(y0 - 2000 * a)+400);
            
            line(frame, pt1, pt2, Scalar(0, 0, 255), 3,8); // Red color
        }

        if (!right_lines.empty()) {
            float avg_rho = 0.0f;
            float avg_theta = 0.0f;
            for (int i = 0; i < right_lines.size(); i++) {
                avg_rho += right_lines[i][0];
                avg_theta += right_lines[i][1];
            }
            avg_rho /= right_lines.size();
            avg_theta /= right_lines.size();
            a = cos(avg_theta);
            b = sin(avg_theta);
            x0 = a * avg_rho; 
            y0 = b * avg_rho;
            Point pt1(cvRound(x0 + 2000 * (-b)) + 600, cvRound(y0 + 2000 * a)+400);
            Point pt2(cvRound(x0 - 2000 * (-b)) + 600, cvRound(y0 - 2000 * a)+400);

            line(frame, pt1, pt2, Scalar(0, 0, 255), 3, 8); // Red color
        }

        // 두 개의 영역을 각각 다른 윈도우에 출력
        namedWindow("Left canny");
        moveWindow("Left canny", 200, 0);
        imshow("Left canny", roi1);

        namedWindow("Right canny");
        moveWindow("Right canny", 600, 0);
        imshow("Right canny", roi2);

        // 동영상 윈도우에 출력
        imshow("Frame", frame);
        waitKey(delay);
    }
    cap.release();
    return 0;
}
