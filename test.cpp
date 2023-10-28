#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {

    Mat query, descriptors1;
    Ptr<ORB> orbF = ORB::create(1000);
    vector<KeyPoint> keypoints1;
    BFMatcher matcher(NORM_HAMMING);

    float nndr = 0.6f;

    // Input query image
    string queryImagePath;
    cout << "Enter query image name: ";
    cin >> queryImagePath;
    queryImagePath = "/Users/ysh/Desktop/oye/assignment/query_image/" + queryImagePath;
    query = imread(queryImagePath);
    resize(query, query, Size(640, 480));

    if (query.empty()) {
        cout << "No file!" << endl;
        return -1;
    }

    // Load the image DBs
    vector<String> dbImages;
    glob("/Users/ysh/Desktop/oye/assignment/DBs/*.jpg", dbImages);

    if (dbImages.empty()) {
        cout << "No DB Images";
        return -1;
    }

    // Process each image in the DB
    int bestMatchIndex = -1;
    double bestMatchScore = DBL_MAX; // big number
    vector<DMatch> bestMatches;

    for (int i = 0; i < dbImages.size(); i++) {
        Mat image = imread(dbImages[i]);
        if (image.empty()) {
            cout << "db Image is empty";
            continue;
        }
        resize(image, image, Size(640, 480));

        // Compute ORB Features for both query and DB images
        orbF->detectAndCompute(query, noArray(), keypoints1, descriptors1);
        vector<KeyPoint> keypoints2;
        Mat descriptors2;
        orbF->detectAndCompute(image, noArray(), keypoints2, descriptors2);

        // KNN Matching (k-nearest neighbor matching)
        vector<vector<DMatch>> matches;
        int k = 2;
        matcher.knnMatch(descriptors1, descriptors2, matches, k);

        // Find good matches
        vector<DMatch> goodMatches;
        for (int j = 0; j < matches.size(); j++) {
            if (matches[j].size() == 2 && matches[j][0].distance <= nndr * matches[j][1].distance) {
                goodMatches.push_back(matches[j][0]);
            }
        }

        if (goodMatches.size() >= 4) {
            double score = static_cast<double>(goodMatches.size());
            if (score < bestMatchScore) {
                bestMatchScore = score;
                bestMatchIndex = i;
                bestMatches = goodMatches;
            }
        }
    }
    cout << "bestMatchIndex: " << bestMatchIndex << endl;

    if (bestMatchIndex != -1) {
        // Display query and best matching image
        Mat image = imread(dbImages[bestMatchIndex]);
        resize(image, image, Size(640, 480));
        Mat imgMatches;
        drawMatches(query, keypoints1, image, keypoints2, bestMatches, imgMatches, Scalar::all(-1), Scalar(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
        imshow("Query", query);
        imshow("Best_matching", imgMatches);
        waitKey(0);
    } else {
        cout << "No good matches found." << endl;
    }
    return 0;
}