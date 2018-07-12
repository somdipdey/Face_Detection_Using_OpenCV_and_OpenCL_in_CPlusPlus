#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "utility.h"

#include <iostream>
#include <boost/lexical_cast.hpp>
//#include <time.h> // Commented out for no-use
#include <ctime>
#include <string>

// basic file operations
#include <fstream>

// for OpenCL
//#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
//#include <OpenCL/cl.hpp>
//#undef CL_VERSION_1_2

using namespace std;
using namespace cv;

/** Function Headers */
void detectAndDisplay( cv::UMat frame ); // modified to UMat from Mat to leverage OpenCL
int executeDetection( const std::string option );
int openCLCheck();

/** Global variables */
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;

/** @function main */
int main( int argc, const char* argv[] )
{
    // fetch OpenCV version
    std::string cvVersion("OpenCV version ==> ");
    cvVersion += boost::lexical_cast<std::string>(CV_VERSION) + "\n";
    cvVersion += "Major version : " + boost::lexical_cast<std::string>(CV_MAJOR_VERSION) + "\n";
    cvVersion += "Minor version : " + boost::lexical_cast<std::string>(CV_MINOR_VERSION) + "\n";
    cvVersion += "Subminor version : " + boost::lexical_cast<std::string>(CV_SUBMINOR_VERSION) + "\n";
    printf("%s\n\n",cvVersion.c_str());
    
    // check OpenCL devices
    //openCLCheck();
    
    if (argc < 2) {
        std::string message("Need another argument along with command, please!\n");
        message += "Current Argument Passed: " + std::to_string(argc-1) + "\n";
        message += "OPTIONS==> \n";
        message += "-a : To write out to audit.csv\n";
        message += "<PATH_TO_VIDEO> : Detects face from video instead of camera\n";
        message += "int <CAMERA_ID> : For a specific camera\n";
        printf("%s\n",message.c_str());
        printf("Choose an option from the above menu: ");
        std::string option;
        cin >> option;
        executeDetection(option);
    } else {
        std::string option = argv[1];
        executeDetection(option);
    }
    

    return 0;
}


int executeDetection(const std::string option){
    int camera_device = 0;
    std::string video_path("");
    ofstream auditfile;
    bool writeToAudit = false;
    
    if(option == "-a"){
        // for audit file
        auditfile.open ("audit.csv", ios::app);
        writeToAudit = true;
    }
    else if(utility::is_number(option)){
        // specify camera
        camera_device = atoi(option.c_str());
    }
    else {
        video_path += option;
    }
    
    //Fetch current path
    const char *currentPath = utility::currentDirectory();
    
    const char *eyeCascadePath = "/haarcascades/haarcascade_eye_tree_eyeglasses.xml";
    std::string fullEyeCascadePath(currentPath);
    utility::replaceStringInPlace(fullEyeCascadePath, "/src", "");
    fullEyeCascadePath += eyeCascadePath;
    
    const char *faceCascadePath = "/haarcascades/haarcascade_frontalface_alt2.xml";
    std::string fullFaceCascadePath(currentPath);
    utility::replaceStringInPlace(fullFaceCascadePath, "/src", "");
    fullFaceCascadePath += faceCascadePath;
    
    
    //-- 1. Load the cascades
    if( !face_cascade.load( fullFaceCascadePath ) )
    {
        cout << "--(!)Error loading face cascade\n";
        return -1;
    };
    if( !eyes_cascade.load( fullEyeCascadePath ) )
    {
        cout << "--(!)Error loading eyes cascade\n";
        return -1;
    };
    
    VideoCapture capture;
    
    //-- 2. Read the video stream
    if(video_path == ""){
        capture.open( camera_device );
    }
    else
    {
        capture.open( video_path );
    }
    if ( ! capture.isOpened() )
    {
        cout << "--(!)Error opening video capture\n";
        return -1;
    }
    
    if(video_path == ""){
        // set the width and height of the video
        capture.set( CV_CAP_PROP_FRAME_WIDTH, 480 );
        capture.set( CV_CAP_PROP_FRAME_HEIGHT, 320 );
    }
    
    // Number of frames to capture
    double num_frames = 1.0;
    
    // Start and end times
    //time_t start, end; // Commented out bcz not used
    
    cv::UMat frame; // modified to UMat from Mat to leverage OpenCL
    while ( capture.read(frame) )
    {
        if( frame.empty() )
        {
            cout << "--(!) No captured frame -- Break!\n";
            break;
        }
        
        // Start time
        //time(&start);
        int start_clock = clock();
        
        // fetch captured fps of the video
        double fps = capture.get(CV_CAP_PROP_FPS);
        //cout << "Frames per second of Streaming Video : " << fps << endl;
        
        //-- 3. Apply the classifier to the frame
        detectAndDisplay( frame );
        
        // End Time
        //time(&end); // Commented out bcz not used
        int stop_clock = clock();
        
        // Time elapsed
        //double seconds = difftime (end, start); // Commented out bcz not used
        double miliseconds = (stop_clock-start_clock)/double(CLOCKS_PER_SEC)*1000;
        //cout << "Time taken : " << seconds << " seconds" << endl;
        
        
        // Calculate frames per second
        double cfps  = num_frames / miliseconds*1000;
        //cout << "Estimated frames per second : " << cfps << endl;
        
        // result/audit output
        std::string auditOut("");
        auditOut += std::to_string(fps) + ",";
        auditOut += std::to_string(cfps);
        
        // write to audit file
        if(writeToAudit == true){
            auditfile.open ("audit.csv", ios::app);
            if (auditfile.is_open()) {
                /* ok, proceed with output */
                auditOut += "\n";
                auditfile << auditOut;
                auditfile.close();
            }
        }
        
        // printout the result on the frame
        int rows = frame.rows;
        int cols = frame.cols;
        cv::Size s = frame.size();
        rows = s.height;
        cols = s.width;
        
        putText( frame, auditOut, Point2f(rows/15,cols/15), FONT_HERSHEY_PLAIN, 2,  Scalar(255,255,255), 2, 7 );
        //-- Show what you got
        imshow( "Project - Faces => Capture - Face detection on CPU/GPU Using OpenCL/OpenCV", frame );
        
        int c = cvWaitKey(1);
        if (c == 27 || c == 13 || c == 10) {
            break;
        }
    }
    
    return 0;
}

/** @function detectAndDisplay */
void detectAndDisplay( cv::UMat frame ) // modified to UMat from Mat to leverage OpenCL
{
    cv::UMat frame_gray; // modified to UMat from Mat to leverage OpenCL
    cvtColor( frame, frame_gray, COLOR_BGR2GRAY );
    equalizeHist( frame_gray, frame_gray );
    
    //-- Detect faces
    std::vector<Rect> faces;
    face_cascade.detectMultiScale( frame_gray, faces );
    
    for ( size_t i = 0; i < faces.size(); i++ )
    {
        //cout << "Number of faces: " << faces.size() << endl;
        
        Point center( faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2 );
        ellipse( frame, center, Size( faces[i].width/2 + 10, faces[i].height/2 + 10 ), 0, 0, 360, Scalar( 0, 0, 255 ), 3 );
        
        cv::UMat faceROI = frame_gray( faces[i] ); // modified to UMat from Mat to leverage OpenCL
        
        //-- In each face, detect eyes
        std::vector<Rect> eyes;
        eyes_cascade.detectMultiScale( faceROI, eyes );
        
        for ( size_t j = 0; j < eyes.size(); j++ )
        {
            Point eye_center( faces[i].x + eyes[j].x + eyes[j].width/2, faces[i].y + eyes[j].y + eyes[j].height/2 );
            int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
            circle( frame, eye_center, radius, Scalar( 0, 255, 0 ), 2 );
        }
    }
    
}

/* // Commented this section because MAC OS does not support OpenCL for some reasons
int openCLCheck(){
    //get all platforms (drivers)
    std::vector<cl::Platform> all_platforms;
    cl::Platform::get(&all_platforms);
    if(all_platforms.size()==0){
        std::cout<<" No platforms found. Check OpenCL installation!\n";
        return -1;
    }
    cl::Platform default_platform=all_platforms[0];
    std::cout << "Using platform: "<< default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";
    
    //get default device of the default platform
    std::vector<cl::Device> all_devices;
    default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
    if(all_devices.size()==0){
        std::cout<<" No devices found. Check OpenCL installation!\n";
        exit(1);
    }
    cl::Device default_device=all_devices[0];
    std::cout<< "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << "\n";
}
 */
