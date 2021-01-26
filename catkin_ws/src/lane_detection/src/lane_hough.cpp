#include<iostream>
#include<math.h>
#include<string>
#include<ros/ros.h>
#include <std_msgs/String.h>
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
static const std::string OPENCV_WINDOW = "ROAD DETECTION";
static const std::string OPENCV_WINDOW_1 = "EDGE";
class LineDetection{
    private:
        ros::NodeHandle nh_;
        image_transport::ImageTransport it_;
        image_transport::Subscriber image_sub_;
        ros::Publisher angle_pub;
        std_msgs::String angle_message;
        std::chrono::time_point<std::chrono::system_clock> start,end;
        std::chrono::duration<double> elapsed_seconds;
        stringstream ss,ss_angle;
        /*variable image*/
        Mat frame;
        Mat img_edges;
        Mat img_lines;
        Mat img_perspective;
        vector<Vec4i> lines;
        vector<vector<Vec4i>> left_right_lines;
        vector<Point> lane;
        double img_size;
        double img_center;
        bool left_flag = false;  // Tells us if there's left boundary of lane detected
        bool right_flag = false;  // Tells us if there's right boundary of lane detected
        Point right_b;  // Members of both line equations of the lane boundaries:
        double right_m;  // y = m*x + b
        Point left_b;  //
        double left_m;  //
        Point2f Source[4];
        Point2f Destination[4];
        double angle_to_mid_radian,x_offset,y_offset,steering_angle_radian;
        int angle_to_mid_deg,steering_angle; 
    public:
        //Constructor
         LineDetection():it_(nh_){
            // Subscrive to input video feed and publish output video feed
            image_sub_ = it_.subscribe("/jetson_camera/raw",1,&LineDetection::LineDetectionCb, this);
            //image_pub_ = it_.advertise("/image_converter/output_video", 1);
            angle_pub = nh_.advertise<std_msgs::String>("/LanesDetection/steering_angle",1500);
            cv::namedWindow(OPENCV_WINDOW, WINDOW_KEEPRATIO);
	        cv::namedWindow(OPENCV_WINDOW_1, WINDOW_KEEPRATIO);
            Source[0]=Point2f(210,130);
            Source[1]=Point2f(430,130);
            Source[2]=Point2f(0,320);
            Source[3]=Point2f(640,320);
            //init points to desination
            Destination[0]=Point2f(200,0);
            Destination[1]=Point2f(440,0);
            Destination[2]=Point2f(200,360);
            Destination[3]=Point2f(440,360);  
        }
        //Destructor
        ~LineDetection(){
            //cv::destroyWindow(OPENCV_WINDOW);
            cv::destroyWindow(OPENCV_WINDOW);
        }
        //Callback funtion
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
            //frame = resize_image(frame,0.25);
            //frame = rotate(frame);
            img_perspective = Perspective(frame);
	        img_edges = Threshold(img_perspective);
            lines = houghLines(img_edges);
            if(!lines.empty()){
                left_right_lines = lineSeparation(lines,img_edges);
                lane = regression(left_right_lines,img_edges); 
                img_lines = plotLanes(img_perspective,lane);
            }
            resizeWindow(OPENCV_WINDOW_1,img_perspective.cols,img_perspective.rows);
            cv::imshow(OPENCV_WINDOW_1,img_edges);
            moveWindow(OPENCV_WINDOW,100,50);
	        resizeWindow(OPENCV_WINDOW,img_perspective.cols,img_perspective.rows);
            cv::imshow(OPENCV_WINDOW,img_lines);
            cv::waitKey(3);
            // Output modified video stream
            //image_pub_.publish(cv_ptr->toImageMsg());
            ss_angle<<steering_angle<<",";
            angle_message.data = ss_angle.str();
            angle_pub.publish(angle_message);
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
	    /*
	    line(dst,Source[0],Source[1],Scalar(0,0,255),2);
            line(dst,Source[1],Source[3],Scalar(0,0,255),2);
            line(dst,Source[2],Source[3],Scalar(0,0,255),2);
            line(dst,Source[2],Source[0],Scalar(0,0,255),2);
	    */
	    return dst;
        }
        Mat Perspective(Mat frame){
            Mat output;
 	    Mat matrixPerspective( 2, 4, CV_32FC1 );
	    matrixPerspective = Mat::zeros( frame.rows, frame.cols, frame.type() );
            matrixPerspective = getPerspectiveTransform(Source,Destination); warpPerspective(frame,output,matrixPerspective,Size(frame.cols,frame.rows));
            return output;
        }
              Mat Threshold(Mat frame)
        {
            Mat frameThreshold;
            Mat sobelx;
            Mat draw;
            double minVal, maxVal;
	    	frame.convertTo(frame, -1, 1, -15);
            cvtColor(frame,frameThreshold,COLOR_RGB2GRAY);
            inRange(frameThreshold,150,255,frameThreshold);
	        medianBlur(frameThreshold,frameThreshold,5);
	        width_filter(frameThreshold);
            Sobel(frameThreshold, sobelx, CV_32F, 1, 0);
            minMaxLoc(sobelx, &minVal, &maxVal);
            sobelx.convertTo(frameThreshold, CV_8U, 255.0/(maxVal), - 255.0/(maxVal));
            dilate(frameThreshold,frameThreshold, getStructuringElement(MORPH_RECT, Size(7,7)));
	      //  medianBlur(frameThreshold,frameThreshold,7);
            return frameThreshold;
        }

		void width_filter(Mat &img,int width_max=25)
        {
			uchar last_point;
			int count_white;
            int start;
			uchar now_point;
			for(int r = 0;r < img.rows;r++)
            {
			    uchar *pixel = img.ptr<uchar>(r);
                last_point = *(pixel+0);
                start  = -1;
                count_white = 0;
	        	for(int c = 1; c<img.cols;c++)
                {
                    now_point = *(pixel+c);
                    if(last_point == 0 && now_point > 0)
                        start = c;
                    if(start != -1)
                        count_white++;
                    if(last_point > 0 && now_point == 0)
                    {
                        if(count_white >= width_max)
                        {
                            for(int k = start;k < c;k++)
                                *(pixel+k) = 0;
                        }
                        start = -1;
                        count_white = 0;
                    }
                    last_point = now_point;
		        }
            }    
		}

	    Mat resize_image(Mat input,float alpha=0.5){
		    Mat output;
		    cv::resize(input,output,Size(input.cols*alpha,input.rows*alpha));
		    return output;
	    }
        vector<Vec4i> houghLines(Mat img_mask) {
            vector<Vec4i> line;
            // rho and theta are selected by trial and error
            HoughLinesP(img_mask, line, 1, CV_PI/180, 20, 20, 3500);
            return line;
        }
        vector<vector<Vec4i>> lineSeparation(vector<Vec4i> lines,Mat img_edges) {
            vector<vector<Vec4i>> output(2);
            size_t j = 0;
            Point ini;
            Point fini;
            double slope_thresh = 0.3;
            vector<double> slopes;
            vector<Vec4i> selected_lines;
            vector<Vec4i> right_lines, left_lines;
            // Calculate the slope of all the detected lines
            for (auto i : lines) {
                ini =  Point(i[0], i[1]);
                fini = Point(i[2], i[3]);
                // Basic algebra: slope = (y1 - y0)/(x1 - x0)
                double slope = (static_cast<double>(fini.y) - static_cast<double>(ini.y))/(static_cast<double>(fini.x) - static_cast<double>(ini.x) + 0.00001);
                // If the slope is too horizontal, discard the line
                // If not, save them  and their respective slope
                if (abs(slope) > slope_thresh) {
                    slopes.push_back(slope);
                    selected_lines.push_back(i);
                }
            }
            // Split the lines into right and left lines
            img_center = static_cast<double>((img_edges.cols / 2));
            while (j < selected_lines.size()) {
                ini = Point(selected_lines[j][0], selected_lines[j][1]);
                fini = Point(selected_lines[j][2], selected_lines[j][3]);
                // Condition to classify line as left side or right side
                if (slopes[j] > 0 && fini.x > img_center && ini.x > img_center) {
                    right_lines.push_back(selected_lines[j]);
                    right_flag = true;
                } 
                else if (slopes[j] < 0 && fini.x < img_center && ini.x < img_center) {
                    left_lines.push_back(selected_lines[j]);
                    left_flag = true;
                }
                 j++;
            }
            output[0] = right_lines;
            output[1] = left_lines;
            return output;
            }    
        vector<Point> regression(vector<vector<Vec4i>> left_right_lines,Mat inputImage) {
            vector<Point> output(4);
            Point ini;
            Point fini;
            Point ini2;
            Point fini2;
            Vec4d right_line;
            Vec4d left_line;
            vector<Point> right_pts;
            vector<Point> left_pts;
            // If right lines are being detected, fit a line using all the init and final points of the lines
            if (right_flag == true) {
                for (auto i : left_right_lines[0]) {
                    ini = Point(i[0], i[1]);
                    fini = Point(i[2], i[3]);
                    right_pts.push_back(ini);
                    right_pts.push_back(fini);
                }
                if (right_pts.size() > 0) {
                    // The right line is formed here
                    fitLine(right_pts, right_line, CV_DIST_L2, 0, 0.01, 0.01);
                    right_m = right_line[1] / right_line[0];
                    right_b = Point(right_line[2], right_line[3]);
                }
            }
            // If left lines are being detected, fit a line using all the init and final points of the lines
            if (left_flag == true) {
                for (auto j : left_right_lines[1]) {
                    ini2 = Point(j[0], j[1]);
                    fini2 = Point(j[2], j[3]);
                    left_pts.push_back(ini2);
                    left_pts.push_back(fini2);
                }
                if (left_pts.size() > 0) {
                    // The left line is formed here
                    fitLine(left_pts, left_line, CV_DIST_L2, 0, 0.01, 0.01);
                    left_m = left_line[1] / left_line[0];
                    left_b = Point(left_line[2], left_line[3]);
                }
            }
            // One the slope and offset points have been obtained, apply the line equation to obtain the line points
            int ini_y = inputImage.rows;
            int fin_y = 1;
            double right_ini_x = ((ini_y - right_b.y) / right_m) + right_b.x;
            double right_fin_x = ((fin_y - right_b.y) / right_m) + right_b.x;
            double left_ini_x = ((ini_y - left_b.y) / left_m) + left_b.x;
            double left_fin_x = ((fin_y - left_b.y) / left_m) + left_b.x;
            output[0] = Point(right_ini_x, ini_y);
            output[1] = Point(right_fin_x, fin_y);
            output[2] = Point(left_ini_x, ini_y);
            output[3] = Point(left_fin_x, fin_y);
            return output;
        }
        Mat plotLanes(Mat inputImage,vector<Point> lane) {
            vector<Point> poly_points;
            Point lane_1,lane_2;
            Mat output;
            // Create the transparent polygon for a better visualization of the lane
            inputImage.copyTo(output);
            poly_points.push_back(lane[2]);
            poly_points.push_back(lane[0]);
            poly_points.push_back(lane[1]);
            poly_points.push_back(lane[3]);
            float alpha = 0.7;
            float beta = 1.0-alpha;
            if(left_flag&&right_flag){
                // Plot both lines of the lane boundary
                //Right line
                line(output, lane[0], lane[1], Scalar(0, 255, 255), 8, CV_LU); 
                //Left line
                line(output, lane[2], lane[3], Scalar(0, 255, 255), 8,  CV_LU);
                x_offset = static_cast<double>((lane[3].x+lane[1].x)/2-(output.cols/2));
                y_offset = static_cast<double>(output.rows - 1);
		        fillConvexPoly(inputImage, poly_points, Scalar(255, 0, 0), CV_LU, 0);
            	addWeighted(output, alpha, inputImage, beta, 0.0, output);
            }
            else if(left_flag){
                //Plot Left line
                line(output, lane[2], lane[3], Scalar(0, 255, 255), 8,  CV_LU);
                x_offset = static_cast<double>(lane[3].x-lane[2].x);
                y_offset = static_cast<double>(output.rows - 1);
            }
            else if(right_flag){
                //Plot Right line
                line(output, lane[0], lane[1], Scalar(0, 255, 255), 8,  CV_LU); 
                x_offset = static_cast<double>(lane[1].x-lane[0].x);
                y_offset = static_cast<double>(output.rows - 1);
            }
            angle_to_mid_radian = atan(x_offset / y_offset);
            angle_to_mid_deg = static_cast<int>(angle_to_mid_radian * 180.0 / 3.14159265);  
            steering_angle = angle_to_mid_deg + 90;
            steering_angle_radian = steering_angle / 180.0 *3.14159265;
            lane_1.x = static_cast<int>(output.cols/2);
            lane_1.y = static_cast<int>(output.rows);
            lane_2.x = static_cast<int>(lane_1.x - output.rows/2/tan(steering_angle_radian));
            lane_2.y = static_cast<int>(1); 
            //Plot the direction line 
            line(output,lane_1,lane_2,Scalar(0,0,255),8);
            // Plot the turn message
            ss.str(" ");
            ss.clear();
            ss<<"[ANGLE]:"<<steering_angle;
            putText(output, ss.str(), Point2f(2,20), 0,2, Scalar(255,0,255), 1);
	        left_flag=right_flag=false;
            return output;
        }
};
int main(int argc,char **argv){
    ros::init(argc, argv, "line_detection");
    LineDetection *ic= new  LineDetection;
    ros::spin();
    return 0;
}

