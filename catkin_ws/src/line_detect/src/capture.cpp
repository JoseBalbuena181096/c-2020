#include<iostream>
#include<math.h>
#include<string>
#include<ros/ros.h>
#include <std_msgs/String.h>
//#include <std_msgs/Int16.h>
#include <std_msgs/UInt8.h>
/*Using image transport for publish and subscribing to image in ros,
allows subscriber to compressed image stream*/
#include <image_transport/image_transport.h>
/*Using the headers for CvBridge, to image encondings*/
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
/*Include the headers for opencv image processing and GUI modules */
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
//Library to use time of computer
#include <chrono>
#include <ctime>
using namespace cv;
using namespace std;

//static const std::string OPENCV_WINDOW = "Original";
static const std::string OPENCV_WINDOW = "CAPTURE IMAGE";
class LineDetection{
    private:
        ros::NodeHandle nh_;
        image_transport::ImageTransport it_;
        image_transport::Subscriber image_sub_;
        std::chrono::time_point<std::chrono::system_clock> start,end;
        std::chrono::duration<double> elapsed_seconds;
        stringstream ss;
        /*variable image*/
        Mat frame;
        int i;
    public:
        //Constructor
        LineDetection():it_(nh_){
        // Subscrive to input video feed and publish output video feed
        image_sub_ = it_.subscribe("/jetson_camera/raw",1,&LineDetection::LineDetectionCb, this);
        cv::namedWindow(OPENCV_WINDOW, WINDOW_KEEPRATIO);
        i=0;
        }
        //Destructor
        ~LineDetection(){
            //cv::destroyWindow(OPENCV_WINDOW);
            cv::destroyWindow(OPENCV_WINDOW);
        }
       void LineDetectionCb(const sensor_msgs::ImageConstPtr& msg){
            set_start_time();
            cv_bridge::CvImagePtr cv_ptr;
            try{
                cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
                copyImageRGB(cv_ptr->image,frame);
            }
            catch (cv_bridge::Exception& e){
                ROS_ERROR("cv_bridge exception: %s", e.what());
                return;
            }
	    	resizeWindow(OPENCV_WINDOW,frame.cols,frame.rows);
            cv::imwrite("sample"+to_string(i++)+".jpg",frame);
            cv::imshow(OPENCV_WINDOW,frame);
            cv::waitKey();
            set_end_time();
            ROS_INFO("[FPS]: %i ",FPS_subscriber());
        }
        void set_start_time(){
            start = std::chrono::system_clock::now();
        }
        void set_end_time(){
            end = std::chrono::system_clock::now();
        }
        int FPS_subscriber(){
            elapsed_seconds = end-start;
            float t = elapsed_seconds.count();
            return 1/t;
        }
       //Method to rotate image 
        Mat rotate(Mat src, double angle=180.0){
            Mat dst;
            Point2f pt(src.cols/2., src.rows/2.);    
            Mat r = getRotationMatrix2D(pt, angle, 1.0);
            warpAffine(src, dst, r, Size(src.cols, src.rows));
	    	return dst;
        }
	    Mat resize_image(Mat input,float alpha=0.15){
		    Mat output;
		    cv::resize(input,output,Size(input.cols*alpha,input.rows*alpha));
		    return output;
	    }
        void copyImageRGB(Mat & image_BGR,Mat &image_RGB){
            cvtColor(image_BGR, image_RGB, COLOR_BGR2RGB);
        }
};
int main(int argc,char **argv){
    ros::init(argc, argv, "line_capture");
    LineDetection *ic= new  LineDetection;
    ros::spin();
    return 0;
}
