#include "Marker.h"

void CMarker::markFaces(Mat& img, const std::vector<ptr_face_t>& vFaces)
{
	for (auto face : vFaces) {
		// ------ PUT YOUR CODE HERE -------
		// 1. Draw all faces using face->getArea();
		// 2. Print the text using face->getText();
		// 3. Print face id using face->getId();
        
        Rect area = face->getArea();
        std::string text = face->getText();
        int id = face->getID();
        std::string strID = std::to_string(id);
        
        Point face_centercenter(area.x + area.width/2, area.y + area.height/2);
        Point face_topleft(area.x, area.y);
        Point face_bottomleft(area.x, area.y + area.height);
        ellipse(img, face_centercenter, Size(area.width/2, area.height/2), 0, 0, 360, Scalar(0, 255, 255), 4);
        putText(img, text, face_topleft, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 255), 3);
        putText(img, strID, face_bottomleft, FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 255), 3);
	}
}

void CMarker::markPoints(Mat& img, const std::vector<Point2f>& vPoints)
{
	for (auto& point : vPoints) 
		circle(img, point, 3, CV_RGB(255, 0, 0), 2);
}

void CMarker::markVecOFF(Mat& img, Mat& recent, Point2f recent_corner, Point2f old_corner)
{
	// ------ PUT YOUR CODE HERE -------
    line(img, recent_corner, old_corner, Scalar(255, 255, 255), 2);
    circle(recent, recent_corner, 5, Scalar(255, 165, 0), -1);
}

void CMarker::markGUI(Mat& img, std::string fps)
{
	// ------ PUT YOUR CODE HERE -------
	// Implement yout ouw GUI
	// Show fps
	for (int y = 0; y < img.rows; y++) {
		Vec3b* ptr = img.ptr<Vec3b>(y);
		for (int x = 0; x < img.cols; x++) {
			float k = static_cast<float>(x) / img.cols;
			ptr[x] = Vec3b(k * 255, 0, 255 - k * 255);
		}
	}
	GaussianBlur(img, img, Size(17, 17), 50);
	putText(img, "Human Computer Interaction", Point(10, 50), FONT_HERSHEY_SIMPLEX, 1.5, Scalar(0, 0, 0), 5);
    putText(img, fps, Point(10, 700), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 255, 255), 3);
}
