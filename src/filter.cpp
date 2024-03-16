#include "filter.h"
#include <vector>
#include <algorithm>


float gaussian2d(int x, int y, float sigma) {
    float coeff = 1.0 / (2.0*M_PI*sigma*sigma);
    float gaussian = pow(M_E, -1.*((x*x + y*y)/(2. * sigma*sigma)));
    return gaussian;
}


Mat gaussian_kernel(float sigma, int dim) {
    int center = dim / 2;

    Mat kernel = Mat::zeros(dim, dim, CV_32F);
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            kernel.at<float>(i, j) = gaussian2d(i - center, j - center, sigma);
        }
    }

    return kernel/sum(kernel);
}


Mat convolve_rgb(Mat img, Mat ker, int ksize) {

    Mat outimg = Mat::zeros(img.size(), img.type());

    // Kernel radius
    int kr = ksize / 2;

    // Loop through each pixels of the image
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {

            Vec3f res = 0;

            // Loop through kernel, let the current pixel be the center
            for (int i_k = -kr; i_k <= kr; i_k++) {
                for (int j_k = -kr; j_k <= kr; j_k++) {

                    bool overflow = i + i_k < 0 
                                    || i + i_k >= img.rows
                                    || j + j_k < 0
                                    || j + j_k >= img.cols;

                    if (overflow) 
                        res += Vec3f(0, 0, 0)*ker.at<float>(i_k + kr, j_k + kr);
                    else {
                        Vec3b pixval = img.at<Vec3b>(i + i_k, j + j_k);
                        res[0] += pixval[0]*ker.at<float>(i_k + kr, j_k + kr);
                        res[1] += pixval[1]*ker.at<float>(i_k + kr, j_k + kr);
                        res[2] += pixval[2]*ker.at<float>(i_k + kr, j_k + kr);
                    }
                }
            }

            outimg.at<Vec3b>(i, j) = res;
        }
    }

    return outimg;
}


Mat convolve_gray(Mat img, Mat ker, int ksize) {

    Mat outimg = Mat::zeros(img.size(), CV_32F);

    // Kernel radius
    int kr = ksize / 2;

    // Loop through each pixels of the image
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {

            float res = 0.0;

            // Loop through kernel, let the current pixel be the center
            for (int i_k = -kr; i_k <= kr; i_k++) {
                for (int j_k = -kr; j_k <= kr; j_k++) {

                    bool overflow = i + i_k < 0 
                                    || i + i_k >= img.rows
                                    || j + j_k < 0
                                    || j + j_k >= img.cols;

                    if (overflow) 
                        res += 0.0*ker.at<float>(i_k + kr, j_k + kr);
                    else {
                        float pixval = img.at<float>(i + i_k, j + j_k);
                        res += pixval*ker.at<float>(i_k + kr, j_k + kr);
                    }
                }
            }

            outimg.at<float>(i, j) = res;
        }
    }

    return outimg;
}


Mat apply_gaussian(Mat img, float sigma, int ksize) {
    Mat outimg;
    
    Mat kernel = gaussian_kernel(sigma, ksize);

    cout << "\nKernel specs:\n";
    cout << ">> ksize: " << ksize << endl;
    cout << ">> sigma: " << sigma << endl << endl;

    if (img.channels() >= 3) outimg = convolve_rgb(img, kernel, ksize);
    else outimg = convolve_gray(img, kernel, ksize);

    return outimg;
}


Mat apply_median(Mat img, int ksize) {

    Mat outimg = Mat::zeros(img.size(), img.type());

    // Kernel radius
    int kr = ksize / 2;

    // Loop through each pixels of the image
    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {

            vector<int> b_chan, g_chan, r_chan;

            // Loop through kernel, let the current pixel be the center
            for (int i_k = -kr; i_k <= kr; i_k++) {
                for (int j_k = -kr; j_k <= kr; j_k++) {

                    bool overflow = i + i_k < 0 
                                    || i + i_k >= img.rows
                                    || j + j_k < 0
                                    || j + j_k >= img.cols;

                    // Simply disregard out of bound values
                    if (!overflow) {
                        cv::Vec3b pixval = img.at<cv::Vec3b>(i + i_k, j + j_k);
                        b_chan.push_back(pixval[0]);
                        g_chan.push_back(pixval[1]);
                        r_chan.push_back(pixval[2]);
                    }
                }
            }

            sort(b_chan.begin(), b_chan.end());
            sort(g_chan.begin(), g_chan.end());
            sort(r_chan.begin(), r_chan.end());

            outimg.at<Vec3b>(i, j) = Vec3b(
                b_chan[b_chan.size()/2],
                g_chan[g_chan.size()/2],
                r_chan[r_chan.size()/2]
            );
        }
    }

    return outimg;
}


Mat apply_sobel(Mat img) {

    Mat outimg;

    Mat sobel_x = (Mat_<float>(3, 3) << 
        -1, 0, 1, 
        -2, 0, 2, 
        -1, 0, 1
    );

    Mat sobel_y = (Mat_<double>(3, 3) << 
        1, 2, 1, 
        0, 0, 0, 
        -1, -2, -1
    );

    cout << "\nKernel specs:\n";
    cout << ">> ksize: " << 3 << endl;
    cout << ">> Sobel x:\n";
    cout << sobel_x << endl << endl;
    cout << ">> Sobel y:\n";
    cout << sobel_y << endl << endl;

    Mat edge_x = convolve_gray(img, sobel_x, 3);
    Mat edge_y = convolve_gray(img, sobel_y, 3);

    edge_x.convertTo(edge_x, CV_32F);
    edge_y.convertTo(edge_y, CV_32F);

    outimg = edge_x.mul(edge_x) + edge_y.mul(edge_y);
    cv::sqrt(outimg, outimg);

    // Min and max response
    double resmin, resmax;
    cv::minMaxLoc(outimg, &resmin, &resmax);

    // Normalize the response to fit between
    // 0 and 255
    resmin = float(resmin);
    resmax = float(resmax);
    outimg = (outimg/resmax)*255.0;

    return outimg;
}


Mat apply_laplacian(Mat img) {

    Mat outimg;

    Mat klaplace = (Mat_<float>(3, 3) << 
        0, -1, 0, 
        -1, 4, -1, 
        0, -1, 0
    );

    cout << "\nKernel specs:\n";
    cout << ">> ksize: " << 3 << endl;
    cout << ">> Laplacian kernel:\n";
    cout << klaplace << endl << endl;

    outimg = convolve_gray(img, klaplace, 3);

    // Min and max response
    double resmin, resmax;
    cv::minMaxLoc(outimg, &resmin, &resmax);

    // Normalize the response to fit between
    // 0 and 255
    resmin = float(resmin);
    resmax = float(resmax);
    outimg = (outimg/resmax)*255.0;

    return outimg;
}


Mat apply_harris(Mat img) {

    // Denoise with Gaussian filter
    img = apply_gaussian(img, 1.0, 3);

    Mat sobel_x = (Mat_<float>(3, 3) << 
        -1, 0, 1, 
        -2, 0, 2, 
        -1, 0, 1
    );
    Mat sobel_y = (Mat_<float>(3, 3) << 
        1, 2, 1, 
        0, 0, 0, 
        -1, -2, -1
    );

    Mat dx = convolve_gray(img, sobel_x, 3);
    Mat dy = convolve_gray(img, sobel_y, 3);

    Mat dxx = dx.mul(dx);
    Mat dyy = dy.mul(dy);
    Mat dxy = dx.mul(dy);

    dxx = apply_gaussian(dxx, 1.0, 3);
    dyy = apply_gaussian(dyy, 1.0, 3);
    dxy = apply_gaussian(dxy, 1.0, 3);

    Mat det = dxx.mul(dyy) - dxy.mul(dxy);
    Mat trace = dxx + dyy;
    Mat response = det - 0.04*(trace.mul(trace));

    normalize(response, response, 0, 255, NORM_MINMAX, CV_32F, Mat());

    return response;
}