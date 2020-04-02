#pragma once

#include "types.h"
#include "Face.h"

class CMarker {
public:
	// draws faces into the image
	static void markFaces(Mat& img, const std::vector<ptr_face_t>& vFaces);
	
	// draws the points (e.g. from GoodFeaturesToTrack) into the image
	static void markPoints(Mat& img, const std::vector<Point2f>& vPoints);
	
	// draws vectors of the vector Optical Flow Field () into the image
	static void markVecOFF(Mat& img, Mat& recent, Point2f recent_corner, Point2f old_corner);
	
	// draws GUI
	static void markGUI(Mat& img, std::string fps);
};

