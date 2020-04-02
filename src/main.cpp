#include "types.h"
#include "CameraController.h"
#include "Face.h"
#include "Marker.h"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/video.hpp"

// Function Headers
std::vector<ptr_face_t> detectFaces(Mat frame);
std::vector<Point2f> featureExtraction(Mat frame);
std::vector<Point2f> faceFeatureExtraction(Mat frame, std::vector<ptr_face_t> vpFaces);
std::vector<Point2f> calcOpticalFlow(Mat old_gray, Mat recent_gray, std::vector<Point2f> old_corners);

// Global variables
CascadeClassifier face_cascade;

int main(int argc, const char** argv) {
    
    // Random colors
    std::vector<Scalar> colors;
    RNG rng;
    for(int i = 0; i < 100; i++) {
        int r = rng.uniform(0, 256);
        int g = rng.uniform(0, 256);
        int b = rng.uniform(0, 256);
        colors.push_back(Scalar(r,g,b));
    }
    
    std::string fps = ""; // Frames per second
	std::vector<Point2f> vPoints;
	std::vector<ptr_face_t> vpFaces;
	
	namedWindow("Camera");
	
	// mouse callback function which fills vPoints with coordinates of mouse clicks
	setMouseCallback("Camera", [] (int event, int x, int y, int flags, void* userdata) {
		if (userdata && event == EVENT_LBUTTONDOWN) {
			std::vector<Point2f> *pvPoints = (std::vector<Point2f> *) userdata;
			pvPoints->push_back(Point2f(x, y));
		}
	}, (void*) &vPoints);
	
	CCameraCantroller controller(16);

	controller.init();
	controller.start();
	
	// Main loop
	Mat img, mask;
	float attenuation = 0.5f;
    int frameCount = 0;
    
    Mat old, old_gray;
    Mat recent, recent_gray;
    std::vector<Point2f> old_corners, good_recent_corners; // Features
    
	for(;;) {
		img = controller.getFrame();

		if (!img.empty()) {
            frameCount++;
			if (mask.empty()) mask = Mat(img.size(), img.type());
			mask.setTo(0);

			// ------ PUT YOUR CODE HERE -------
			if (frameCount%10 == 0) vpFaces = detectFaces(img);
//			  CMarker::markFaces(mask, vpFaces);
//            add(img, mask, img);
            
            // ------ PUT YOUR CODE HERE -------
            vPoints = featureExtraction(img);
//            CMarker::markPoints(mask, vPoints, Scalar(255, 0, 0));
//            add(img, mask, img);

            if (old_gray.empty() || old_corners.empty()) {
                //First frame
                old = img.clone();
                cvtColor(old, old_gray, COLOR_BGR2GRAY);

                // Find corners of first frame
                old_corners = featureExtraction(img);
                continue;
            }

            if (frameCount%2 == 0) {
                recent = img.clone();
                cvtColor(recent, recent_gray, COLOR_BGR2GRAY);

                good_recent_corners = calcOpticalFlow(old_gray, recent_gray, old_corners);
                // Optical Flow Visualization
                for(int i = 0; i < good_recent_corners.size(); i++) {
                    CMarker::markVecOFF(mask, recent, good_recent_corners[i], old_corners[i], colors[i]);
                    add(recent, mask, img);
                }

                old_gray = recent_gray.clone(); // Update the previous frame
                old_corners = good_recent_corners; // Update previous corners
            }
            
//            if (!vpFaces.empty()){
//                std::vector<Point2f> vfPoints = faceFeatureExtraction(img, vpFaces);
//                vPoints.insert(vPoints.end(), vfPoints.begin(), vfPoints.end());
//                CMarker::markPoints(mask, vPoints, Scalar(255, 0, 0));
//            }
            
            CMarker::markGUI(mask, fps);
            
            add(img, attenuation * mask, img);
            imshow("Camera", img);
		}
        
		int key = waitKey(5);
		if (key == 27 || key == 'q') break;
		if (key == 'a') attenuation *= 1.1f;
		if (key == 'z') attenuation *= 0.9f;
	}

	controller.stop();
	return 0;
}

// Face
std::vector<ptr_face_t> detectFaces(Mat frame) {
    if(!face_cascade.load("/Users/fatine/Documents/HCI/GitHub/visir-tracker-03/build/bin/Debug/face.xml") ) {
        printf("Error loading face cascade");
        exit(1);
    };
    
    Mat frame_gray;
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY); // Convert to grayscale
    equalizeHist(frame_gray, frame_gray); // Applying histogram equalization
    
    // Face Detection
    std::vector<Rect> faces; // Vector of rectangles where faces were detected
    face_cascade.detectMultiScale(frame_gray, faces); // Detects faces and stores them in faces vector
    
    std::vector<ptr_face_t> ptrFaces; // Vector of pointers to CFace objects
    for (int i = 0; i < faces.size(); i++) {
        std::shared_ptr<CFace> face (new CFace(faces[i], i, "No name"));
        ptrFaces.push_back(std::move(face));
    }
    
    return ptrFaces;
}

// Feature Extraction
std::vector<Point2f> featureExtraction(Mat frame) {
    Mat frame_gray;
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY); // Convert to grayscale
    std::vector<Point2f> corners;

    // Apply corner detection
    goodFeaturesToTrack(frame_gray, corners, 200, 0.01, 10,
                        Mat(), 3, false, 0.04);
    
    // Return features
    return corners;
}

// Face Feature Extraction
std::vector<Point2f> faceFeatureExtraction(Mat frame, std::vector<ptr_face_t> vpFaces) {
    std::vector<Point2f> corners;
    Mat frame_gray;
    cvtColor(frame, frame_gray, COLOR_BGR2GRAY); // Convert to grayscale
    
    for (auto face : vpFaces) {
        Mat roi = frame_gray(face->getArea());
        std::vector<Point2f> facecorners;
        
        // Apply corner detection
        goodFeaturesToTrack(roi, facecorners, 200, 0.01, 10,
                            Mat(), 3, false, 0.04);
        
        corners.insert(corners.end(), facecorners.begin(), facecorners.end());
    }
    
    // Return features
    return corners;
}

std::vector<Point2f> calcOpticalFlow(Mat old_gray, Mat recent_gray, std::vector<Point2f> old_corners) {
    std::vector<Point2f> recent_corners;
    std::vector<Point2f> good_recent_corners;
    
    // calcOpticalFlowPyrLK arguments
    std::vector<uchar> status;
    std::vector<float> err;
    TermCriteria criteria = TermCriteria((TermCriteria::COUNT) + (TermCriteria::EPS), 10, 0.03);

    // Calculate optical flow
    calcOpticalFlowPyrLK(old_gray, recent_gray, old_corners, recent_corners, status, err, Size(15,15), 2, criteria);

    for(int i = 0; i < old_corners.size(); i++) {
        // Select good corners
        if(status[i] == 1) {
            good_recent_corners.push_back(recent_corners[i]);
        }
    }
    
    // Return features
    return good_recent_corners;
}

	
