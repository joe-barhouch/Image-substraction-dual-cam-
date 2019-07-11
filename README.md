# Image-substraction-dual-cam-

Main.cpp runs the main code
blob.h and blob.cpp are the additional headers

this code is to be compiled with OpenCV 4.1+ installed on the devce.
The code is able to detect and track moving objects using image substraction on two seperate cameras.

Cam A is set to be the IN camera, and the B cam is set to be the OUT camera.
When the virtual line on A is crossed, the trigger count will move one up, and when the line on B is crossed, the count will move one down.
