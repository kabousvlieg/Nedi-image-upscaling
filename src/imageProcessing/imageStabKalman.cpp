#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <iostream>
#include "imageStabKalman.h"

using namespace cv;
using namespace std;

Tracker::Tracker() {
    rigidTransform = Mat::eye(3,3,CV_32FC1); //affine 2x3 in a 3x3 matrix
}

void Tracker::processThisImage(Mat &img) {
    Mat gray;
    img.copyTo(gray);
    //cvtColor(img,gray,CV_BGR2GRAY);
    vector<Point2f> corners;
    if(trackedFeatures.size() < 200) {
        goodFeaturesToTrack(gray,corners,300,0.01,10);
        cout << "found " << corners.size() << " features\n";
        for (int i = 0; i < corners.size(); ++i) {
            trackedFeatures.push_back(corners[i]);
        }
    }

    if(!trackedImage.empty()) {
        vector<uchar> status; vector<float> errors;
        calcOpticalFlowPyrLK(trackedImage,gray,trackedFeatures,corners,status,errors,Size(10,10));

        if(countNonZero(status) < status.size() * 0.8) {
            cout << "cataclysmic error \n";
            rigidTransform = Mat::eye(3,3,CV_32FC1);
            trackedFeatures.clear();
            trackedImage.release();
            return;
        }

        Mat_<float> newRigidTransform = estimateRigidTransform(trackedFeatures,corners,false);
        Mat_<float> nrt33 = Mat_<float>::eye(3,3);
        newRigidTransform.copyTo(nrt33.rowRange(0,2));
        rigidTransform *= nrt33;

        trackedFeatures.clear();
        for (int i = 0; i < status.size(); ++i) {
            if(status[i]) {
                trackedFeatures.push_back(corners[i]);
            }
        }
    }

//    for (int i = 0; i < trackedFeatures.size(); ++i) {
//        circle(img,trackedFeatures[i],1,Scalar(255),CV_FILLED);
//    }

    gray.copyTo(trackedImage);
}


void Tracker::processImages(Mat img[], int numFrames)
{
    Mat orig,orig_warped,tmp;

    trackedImage.release();
    trackedFeatures.clear();
    for (int i = 0; i < numFrames; i++) {
        img[i].copyTo(orig);
        if(orig.empty()) break;

        processThisImage(orig);

        Mat invTrans = rigidTransform.inv(DECOMP_SVD);
        warpAffine(orig, orig_warped, invTrans.rowRange(0,2), Size());

        orig_warped.copyTo(img[i]);
    }
}

void Tracker::start(Mat img)
{
    trackedImage.release();
    trackedFeatures.clear();
    rigidTransform = Mat::eye(3,3,CV_32FC1);
    processThisImage(img);
}

void Tracker::processImage(Mat img)
{
    Mat orig_warped,tmp;

    if(trackedImage.empty())
        return;

    processThisImage(img);

    Mat invTrans = rigidTransform.inv(DECOMP_SVD);
    warpAffine(img, orig_warped, invTrans.rowRange(0,2), Size());

    orig_warped.copyTo(img);
}