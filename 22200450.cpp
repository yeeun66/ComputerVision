#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main() {

    Mat query, image, descriptors1, descriptors2;
    Ptr<ORB> orbF = ORB::create(1000);
    vector<KeyPoint> keypoints1, keypoints2;
    vector<vector<DMatch>> matches;
    vector<DMatch> goodMatches;
    BFMatcher matcher(NORM_HAMMING);
    Mat imgMatches;

    float nndr = 0.6f;

    // Input query image
    string queryImagePath;
    cout << "Enter query image name: ";
    cin >> queryImagePath;

    query = imread("query_image/" + queryImagePath);
    if (query.empty()) {
        cout << "No file!" << endl;
        return -1;
    }

    // Load the image DBs
    vector<string> dbImages;
    glob("DBs/*.jpg", dbImages);

    if (dbImages.empty()) {
        return -1;
    }

    // Process each image in the DB
    int bestMatchIndex = -1;
    double bestMatchScore = DBL_MAX; // big number

    for (int i = 0; i < dbImages.size(); i++) {
        image = imread(dbImages[i]);
        if (image.empty()) {
            continue;
        }
    

        resize(image, image, Size(640, 480));

        // Compute ORB Features
        orbF->detectAndCompute(query, noArray(), keypoints1, descriptors1);
        orbF->detectAndCompute(image, noArray(), keypoints2, descriptors2);

        // KNN Matching (k-nearest neighbor matching)
        int k = 2;
        matcher.knnMatch(descriptors1, descriptors2, matches, k);

        // Find good matches
        for (int j = 0; j < matches.size(); j++) {
            if (matches.at(j).size() == 2 && matches.at(j).at(0).distance <= nndr * matches.at(j).at(1).distance) {
                goodMatches.push_back(matches[j][0]);
                cout << "image number " + j+1 + "matching: " + goodMatches[j];
            }
        }

        if (goodMatches.size() < 4) {
            cout << "Matching failed for image " << dbImages[i] << endl;
        } 
        else {
            double score = static_cast<double>(goodMatches.size());
            if (score < bestMatchScore) {
                bestMatchScore = score;
                bestMatchIndex = i;
            }
        }
        goodMatches.clear();
    }

    if (bestMatchIndex != -1) {
        // Display query and best matching image
        image = imread(dbImages[bestMatchIndex]);
        resize(image, image, Size(640, 480));
        drawMatches(query, keypoints1, image, keypoints2, goodMatches, imgMatches, Scalar::all(-1), Scalar(-1), vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
        
        imshow("Query", query);
        imshow("Best_matching", image);
        waitKey(0);
    } else {
        cout << "No good matches found." << endl;
    }

    return 0;
}
