#include "filter.h"
#include "process.h"


int main(int argc, char* argv[]) {

    // Load original image
    Mat img = imread(argv[2]);
    if (img.empty()) {
        cout << "Error loading the image...\n";
        return 0;
    }

    cout << "Loaded " << argv[2] << endl; 

    // Place holder for output
    Mat outimg;

    if (strcmp(argv[1], "-rgb2gray") == 0) {
        cout << "Converting to grayscale...\n";
        outimg = rgb2gray(img);
    } 

    else if (strcmp(argv[1], "-brightness") == 0) {
        float factor = stod(argv[4]);
        outimg = adjust_brightness(img, factor);
    } 

    else if (strcmp(argv[1], "-contrast") == 0) {
        float factor = stod(argv[4]);
        outimg = adjust_contrast(img, factor);
    }

    else if (strcmp(argv[1], "-avg") == 0) {
        int ksize = stoi(argv[4]);
        Mat kernel = Mat::ones(ksize, ksize, CV_32F)/(ksize*ksize);

        if (img.channels() >= 3) outimg = convolve_rgb(img, kernel, ksize);
        else outimg = convolve_gray(img, kernel, ksize);
    }

    else if (strcmp(argv[1], "-med") == 0) {
        int ksize = stoi(argv[4]);
        outimg = apply_median(img, ksize);
    }

    else if (strcmp(argv[1], "-gau") == 0) {
        cout << "Applying Gaussian filter...\n";
        float sigma = 20;
        int ksize = stoi(argv[4]);
        outimg = apply_gaussian(img, sigma, ksize);
    }

    else if (strcmp(argv[1], "-sobel") == 0) {
        cout << "Applying Sobel edge detector...\n";
        Mat gray = rgb2gray(img);
        outimg = apply_sobel(gray);
    }

    else if (strcmp(argv[1], "-laplace") == 0) {
        cout << "Applying Laplacian edge detector...\n";
        Mat gray = rgb2gray(img);
        outimg = apply_laplacian(gray);
    }

    else {
        cout << "Operation not found!\n";
        return 0;
    }

    imwrite(argv[3], outimg);
    cout << "Output saved at " << argv[3] << endl;
    
    return 0;
}