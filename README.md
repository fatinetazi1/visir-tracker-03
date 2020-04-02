# Practical Assignment 3
Please put your name (or names if you work in a group) here:  
**Name**: Fatine Tazi
## Problem 3.1
### Ring Buffer (Points 30)
1. Fork the current repository
2. Study the new framework-code of 
    - main.cpp
    - Camera Controller.h/cpp
    - Face.h/cpp
    - Marker.h/cpp
3. Check that the code is running correctly: it should show the video stream from the web-camera of your laptop.
4. Implement the _ring buffer_ from lecture by modifying the code in Camera Controller.cpp file (places marked with MODIFY CODE HERE tag)
5. In cases when producer overwrites a frame, which was not querried by consumer, report a _drop frame_ state by printing the corresponding message to console. Test it with increasing the delay time in `waitKey()`function in the main loop of consumer. 

## Problem 3.2
### Face detection (Points 20)
Incorporate the face detection solution you done in the [Assignment 1](https://github.com/Jacobs-University/visir-tracker-01) into the framework in the following way:
1. Your face detector should return a vector of pointer to the `CFace` classes with detected faces: `std::vector<ptr_face_t> vpFaces` 
2. Implment function `CMarker::markFaces()` and use it for drawing the faces to GUI
3. Perform face detection every 10th frame.

## Problem 3.3
### Face tracking (Points 50)
Incorporate the optical flow field solution you done in the [Assignment 2](https://github.com/Jacobs-University/visir-tracker-02) into the framework in the following way:
1. Your sparse optical flow shoud return a vector of 2-d points: `std::vector<Point2f>`
2. Implment function `CMarker::markVecOFF()` and use it for drawing the optical flow field (feel free to mify its arguments if needed). 
3. Now detect the points only in the _face area_ , whic is described in `vpFaces` variable.
4. Update the detected points every 10th frame (when face detection is used).
5. Track also the points added with mouse in the mouse callback function.
6. Inbetween 10 frames (when the face detection is not applied) track the detected faces using the optical flow field. Update the pisition of faces in `vpFaces` variable variable for every frame.

## Problem 3.4
### Graphical User Interface (GUI) (Bonus Points 20)
1. Modify the function `CMarker::markGUI()` and design your own gui. Be creative!
2. Implement showing the current FPS in GUI
