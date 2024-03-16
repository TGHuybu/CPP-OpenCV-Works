#include "libs.h"


Mat convolve_rgb(Mat img, Mat ker, int ksize);
Mat convolve_gray(Mat img, Mat ker, int ksize);

float gaussian2d(int x, int y, float sigma);
Mat gaussian_kernel(float sigma, int dim);

Mat apply_gaussian(Mat img, float sigma, int ksize);
Mat apply_median(Mat img, int ksize);
Mat apply_sobel(Mat img);
Mat apply_laplacian(Mat img);
Mat apply_harris(Mat img);