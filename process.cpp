#include "process.h"


Mat rgb2gray(Mat rgb) {
    Mat gray = Mat::zeros(rgb.size(), CV_8UC1);
    
    // Loop through each pixels of the image
    for (int i = 0; i < rgb.rows; i++) {
        for (int j = 0; j < rgb.cols; j++) {

            Vec3b pixval = rgb.at<Vec3b>(i, j);
            int blue = pixval[0] * 0.114;
            int green = pixval[1] * 0.587;
            int red = pixval[2] * 0.299;

            gray.at<uchar>(i, j) = blue + green + red;
        }
    }

    return gray;
}


Mat adjust_brightness(Mat img, float factor) {
    return factor*img;
}


Mat adjust_contrast(Mat img, float factor) {
    return factor*(img - 128) + 128;
}