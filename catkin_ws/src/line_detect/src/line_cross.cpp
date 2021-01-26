#include<iostream>
#include<math.h>
#include<string>
#include<ros/ros.h>
#include <std_msgs/String.h>
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
static const std::string OPENCV_WINDOW = "CROSS DETECTION";
class CrossDetection{
    private:
        ros::NodeHandle nh_;
        image_transport::ImageTransport it_;
        image_transport::Subscriber image_sub_;
        ros::Publisher cross_pub;
        std_msgs::UInt8 cross_message;
        std::chrono::time_point<std::chrono::system_clock> start,end;
        std::chrono::duration<double> elapsed_seconds;
        stringstream ss,ss_angle;
        /*variable image*/
        Mat frame;
        Mat img_edges;
        Mat img_lines;
        Mat img_perspective;
		vector<Point> left_points; 
        vector<Point> left_line;
        float polyleft[3];
        Point2f Source[4];
        Point2f Destination[4];
        int distance_to_cross;
		int center_cam;
    	int center_lines;
		float angle_cross_deg;
    public:
        //Constructor
        CrossDetection():it_(nh_){
        // Subscrive to input video feed and publish output video feed
        image_sub_ = it_.subscribe("/jetson_camera/raw",1,&CrossDetection::CrossDetectionCb, this);
        cross_pub = nh_.advertise<std_msgs::UInt8>("/distance_cross",1000);
        //cv::namedWindow(OPENCV_WINDOW, WINDOW_KEEPRATIO);
        Source[0]=Point2f(824*0.2,670*0.2);
        Source[1]=Point2f(1640*0.2,670*0.2);
        Source[2]=Point2f(14*0.2,1200*0.2);
        Source[3]=Point2f(2450*0.2,1200*0.2);
        //init points to desination
        Destination[0]=Point2f(314*0.2,0);
        Destination[1]=Point2f(2150*0.2,0);
        Destination[2]=Point2f(314*0.2,1440*0.2);
        Destination[3]=Point2f(2150*0.2,1440*0.2);  
        distance_to_cross = 1000;
	    center_cam = 0;
        center_lines = 0;
		angle_cross_deg = 0;
        }
        //Destructor
        ~CrossDetection(){
            //cv::destroyWindow(OPENCV_WINDOW);
        }
        //Callback funtion
        void CrossDetectionCb(const sensor_msgs::ImageConstPtr& msg){
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
            Perspective(frame);
            cv::rotate(frame,frame,ROTATE_90_COUNTERCLOCKWISE); 
	    	img_edges = Threshold(frame);
	    	locate_lanes(img_edges,frame);
            draw_lines(frame);
	    //	resizeWindow(OPENCV_WINDOW,frame.cols,frame.rows);
            //cv::imshow(OPENCV_WINDOW,frame);
            //cv::waitKey(5);
            // Output modified video stream
	    	if(angle_cross_deg>=80.0 &&  angle_cross_deg<=100.0){
	    		cross_message.data = distance_to_cross;
            	cross_pub.publish(cross_message);
	    	}
	    	else{
	        	cross_message.data = img_perspective.cols-1;
            	cross_pub.publish(cross_message);
	     	}
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
        void Perspective(Mat &frame){
            //Mat output;
 	        Mat matrixPerspective( 2, 4, CV_32FC1 );
	        matrixPerspective = Mat::zeros( frame.rows, frame.cols, frame.type());
            matrixPerspective = getPerspectiveTransform(Source,Destination); 
			warpPerspective(frame,frame,matrixPerspective,Size(frame.cols,frame.rows));
        }

        Mat Threshold(Mat frame){
            Mat frameThreshold;
	    	frame.convertTo(frame, -1, 1, -15);
            cvtColor(frame,frameThreshold,COLOR_BGR2GRAY);
	    	medianBlur(frameThreshold,frameThreshold,5);
	    	erode(frameThreshold,frameThreshold, getStructuringElement(MORPH_RECT, Size(3,3)));
	    	dilate(frameThreshold,frameThreshold, getStructuringElement(MORPH_RECT, Size(4,4)));
            inRange(frameThreshold,105,255,frameThreshold);
            return frameThreshold;
        }
	    Mat resize_image(Mat input,float alpha=0.5){
		    Mat output;
		    cv::resize(input,output,Size(input.cols*alpha,input.rows*alpha));
		    return output;
	    }
		void width_filter(Mat &img,int width_max=30){
			int last_point = 0;
			int count_white = 0;
			uchar now_point = 0; 
	    	int middle_step;
			bool black_to_white = false;
			int white_left = 0;
			int white_right = 0;
			bool big_white = false;
			uchar *pixel_l = img.ptr<uchar>(img.rows/4);
			uchar *pixel_c = img.ptr<uchar>(img.rows/2);
			uchar *pixel_h = img.ptr<uchar>(img.rows*3/4);
	        for(int c =0; c<img.cols;c++){
		        if(*(pixel_l+c)>0 || *(pixel_c+c)>0||*(pixel_h+c)>0){
			    if(c<img.cols/2)
		    	    white_left++;
			    else if(c>=img.cols/2)
				    white_right++;
 		        }
	        }      
			if(white_right>white_left){
				for(int r=0;r<img.rows;r++){
				uchar *pixel = img.ptr<uchar>(r);
	        		for(int c =0; c<img.cols;c++){
					if(!big_white){
		        		now_point = *(pixel+c);	 
		        		if(now_point>0){
		    	    		count_white++;
 		        		}
		        		if(now_point>0 &&  last_point==0){
			    		count_white = 0;	
			    		black_to_white = true;
					}	
					else if(now_point==0 &&  last_point>0){	
				    	black_to_white = false;
					}	    	
					if(black_to_white && (count_white>=width_max)){
			   			c = c-(width_max+1);
			   			big_white = true;
					}
	            		last_point = now_point;
					}
					else
				    	*(pixel+c)=0;
	        		}
					big_white = false;      
		        }
		    }
		    else {
		 		for(int r=0;r<img.rows;r++){
				uchar *pixel = img.ptr<uchar>(r);
	        		for(int c =img.cols-1;c>1;c--){
					if(!big_white){
		        		now_point = *(pixel+c);	  
		        		if(now_point>0){
		    	    		count_white++;
 		        		}
		        	if(now_point>0 &&  last_point==0){
			    		count_white = 0;	
			    		black_to_white = true;
					}	
					else if(now_point==0 &&  last_point>0){	
				    	black_to_white = false;
					}	    	
					if(black_to_white && (count_white>=width_max)){
			   			c = c+(width_max+1);
			   			big_white = true;
					}
	            	last_point = now_point;
					}
					else
				     	*(pixel+c)=0;
	        	  	}
					big_white = false;      
		     	}
			}
		}
	    int *Histogram(Mat &img){
            //Create histogram with the length of the width of the frame 
	        vector<int> histogramLane;
	        static int LanePosition[2];
			int init_row,end_row;
	        Mat ROILane;
	        Mat frame;  
		    init_row = img.rows/2;	
		    end_row = img.rows/2-1;
	    	img.copyTo(frame);
            for(int i=0;i<img.cols;i++){
                //Region interest
                ROILane=frame(Rect(i,init_row ,1,end_row));
                //Normal values 
                divide(255,ROILane,ROILane);
                //add the value 
                histogramLane.push_back((int)(sum(ROILane)[0]));
            } 
            //Find line left
            vector<int>:: iterator LeftPtr;
            LeftPtr = max_element(histogramLane.begin(),histogramLane.begin()+img.cols/2);
            LanePosition[0] = distance(histogramLane.begin(),LeftPtr);
            //find line right
            vector<int>:: iterator RightPtr;
            RightPtr = max_element(histogramLane.begin()+(img.cols/2)+1,histogramLane.end());
            LanePosition[1] = distance(histogramLane.begin(),RightPtr);
	        return  LanePosition;
        }

	void locate_lanes(Mat &img,Mat &out_img){
	    Mat region_interest;
	    left_points.clear();
	    int nwindows , margin,minpix;
	    int win_y_low = 0, win_y_high = 0, win_xleft_low = 0, win_xleft_high = 0, win_xright_low = 0, win_xright_high = 0;
	    int leftx_current;
	    int mean_leftx,count_left;
	    int *locate_histogram;
	    bool isMaxLeft;
        uchar max_left=0;
		uchar now_left_point;
        Point max_left_point;
	    nwindows = 12;
	    int window_height = img.rows/nwindows;
	    locate_histogram = Histogram(img);
	    leftx_current = locate_histogram[0];
	    // Set the width of the windows +/- margin
	    margin = 40;
	    minpix = 14;
	    // Set minimum number of pixels found to recenter window
	    for(int window=0;window<nwindows;window++){
	        mean_leftx = 0;
	        count_left = 0;
	        win_y_low = img.rows - (window+1)*window_height;
    	    win_y_high = img.rows - window*window_height;
   	        win_xleft_low = leftx_current - margin;
    	    win_xleft_high = leftx_current + margin;
	        if( win_xleft_low<0)
		        win_xleft_low = 0+1;
	        if(win_xleft_high>= img.cols-1)
	    	    win_xleft_high =  img.cols-1;
	        if(win_y_high>=img.rows-1)
		        win_y_high = img.rows-1;
	        if(win_y_low<=0)
		        win_y_low = 1;
	        for(int r = win_y_low;r<win_y_high;r++){
                max_left = img.at<uchar>(r,win_xleft_low);
				isMaxLeft = false;
		        for(int cl = win_xleft_low+1;cl<win_xleft_high;cl++){
					now_left_point = img.at<uchar>(r,cl);
		            if(now_left_point >0 && now_left_point >=max_left){
                        	max_left_point.x = r;
                        	max_left_point.y = cl; 
                        	max_left = now_left_point;
							isMaxLeft = true;
    		        }		
		        }
		    	if(isMaxLeft){
			 		left_points.push_back(max_left_point);
		    		mean_leftx += max_left_point.y; 
	                count_left++; 
		    	}
	    	}
	    if(count_left>=minpix){
	    	mean_leftx /=  count_left;
		leftx_current = mean_leftx;
   	    }
	    rectangle(out_img,cv::Point(win_xleft_low,win_y_low),cv::Point(win_xleft_high,win_y_high),cv::Scalar(0,255,0));
	    }
	}
	bool regression_left(){
        if(left_points.size()<266)
            return false;
	    long sumX[3] = {0,0,0};
	    long sumY[2] = {0,0};
	    long pow2 = 0;
	    for(auto point=left_points.begin();point!=left_points.end();point++){
		    pow2 = (point->x)*(point->x);
	        sumX[0]++;
		    sumX[1]+= (point->x);
		    sumX[2]+= pow2;
		    sumY[0]+= (point->y);
		    sumY[1]+= (point->y)*(point->x);
	    }	
        solve_system(sumX,sumY,polyleft);
        return true;
	}
 	void solve_system(long *sX,long *sY,float *x){
	    int n,i,j,k;
        n=2;
    	float a[n][n+1];
        //declare an array to store the elements of augmented-matrix    
    	//"Enter the elements of the augmented-matrix row-wise
        a[0][0]=sX[0];   
        a[0][1]=sX[1];    
        a[0][2]=sY[0];
        ////////////////   
        a[1][0]=sX[1];   
        a[1][1]=sX[2];     
        a[1][2]=sY[1];  
        ////////////////
        for (i=0;i<n;i++)                    //Pivotisation
            for (k=i+1;k<n;k++)
                if (abs(a[i][i])<abs(a[k][i]))
                    for (j=0;j<=n;j++){
                        double temp=a[i][j];
                        a[i][j]=a[k][j];
                        a[k][j]=temp;
                    }
        for (i=0;i<n-1;i++)            //loop to perform the gauss elimination
            for (k=i+1;k<n;k++){
                double t=a[k][i]/a[i][i];
                for (j=0;j<=n;j++)
                    a[k][j]=a[k][j]-t*a[i][j];    //make the elements below the pivot elements equal to zero or elimnate the variables
            }
        for (i=n-1;i>=0;i--)                //back-substitution
        {                        //x is an array whose values correspond to the values of x,y,z..
            x[i]=a[i][n];                //make the variable to be calculated equal to the rhs of the last equation
            for (j=i+1;j<n;j++)
                if (j!=i)            //then subtract all the lhs values except the coefficient of the variable whose value                                   is being calculated
                    x[i]=x[i]-a[i][j]*x[j];
            x[i]=x[i]/a[i][i];            //now finally divide the rhs by the coefficient of the variable to be calculated
        } 
	}

    void draw_lines(Mat &img){
	    int column;
	    int row;
	    Point half_line,half_start;
        center_cam =(img.cols/2)+1;
        center_lines =0;
	    float angle_cross_radian;
	    left_line.clear();
        if(regression_left()){
            for(row=(img.rows/4);row<(img.rows*3/4)-1;row+=6){
                column = polyleft[0] + polyleft[1]*(row);
		left_line.push_back(cv::Point(column,row));
                circle(img,cv::Point(column,row),cvRound((double)4/ 2), Scalar(255,0,0), -3);
                }
		half_line.x = ((left_line.end()-1)->x +left_line.begin()->x)/2;
		half_line.y = img.rows/2;
		half_start.x = (img.cols-1);
		half_start.y = img.rows/2;
		line(img,half_line,half_start,Scalar(0,0,255),4);
		distance_to_cross = half_start.x - half_line.x; 
		angle_cross_radian = atan(static_cast<float>((left_line.end()-1)->y - left_line.begin()->y)/static_cast<float>((left_line.end()-1)->x - left_line.begin()->x));
            angle_cross_deg = static_cast<int>(angle_cross_radian * 180.0 / 3.14159265);  
            if(angle_cross_deg<0 && angle_cross_deg>(0-90))
                angle_cross_deg = (0-1)*(angle_cross_deg);
            else if(angle_cross_deg>0 && angle_cross_deg<90 )
                angle_cross_deg = 180 - angle_cross_deg;
            }
	    else 
		angle_cross_deg = 0;
            ss.str(" ");
            ss.clear();
            ss<<"[DIST]: "<<distance_to_cross;
            putText(img, ss.str(), Point2f(2,20), 0,1, Scalar(0,255,255), 1);
        }
};
int main(int argc,char **argv){
    ros::init(argc, argv, "cross_detection");
    CrossDetection *ic= new  CrossDetection;
    ros::spin();
    return 0;
}
