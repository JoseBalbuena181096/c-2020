
#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>
std::chrono::time_point<std::chrono::system_clock> start,end;
std::chrono::duration<double> elapsed_seconds;

std::string gstreamer_pipeline (int capture_width, int capture_height, int display_width, int display_height, int framerate, int flip_method) {
    return "nvarguscamerasrc ! video/x-raw(memory:NVMM), width=(int)" + std::to_string(capture_width) + ", height=(int)" +
           std::to_string(capture_height) + ", format=(string)NV12, framerate=(fraction)" + std::to_string(framerate) +
           "/1 ! nvvidconv flip-method=" + std::to_string(flip_method) + " ! video/x-raw, width=(int)" + std::to_string(display_width) + ", height=(int)" +
           std::to_string(display_height) + ", format=(string)BGRx ! videoconvert ! video/x-raw, format=(string)BGR ! appsink";
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

 int main(int argc, char** argv)
{
    ros::init(argc, argv, "jetson_camera");
    ros::NodeHandle nh;
    image_transport::ImageTransport it(nh);
    image_transport::Publisher pub = it.advertise("/jetson_camera/raw", 1);
    sensor_msgs::ImagePtr msg; //
    
     int capture_width = 1280 ;
    int capture_height = 720 ;
    int display_width = 448;
    int display_height = 252;
    int framerate = 60 ;
    int flip_method = 0 ;

    std::string pipeline = gstreamer_pipeline(capture_width,
	capture_height,
	display_width,
	display_height,
	framerate,
	flip_method);
    std::cout << "Using pipeline: \n\t" << pipeline << "\n";
    cv::VideoCapture cap(pipeline, cv::CAP_GSTREAMER);
    if(!cap.isOpened()) {
        ROS_ERROR("Failed to open camera\n");
	    return (-1);
    }

    //cv::namedWindow("CSI Camera", cv::WINDOW_AUTOSIZE);
    cv::Mat img;

    //std::cout << "Hit ESC to exit" << "\n" ;
    while(true)
    {
        set_start_time();
    	if (!cap.read(img)) {
            ROS_ERROR("Capture read error \n");
		    break;
	    }
        /*
	    std::cout<<"Capture good1"<<std::endl;
	    cv::imshow("CSI Camera",img);

	    int keycode = cv::waitKey(30) & 0xff ; 
            if (keycode == 27) break ;
        */
        msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", img).toImageMsg();
        pub.publish(msg);
        set_end_time();
        ROS_INFO("[FPS]: %i ",FPS_subscriber());
    }
    cap.release();
    //cv::destroyAllWindows() ;
    return 0;
}



