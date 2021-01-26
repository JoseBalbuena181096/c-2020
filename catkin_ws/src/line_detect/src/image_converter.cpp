#include<iostream>
#include<ros/ros.h>
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
static const std::string OPENCV_WINDOW2 = "Region Interest";
static const std::string OPENCV_WINDOW3 = "Transformation Perspective";
static const std::string OPENCV_WINDOW4 = "Threshold";
class ImageConverter{
    private:
        ros::NodeHandle nh_;
        image_transport::ImageTransport it_;
        image_transport::Subscriber image_sub_;
        //image_transport::Publisher image_pub_;
        std::chrono::time_point<std::chrono::system_clock> start, end;
        std::chrono::duration<double> elapsed_seconds;
        Mat frame,matrixPerspective,framePerspective,frameGray,frameThreshold,frameEdge,frameFinal;   
        Mat ROILane,frameFinalBGR;
        Point2f Source[4];
        Point2f Destination[4];
        vector<int> histogramLane;
        int LeftLanePosition, RightLanePosition,laneCenter,centerFrame,error;
        stringstream ss;
    public:
        //Constructor
        ImageConverter():it_(nh_){
            // Subscrive to input video feed and publish output video feed
            image_sub_ = it_.subscribe("/jetbot_camera/raw",1,&ImageConverter::imageCb, this);
            //image_pub_ = it_.advertise("/image_converter/output_video", 1);
            //init points to source 
            Source[0]=Point2f(52,160);
            Source[1]=Point2f(308,160);
            Source[2]=Point2f(22,210);
            Source[3]=Point2f(338,210);
            //init points to desination
            Destination[0]=Point2f(55,0);
            Destination[1]=Point2f(305,0);
            Destination[2]=Point2f(55,240);
            Destination[3]=Point2f(305,240);            
            //cv::namedWindow(OPENCV_WINDOW);
            cv::namedWindow(OPENCV_WINDOW2, WINDOW_KEEPRATIO);
            cv::namedWindow(OPENCV_WINDOW3, WINDOW_KEEPRATIO);
            cv::namedWindow(OPENCV_WINDOW4, WINDOW_KEEPRATIO);
        }
        //Destructor
        ~ImageConverter(){
            //cv::destroyWindow(OPENCV_WINDOW);
            cv::destroyWindow(OPENCV_WINDOW2);
            cv::destroyWindow(OPENCV_WINDOW3);
        }
        //Callback funtion
        void imageCb(const sensor_msgs::ImageConstPtr& msg){
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
            // Draw an example circle on the video stream
            /*
            if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
                cv::circle(cv_ptr->image, cv::Point(60, 60), 10, CV_RGB(0,0,255));
            // Update GUI Window
            cv::imshow(OPENCV_WINDOW, cv_ptr->image);
            */
            frame=rotate(frame);
            Perspective();
            Threshold();
            Histogram();
            LaneFinder();
            LaneCenter();
            showError();
            //show image RGB and region interest
            moveWindow(OPENCV_WINDOW2,50,100);
            resizeWindow(OPENCV_WINDOW2,640,480);
            cv::imshow(OPENCV_WINDOW2, frame);
            //show image perspective transformation 
            moveWindow(OPENCV_WINDOW3,700,100);
            resizeWindow(OPENCV_WINDOW3,640,480);
            cv::imshow(OPENCV_WINDOW3,framePerspective);
            //gray image threshold
            moveWindow(OPENCV_WINDOW4,1340,100);
            resizeWindow(OPENCV_WINDOW4,640,480);
            cv::imshow(OPENCV_WINDOW4,frameFinal);
            cv::waitKey(3);
            // Output modified video stream
            //image_pub_.publish(cv_ptr->toImageMsg());
            set_end_time();
            std::cout<<"FPS "<<FPS_subscriber()<<std::endl;
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
        //Method to change image from BGR to RGB
        void copyImageRGB(Mat & image_BGR,Mat &image_RGB){
            cvtColor(image_BGR, image_RGB, COLOR_BGR2RGB);
        }
        //Method to rotate image 
        Mat rotate(Mat src, double angle=180.0){
            Mat dst;
            Point2f pt(src.cols/2., src.rows/2.);    
            Mat r = getRotationMatrix2D(pt, angle, 1.0);
            warpAffine(src, dst, r, Size(src.cols, src.rows));
            return dst;
        }
        void Perspective(){
            //Draw lines to source
            line(frame,Source[0],Source[1],Scalar(0,0,255),2);
            line(frame,Source[1],Source[3],Scalar(0,0,255),2);
            line(frame,Source[2],Source[3],Scalar(0,0,255),2);
            line(frame,Source[2],Source[0],Scalar(0,0,255),2);
            /*
            //Draw lines to destination
            line(frame,Destination[0],Destination[1],Scalar(255,0,0),2);
            line(frame,Destination[1],Destination[3],Scalar(255,0,0),2);
            line(frame,Destination[2],Destination[3],Scalar(255,0,0),2);
            line(frame,Destination[2],Destination[0],Scalar(255,0,0),2);
            */
            //Apply transformation perspective 
            matrixPerspective = getPerspectiveTransform(Source,Destination);
            warpPerspective(frame,framePerspective,matrixPerspective,Size(360,240));
        }
        void Threshold(){
            cvtColor(framePerspective,frameGray,COLOR_RGB2GRAY);
            //This parameter depent od light
            //this is to font white and line black
            GaussianBlur(frameGray, frameGray, Size(5, 5), 0);
            inRange(frameGray,150,255,frameThreshold);
            ////this is to font black and line whine
            //inRange(frameGray,240,255,frameGray);
            Canny(frameGray,frameEdge,200,400,3,false);
            add(frameThreshold,frameEdge,frameFinal);
            cvtColor(frameFinal, frameFinal, COLOR_GRAY2RGB);
            cvtColor(frameFinal, frameFinalBGR, COLOR_RGB2BGR);
        }
        void Histogram(){
            //Create histogram with the length of the width of the frame 
            histogramLane.resize(frame.size().width);       
            histogramLane.clear();
            for(int i=0;i<frame.size().width;i++){
                //Region interest
                ROILane=frameFinalBGR(Rect(i,140,1,100));
                //Normal values 
                divide(255,ROILane,ROILane);
                //add the value 
                histogramLane.push_back((int)(sum(ROILane)[0]));
            } 
        }
        void LaneFinder(){
            //Find line left
            vector<int>:: iterator LeftPtr;
            LeftPtr = max_element(histogramLane.begin(),histogramLane.begin()+135);
            LeftLanePosition=distance(histogramLane.begin(),LeftPtr);
            //find line right
            vector<int>:: iterator RightPtr;
            RightPtr = max_element(histogramLane.begin()+225,histogramLane.end());
            RightLanePosition=distance(histogramLane.begin(),RightPtr);
            //Draw line left
            line(frameFinal,Point2f(LeftLanePosition, 0), Point2f(LeftLanePosition, 240),Scalar(0,255,0),2);
            //Draw line Right
            line(frameFinal,Point2f(RightLanePosition, 0), Point2f(RightLanePosition, 240),Scalar(0,255,0),2);
        }
        void LaneCenter(){
            laneCenter=(RightLanePosition-LeftLanePosition)/2+LeftLanePosition;
            centerFrame=183;
            line(frameFinal,Point2f(laneCenter,0),Point2f(laneCenter,240),Scalar(0,255,0),2);
            line(frameFinal,Point2f(centerFrame,0),Point2f(centerFrame,240),Scalar(0,0,255),2);
            error=laneCenter-centerFrame;
        }
        void showError(){
            ss.str(" ");
            ss.clear();
            ss<<"Error = "<<error;
            putText(frame, ss.str(), Point2f(1,50), 0,1, Scalar(255,0,0), 1);
        }
};

int main(int argc,char **argv){
    ros::init(argc, argv, "image_converter");
    ImageConverter *ic= new ImageConverter;
    ros::spin();
    return 0;
}