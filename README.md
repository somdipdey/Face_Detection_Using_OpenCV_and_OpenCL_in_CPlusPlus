## Dependencies

    1. OpenCV
    2. OpenCL
    3. Boost Libraries

You have to have Boost libraries installed in the system for C++11 or else you might get issue during build process such as:

    $ fatal error: 'boost/lexical_cast.hpp' file not found

### OpenCV

For MAC OS X

    >> brew install opencv3

    You could follow this link for more info: https://www.pyimagesearch.com/2016/12/19/install-opencv-3-on-macos-with-homebrew-the-easy-way/

For Ubuntu

    Follow this link: https://www.learnopencv.com/install-opencv3-on-ubuntu/
### OpenCL

For OpenCL installation for your specific system, check online (Google, may be).

### Boost

To install Boost libraries use the following options as necessary:

For MAC OS X

    >> brew install boost --c++11

For Ubuntu

    >> sudo apt-get install libboost-all-dev


## Installation

    >> git clone https://github.com/somdipdey/Face_Detection_using_OpenCV_and_OpenCL_in_CPlusPlus.git
    >> mv Face_Detection_using_OpenCV_and_OpenCL_in_CPlusPlus faces
    >> cd faces
    >> cmake .
    >> make

## Utilization

This will automatically detect webcam and start it and additionally write fps to an audit.csv file:

    >> ./faces -a

This will start the face detection on a video instead of camera:

    >> ./faces OUR_VIDEO_PATH

This will open camera with specific id:

    >> ./faces CAMERA_ID
