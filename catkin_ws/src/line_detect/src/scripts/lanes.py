#!/usr/bin/env python3
import sys
import rospy
import cv2
import numpy as np
import matplotlib.pyplot as plt
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

class image_converter:
     def __init__(self):
         self.bridge = CvBridge()
         self.image_sub = rospy.Subscriber("/jetbot_camera/raw",Image,self.callback)   
     def callback(self,data):
         try: 
             cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")
         except CvBridgeError as e:
             print(e)
         img_resize = self.resize_image(cv_image,alpha=0.25)
         img_rotate = self.rotate_image(img_resize)
         warped_img = self.transform_image(img_rotate)
         gradient =  self.canny_and_mask(warped_img)
         #left_fit, right_fit,left_lane_inds, right_lane_inds, nonzerox, nonzeroy = self.locate_lines(warped_img) 
         #image_lines=self.visulizeLanes(left_fit, right_fit, left_lane_inds, right_lane_inds, warped_img, nonzerox, nonzeroy, margin = 20)         
         #left_curvature, right_curvature, center = self.radius_curvature(warped_img, left_fit, right_fit)
         #final_img = self.draw_on_image(img_rotate, warped_img, left_fit, right_fit, M, left_curvature, right_curvature, center)
         cv2.imshow("Image window",gradient)
         cv2.waitKey(1)

     def rotate_image(self,img):
         #get image height , width
         (h,w)=img.shape[:2]
         #calculate the center of image
         center=(w/2,h/2)
         #rotate image 180 degrees
         M=cv2.getRotationMatrix2D(center,180,1.0)
         rotated180=cv2.warpAffine(img,M,(w,h))
         return rotated180
     def  resize_image(self,img,alpha=0.5):
         width = int(img.shape[1]*alpha)
         height = int(img.shape[0]*alpha)
         resized = cv2.resize(img,(width,height),interpolation = cv2.INTER_AREA)
         return resized
     def canny_and_mask(self,img):
         #convert to gray
         kernel = np.ones((5,5),np.uint8)
         gray=cv2.cvtColor(img,cv2.COLOR_BGR2GRAY)
         #apply filter for smoothing image
         blur=cv2.medianBlur(gray,5)
         #apply canny for obtaining the threshold
         canny = cv2.Canny(blur,900,900)
         mask = cv2.inRange(blur, 125, 255)
         canny_mask = cv2.add(canny,mask)
         #opening = cv2.morphologyEx(canny_mask, cv2.MORPH_OPEN, kernel)
         closing = cv2.morphologyEx(canny_mask, cv2.MORPH_CLOSE, kernel)
         return closing
     def transform_image(self,img):
         #Grap the image shape 
         img_size = (img.shape[1],img.shape[0])
         leftupperpoint = [600*0.25,720*0.25]
         rightupperpoint = [1964*0.25,720*0.25]
         leftlowerpoint = [0,1240*0.25]
         rightlowerpoint = [464*0.25,1240*0.25]
         src = np.float32([leftupperpoint,leftlowerpoint,rightupperpoint,rightlowerpoint])
         dst = np.float32([[0,0],[2464*0.25,0],[0,1440*0.25],[2464*0.25,1440*0.25]])
         #Given src and dst points, calculate the perspective transform matrix
         M = cv2.getPerspectiveTransform(src,dst)
         #Warp the image using opencv warpPerspective()
         warped=cv2.warpPerspective(img,M,img_size,flags=cv2.INTER_NEAREST)
         return warped
     
     def locate_lines(self,binary_warped, nwindows = 6, margin = 20, minpix = 10):
         # Assuming you have created a warped binary image called "binary_warped"
         # Take a histogram of the bottom half of the image
         histogram = np.sum(binary_warped[int(binary_warped.shape[0]/2):,:],axis=0) 
         # Find the peak of the left and right halves of the histogram
         # These will be the starting point for the left and right lines
         midpoint = np.int(histogram.shape[0]/2)
         leftx_base = np.argmax(histogram[:midpoint])
         rightx_base = np.argmax(histogram[midpoint:]) + midpoint
         # Set height of windows
         window_height = np.int(binary_warped.shape[0]/nwindows)
         # Identify the x and y positions of all nonzero pixels in the image
         nonzero = binary_warped.nonzero()
         nonzeroy = np.array(nonzero[0])
         nonzerox = np.array(nonzero[1])
         # Current positions to be updated for each window
         leftx_current = leftx_base
         rightx_current = rightx_base
         # Create empty lists to receive left and right lane pixel indices
         left_lane_inds = []
         right_lane_inds = []
         # Create an image to draw on and an image to show the selection window
         out_img = np.dstack((binary_warped, binary_warped, binary_warped))*255
         # Step through the windows one by one
         for window in range(nwindows):
             # Identify window boundaries in x and y (and right and left)
             win_y_low = binary_warped.shape[0] - (window+1)*window_height
             win_y_high = binary_warped.shape[0] - window*window_height
             win_xleft_low = leftx_current - margin
             win_xleft_high = leftx_current + margin
             win_xright_low = rightx_current - margin
             win_xright_high = rightx_current + margin
             # Identify the nonzero pixels in x and y within the window
             good_left_inds = ((nonzeroy >= win_y_low) & (nonzeroy < win_y_high) & (nonzerox >= win_xleft_low) &  (nonzerox < win_xleft_high)).nonzero()[0]
             good_right_inds = ((nonzeroy >= win_y_low) & (nonzeroy < win_y_high) & (nonzerox >= win_xright_low) &  (nonzerox < win_xright_high)).nonzero()[0]
             # Append these indices to the lists
             left_lane_inds.append(good_left_inds)
             right_lane_inds.append(good_right_inds)
             # If you found > minpix pixels, recenter next window on their mean position
             if len(good_left_inds) > minpix:
                 leftx_current = np.int(np.mean(nonzerox[good_left_inds]))
             if len(good_right_inds) > minpix:        
                 rightx_current = np.int(np.mean(nonzerox[good_right_inds]))
         # Concatenate the arrays of indices
         left_lane_inds = np.concatenate(left_lane_inds)
         right_lane_inds = np.concatenate(right_lane_inds)
         # Extract left and right line pixel positions
         leftx = nonzerox[left_lane_inds]
         lefty = nonzeroy[left_lane_inds] 
         rightx = nonzerox[right_lane_inds]
         righty = nonzeroy[right_lane_inds] 
         # Fit a second order polynomial to each
         left_fit = np.polyfit(lefty, leftx, 2)
         right_fit = np.polyfit(righty, rightx, 2)
         return left_fit, right_fit,left_lane_inds, right_lane_inds, nonzerox, nonzeroy

     def locate_line_further(self,left_fit, right_fit, binary_warped): 
         nonzero = binary_warped.nonzero()
         nonzeroy = np.array(nonzero[0])
         nonzerox = np.array(nonzero[1])
         margin = 20
         left_lane_inds = ((nonzerox > (left_fit[0]*(nonzeroy**2) + left_fit[1]*nonzeroy + left_fit[2] - margin)) & (nonzerox < (left_fit[0]*(nonzeroy**2) + left_fit[1]*nonzeroy + left_fit[2] + margin))) 
         right_lane_inds = ((nonzerox > (right_fit[0]*(nonzeroy**2) + right_fit[1]*nonzeroy + right_fit[2] - margin)) & (nonzerox < (right_fit[0]*(nonzeroy**2) + right_fit[1]*nonzeroy + right_fit[2] + margin)))  
         # Again, extract left and right line pixel positions
         leftx = nonzerox[left_lane_inds]
         lefty = nonzeroy[left_lane_inds] 
         rightx = nonzerox[right_lane_inds]
         righty = nonzeroy[right_lane_inds]
         # Fit a second order polynomial to each
         if len(leftx) == 0:
             left_fit_new =[]
         else:
             left_fit_new = np.polyfit(lefty, leftx, 2)
    
         if len(rightx) == 0:
             right_fit_new =[]
         else:
             right_fit_new = np.polyfit(righty, rightx, 2)
         return left_fit_new,right_fit_new

     def visulizeLanes(self,left_fit, right_fit, left_lane_inds, right_lane_inds, binary_warped, nonzerox, nonzeroy, margin = 20):
         # Generate x and y values for plotting
         ploty = np.linspace(0, binary_warped.shape[0]-1, binary_warped.shape[0])
         left_fitx = left_fit[0]*ploty**2 + left_fit[1]*ploty + left_fit[2]
         right_fitx = right_fit[0]*ploty**2 + right_fit[1]*ploty + right_fit[2] 
         # Create an image to draw on and an image to show the selection window
         out_img = np.dstack((binary_warped, binary_warped, binary_warped))*255
         window_img = np.zeros_like(out_img)
         # Color in left and right line pixels
         out_img[nonzeroy[left_lane_inds], nonzerox[left_lane_inds]] = [0, 255, 0]
         out_img[nonzeroy[right_lane_inds], nonzerox[right_lane_inds]] = [0, 0, 255]
         # Generate a polygon to illustrate the search window area
         # And recast the x and y points into usable format for cv2.fillPoly()
         left_line_window1 = np.array([np.transpose(np.vstack([left_fitx-margin, ploty]))])
         left_line_window2 = np.array([np.flipud(np.transpose(np.vstack([left_fitx+margin,ploty])))])
         left_line_pts = np.hstack((left_line_window1, left_line_window2))
         right_line_window1 = np.array([np.transpose(np.vstack([right_fitx-margin, ploty]))])
         right_line_window2 = np.array([np.flipud(np.transpose(np.vstack([right_fitx+margin,ploty])))])
         right_line_pts = np.hstack((right_line_window1, right_line_window2))
         # Draw the lane onto the warped blank image
         cv2.fillPoly(window_img, np.int_([left_line_pts]), (255,0,0))
         cv2.fillPoly(window_img, np.int_([right_line_pts]), (255,0,0))
         result = cv2.addWeighted(out_img, 1, window_img, 0.3, 0)
         return result 
     #Radius of Curvature
     def radius_curvature(self,binary_warped, left_fit, right_fit):
         ploty = np.linspace(0, binary_warped.shape[0]-1, binary_warped.shape[0] )
         left_fitx = left_fit[0]*ploty**2 + left_fit[1]*ploty + left_fit[2]
         right_fitx = right_fit[0]*ploty**2 + right_fit[1]*ploty + right_fit[2]
         y_eval = np.max(ploty)
         # Fit new polynomials to x,y in world space
         left_fit_cr = np.polyfit(ploty, left_fitx, 2)
         right_fit_cr = np.polyfit(ploty, right_fitx, 2)
         #  Calculate the new radii of curvature
         left_curvature =  ((1 + (2*left_fit_cr[0] *y_eval + left_fit_cr[1])**2) **1.5) / np.absolute(2*left_fit_cr[0])
         right_curvature = ((1 + (2*right_fit_cr[0]*y_eval + right_fit_cr[1])**2)**1.5) / np.absolute(2*right_fit_cr[0])    
         # Calculate vehicle center
         #left_lane and right lane bottom in pixels
         left_lane_bottom = (left_fit[0]*y_eval)**2 + left_fit[0]*y_eval + left_fit[2]
         right_lane_bottom = (right_fit[0]*y_eval)**2 + right_fit[0]*y_eval + right_fit[2]
         # Lane center as mid of left and right lane bottom                        
         lane_center = (left_lane_bottom + right_lane_bottom)/2.
         center_image = 120
         center = (lane_center - center_image)
         position = "left" if center < 0 else "right"
         center = "Vehicle is {:.2f}m {}".format(center, position)
         # Now our radius of curvature is in meters
         return left_curvature, right_curvature, center

def main(args):
    ic = image_converter()
    rospy.init_node('image_converter', anonymous=True)
    try:
        rospy.spin()
    except KeyboardInterrupt:
        print("Shutting down")
    cv2.destroyAllWindows()
if __name__ == '__main__':
    main(sys.argv)