## Dependencies

    1. OpenCV
    2. OpenCL
    3. Boost Libraries

You have to have Boost libraries installed in the system for C++11 or else you might get issue during build process such as:

    $ fatal error: 'boost/lexical_cast.hpp' file not found

### OpenCV
------

For MAC OS X

    >> brew install opencv3

  You could follow this link for more info: [OpenCV3 for Mac OS](https://www.pyimagesearch.com/2016/12/19/install-opencv-3-on-macos-with-homebrew-the-easy-way/)

For Ubuntu

  Follow this link: [OpenCV3 for Ubuntu](https://docs.opencv.org/3.4.0/d7/d9f/tutorial_linux_install.html)
    
#### Alternatively try the following to install OpenCV3 on Ubuntu:

    >> [sudo] apt-get install opencv-data
    >> [sudo] apt-get install libopencv-dev
    
#### Check if OpenCV3 is installed properly:

    >> pkg-config --modversion opencv
    3.2.x
    
### OpenCL
------

For OpenCL installation for your specific system, check online (Google, may be).

If you are trying to install OpenCL on Ubuntu ten follow this:

    >> [sudo] apt install ocl-icd-libopencl1
    >> [sudo] apt install opencl-headers
    >> [sudo] apt install clinfo
    >> [sudo] apt install ocl-icd-opencl-dev
    
To test if OpenCL is installed on Ubuntu try the following command:
    
    >> clinfo
    
For more info on OpenCL installation, you could also take a look at this [link](https://askubuntu.com/a/850594).

### Boost
------

To install Boost libraries use the following options as necessary:

For MAC OS X

    >> brew install boost --c++11

For Ubuntu

    >> sudo apt-get install libboost-all-dev


# Installation

    >> git clone https://github.com/somdipdey/Face_Detection_using_OpenCV_and_OpenCL_in_CPlusPlus.git
    >> mv Face_Detection_using_OpenCV_and_OpenCL_in_CPlusPlus faces
    >> cd faces
    >> cmake .
    >> make
    
#### Comments:

1) cmake . (cmake dot) makes the build files in te same directory.

2) Compile the project with the generated makefile: make -jN.

Set N to the number of simultaneous threads supported on your compilation platform, e.g. make -j8.


# Utilization

This will automatically detect webcam and start it and additionally write fps to an audit.csv file:

    >> ./faces -a

This will start the face detection on a video instead of camera:

    >> ./faces "OUR_VIDEO_PATH"

"OUR_VIDEO_PATH" is a string value, mentioning the path to the video. Example: "/usr/somdipdey/videos/myVideo.avi"


This will open camera with specific id:

    >> ./faces CAMERA_ID
    
CAMERA_ID is an integer value, mentioning the ID of the camera. Example: 0 for attached default webcam.
