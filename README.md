# **C++ OpenCV Algorithm Implementations**
A number of image processing and computer vision algorithms implemented with the help of [OpenCV](https://docs.opencv.org/4.x/index.html).
The project is part of a series of assignemts in [HCMUS](https://en.wikipedia.org/wiki/Ho_Chi_Minh_City_University_of_Science)' *Computer Vision* course.

## **Contents**
- `data/` folder contains some sample input and output images
- `include/` folder contains C++ header files (`*.h` extension)
- `src/` folder contains C++ source files (`*.cpp` extension)

## **Algorithms**
At the moment, the following algorithms and processes are implemented:
- `-rgb2grayscale`$^1$: convert RGB to grayscale image
- `-brightness`$^1$: adjust brightness of the image by a given factor
- `-contrast`$^1$: adjust brightness of the image by a given factor
- `-avg`, `-med`, and `-gau`$^2$: apply image filtering with average, median, and Gaussian kernel respectively
- `-sobel`, `-laplace`$^2$: edge detection using $3 \times 3$ Sobel and Laplacian kernel respectively
- `-harris`$^2$: corner detection with Harris detector

*Notes* \
$^1$ see file `src/process.cpp` for detailed implementation \
$^2$ see file `src/filter.cpp` for detailed implementation

## **Usage**
Compile the program using your choice of environment and invoke the executable. 

Here I present the Bash syntax to run the program in Ubuntu terminal:
```bash
input="path/to/input/image"
output="path/to/input/image"
factor=1.5  # a floating point number
ksize=5     # integer kernel size (must be an odd number)

# Examples of how to run the program
./main -rgb2gray $input $output
./main -brightness $input $output $factor
./main -contrast $input $output $factor
./main -avg $input $output $ksize
./main -med $input $output $ksize
./main -gau $input $output $ksize
./main -sobel $input $output
./main -laplace $input $output
```

## **Acknowledgements**
The testing samples used in this project are products of NASA/ESA [Hubble Space Telescope](https://hubblesite.org/home), Twitter
user [hitsukuya](https://twitter.com/hitsukuya), [miHoYo Co., Ltd.](https://hsr.hoyoverse.com/en-us/home), and unknown creator of an Internet [meme](https://www.youtube.com/watch?v=B-O6pWeZ7W0&t=0s). All data were used for
educational and research purposes.
