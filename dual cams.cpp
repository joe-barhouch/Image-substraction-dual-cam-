#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/videoio.hpp>

#include<iostream>
#include<conio.h>           

#include "Blob.h"

//voids
void matchCurrentFrameBlobsToExistingBlobsB(vector<BlobB>& existingBlobsB, vector<BlobB>& currentFrameBlobsB);
void matchCurrentFrameBlobsToExistingBlobsA(vector<BlobA>& existingBlobsA, vector<BlobA>& currentFrameBlobsA);
void drawtriggerOnImage(int& triggerOut, Mat& AimgFrame2Copy, Mat& BimgFrame2Copy);
void drawBlobInfoOnB(vector<BlobB>& blobsB, Mat& BimgFrame2Copy);
void drawBlobInfoOnA(vector<BlobA>& blobsA, Mat& AimgFrame2Copy);
bool checkIfBlobsCrossedLineB(vector<BlobB>& blobsB, int& LinePos, int& triggerOut);
bool checkIfBlobsCrossedLineA(vector<BlobA>& blobsA, int& LinePos, int& triggerOut);
double distanceBetweenPoints(Point point1, Point point2);
void addNewBlobB(BlobB& currentFrameBlobB, vector<BlobB>& existingBlobsB);
void addBlobToExistingBlobsB(BlobB& currentFrameBlobB, vector<BlobB>& existingBlobsB, int& intIndexB);
void addNewBlobA(BlobA& currentFrameBlobA, vector<BlobA>& existingBlobsA);
void addBlobToExistingBlobsA(BlobA& currentFrameBlobA, vector<BlobA>& existingBlobsA, int& intIndexA);


//colours
const Scalar BLACK	= Scalar(0.0, 0.0, 0.0);
const Scalar WHITE	= Scalar(255.0, 255.0, 255.0);
const Scalar YELLOW	= Scalar(0.0, 255.0, 255.0);
const Scalar GREEN	= Scalar(0.0, 200.0, 0.0);
const Scalar RED	= Scalar(0.0, 0.0, 255.0);



int main(void) {
	//multithreading initialization
//	setUseOptimized(true);
//	setNumThreads(4);

	VideoCapture capVideoA(0);
	VideoCapture capVideoB(1);
	Mat AimgFrame1;
	Mat AimgFrame2;
	Mat BimgFrame1;
	Mat BimgFrame2;


	//capturing 2 frames
	capVideoA.read(AimgFrame1);
	capVideoA.read(AimgFrame2);
	capVideoB.read(BimgFrame1);
	capVideoB.read(BimgFrame2);

	vector<BlobA> blobsA;		// create the blobs vector A
	vector<BlobB> blobsB;		// create the blobs vector B
	Point crossingLineA[2];		// first point of the array for img1, second point of the array for img2  ..... both points x and y ... on A
	Point crossingLineB[2];		// first point of the array for img1, second point of the array for img2  ..... both points x and y ... on B
	int triggerOut = 0;			// if the line is corssed from the top than triggerOut goes up

	//error messages
	if (!capVideoA.isOpened() ) {
		cout << "\ncannot find video A" << endl << endl;
		return(0);
	}

	if (!capVideoB.isOpened() ) {
		cout << "\ncannot find video B" << endl << endl;
		return(0);
	}


	//create the virtual line for crossing
	int LinePos = 300;  // in this case, horizontal line at pos 300 pixels from the top
	
	//initiate the crossing line positions
	crossingLineA[0].x = 0;
	crossingLineA[0].y = LinePos;

	crossingLineA[1].x = AimgFrame1.cols - 1;
	crossingLineA[1].y = LinePos;

	crossingLineB[0].x = 0;
	crossingLineB[0].y =  LinePos;

	crossingLineB[1].x = BimgFrame1.cols - 1;
	crossingLineB[1].y =  LinePos;

	//esc key check
	char CheckForEscKey = 0;

	//frame order config
	bool firstFrame = true;
	int frameCount = 2;



	//while loop
	while (capVideoA.isOpened() && capVideoB.isOpened() && CheckForEscKey != 27) {

		vector<BlobA> currentFrameBlobsA;
		vector<BlobB> currentFrameBlobsB;

		Mat AimgFrame1Copy = AimgFrame1.clone();
		Mat AimgFrame2Copy = AimgFrame2.clone();
		Mat BimgFrame1Copy = BimgFrame1.clone();
		Mat BimgFrame2Copy = BimgFrame2.clone();


		//constructing the black and white images with only the moving countours
		cvtColor(AimgFrame1Copy, AimgFrame1Copy, COLOR_BGR2GRAY);
		cvtColor(AimgFrame2Copy, AimgFrame2Copy, COLOR_BGR2GRAY);
		cvtColor(BimgFrame1Copy, BimgFrame1Copy, COLOR_BGR2GRAY);
		cvtColor(BimgFrame2Copy, BimgFrame2Copy, COLOR_BGR2GRAY);

		GaussianBlur(AimgFrame1Copy, AimgFrame1Copy, Size(5, 5), 0);
		GaussianBlur(AimgFrame2Copy, AimgFrame2Copy, Size(5, 5), 0);
		GaussianBlur(BimgFrame1Copy, BimgFrame1Copy, Size(5, 5), 0);
		GaussianBlur(BimgFrame2Copy, BimgFrame2Copy, Size(5, 5), 0);


		Mat imgDifferenceA;
		Mat imgThreshA;
		Mat imgDifferenceB;
		Mat imgThreshB;

		absdiff(AimgFrame1Copy, AimgFrame2Copy, imgDifferenceA);
		threshold(imgDifferenceA, imgThreshA, 30, 255.0, THRESH_BINARY);

		absdiff(BimgFrame1Copy, BimgFrame2Copy, imgDifferenceB);
		threshold(imgDifferenceB, imgThreshB, 30, 255.0, THRESH_BINARY);

		//start to fill in the countours by dilating every pixel inside the binary by 9, 7, 5, or 3 pixels to fill in a countour
		Mat structuringElement3 = getStructuringElement(MORPH_RECT, Size(3, 3));
		Mat structuringElement5 = getStructuringElement(MORPH_RECT, Size(5, 5));
		Mat structuringElement7 = getStructuringElement(MORPH_RECT, Size(7, 7));
		Mat structuringElement9 = getStructuringElement(MORPH_RECT, Size(9, 9));

		for (unsigned int i = 0; i < 5; i++) {
			dilate(imgThreshA, imgThreshA, structuringElement3);
			dilate(imgThreshA, imgThreshA, structuringElement3);
			erode(imgThreshA, imgThreshA, structuringElement3);

			dilate(imgThreshB, imgThreshB, structuringElement3);
			dilate(imgThreshB, imgThreshB, structuringElement3);
			erode(imgThreshB, imgThreshB, structuringElement3);
		}


		//find the countours of the moving parts and show it
		vector<vector<Point> > contoursA;
		vector<vector<Point> > contoursB;

		Mat imgThreshCopyA = imgThreshA.clone();
		Mat imgThreshCopyB = imgThreshB.clone();

		findContours(imgThreshCopyA, contoursA, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		findContours(imgThreshCopyB, contoursB, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		//create the convex hulls and show it
		vector<vector<Point> > convexHullsA(contoursA.size());

		for (unsigned int i = 0; i < contoursA.size(); i++) {
			convexHull(contoursA[i], convexHullsA[i]);
		}

		vector<vector<Point> > convexHullsB(contoursB.size());

		for (unsigned int i = 0; i < contoursB.size(); i++) {
			convexHull(contoursB[i], convexHullsB[i]);
		}


		// filtering unlikely blobs,
		// every blob is pushed back into the blob vectors
		for (auto& convexHull : convexHullsA) {
			BlobA possibleBlobA(convexHull);

			if (possibleBlobA.currentBoundingRectA.area() > 600 &&
				possibleBlobA.ACurrentAspectRatio > 0.3 &&
				possibleBlobA.ACurrentAspectRatio < 6.0 &&
				possibleBlobA.currentBoundingRectA.width > 60 &&
				possibleBlobA.currentBoundingRectA.height > 60 &&
				possibleBlobA.ACurrentDiagonalSize > 120.0 &&
				(contourArea(possibleBlobA.currentContourA) / (double)possibleBlobA.currentBoundingRectA.area()) > 0.50) {
				currentFrameBlobsA.push_back(possibleBlobA);
			}
		}

		for (auto& convexHull : convexHullsB) {
			BlobB possibleBlobB(convexHull);

			if (possibleBlobB.currentBoundingRectB.area() > 600 &&
				possibleBlobB.BCurrentAspectRatio > 0.3 &&
				possibleBlobB.BCurrentAspectRatio < 6.0 &&
				possibleBlobB.currentBoundingRectB.width > 60 &&
				possibleBlobB.currentBoundingRectB.height > 60 &&
				possibleBlobB.BCurrentDiagonalSize > 120.0 &&
				(contourArea(possibleBlobB.currentContourB) / (double)possibleBlobB.currentBoundingRectB.area()) > 0.50) {
				currentFrameBlobsB.push_back(possibleBlobB);
			}
		}



		//update the blob vectors 
		if (firstFrame == true) {
			for (auto& currentFrameBlobA : currentFrameBlobsA) {
				blobsA.push_back(currentFrameBlobA);
			}

			for (auto& currentFrameBlobB : currentFrameBlobsB) {
				blobsB.push_back(currentFrameBlobB);
			}
		}
		else {
			matchCurrentFrameBlobsToExistingBlobsB(blobsB, currentFrameBlobsB);
			matchCurrentFrameBlobsToExistingBlobsA(blobsA, currentFrameBlobsA);
		}


		AimgFrame2Copy = AimgFrame2.clone();          // get another copy of frame 2 since we changed the previous frame 2 copy in the processing above
		BimgFrame2Copy = BimgFrame2.clone();        

		//draw the bounding rectangles
		drawBlobInfoOnA(blobsA, AimgFrame2Copy);
		drawBlobInfoOnB(blobsB, BimgFrame2Copy);

		//check if the line is triggered. switch the line to yellow
		bool lineCrossedA = checkIfBlobsCrossedLineA(blobsA, LinePos, triggerOut);

		if (lineCrossedA == true) {
			line(AimgFrame2Copy, crossingLineA[0], crossingLineA[1], YELLOW, 2);
		}
		else {
			line(AimgFrame2Copy, crossingLineA[0], crossingLineA[1], RED, 2);
		}

		bool lineCrossedB =	checkIfBlobsCrossedLineB(blobsB, LinePos, triggerOut);
		
		if (lineCrossedB == true) {
			line(BimgFrame2Copy,  crossingLineB[0],  crossingLineB[1], YELLOW, 2);
		}
		else {
			line(BimgFrame2Copy,  crossingLineB[0],  crossingLineB[1], RED, 2);
		}


		// display the nbr of triggers
		cout << "---------------------------------------------------------" << endl;
		cout << "Triggers: " << triggerOut << endl;
		cout << "---------------------------------------------------------" << endl;

		drawtriggerOnImage(triggerOut, AimgFrame2Copy, BimgFrame2Copy);

		//show the vids
		namedWindow("IN video", WINDOW_AUTOSIZE);
		namedWindow("OUT video", WINDOW_AUTOSIZE);

		imshow("IN video", AimgFrame2Copy);
		imshow("OUT video", BimgFrame2Copy);

		// now we prepare for the next set of frames
		currentFrameBlobsA.clear();
		currentFrameBlobsB.clear();

		AimgFrame1 = AimgFrame2.clone();           // move frame 1 up to where frame 2 is
		BimgFrame1 = BimgFrame2.clone();           // move frame 1 up to where frame 2 is
		//waitKey(5);

		if ((capVideoA.get(CAP_PROP_POS_FRAMES) + 1) < capVideoA.get(CAP_PROP_FPS) && (capVideoB.get(CAP_PROP_POS_FRAMES) + 1) < capVideoB.get(CAP_PROP_FPS)) {
			capVideoA.read(AimgFrame2);
			capVideoB.read(BimgFrame2);
		}
		else {
			cout << "video FPS dropped to zero \n";
			break;
		}

		firstFrame = false;
		frameCount++;
		CheckForEscKey = waitKey(1);

	}

	if (CheckForEscKey != 27) {               // if the user did not press esc (i.e. we reached the end of the video)
		waitKey(0);                           // hold the windows open to allow the "end of video" message to show
	}
	
	return(0);

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void addBlobToExistingBlobsA(BlobA& currentFrameBlobA, vector<BlobA>& existingBlobsA, int& intIndexA) {

	existingBlobsA[intIndexA].currentContourA = currentFrameBlobA.currentContourA;
	existingBlobsA[intIndexA].currentBoundingRectA = currentFrameBlobA.currentBoundingRectA;

	existingBlobsA[intIndexA].centerPositionsA.push_back(currentFrameBlobA.centerPositionsA.back());

	existingBlobsA[intIndexA].ACurrentDiagonalSize = currentFrameBlobA.ACurrentDiagonalSize;
	existingBlobsA[intIndexA].ACurrentAspectRatio = currentFrameBlobA.ACurrentAspectRatio;

	existingBlobsA[intIndexA].AStillBeingTracked = true;
	existingBlobsA[intIndexA].ACurrentMatchFoundOrNewBlob = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void addNewBlobA(BlobA& currentFrameBlobA, vector<BlobA>& existingBlobsA) {

	currentFrameBlobA.ACurrentMatchFoundOrNewBlob = true;

	existingBlobsA.push_back(currentFrameBlobA);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void addBlobToExistingBlobsB(BlobB& currentFrameBlobB, vector<BlobB>& existingBlobsB, int& intIndexB) {

	existingBlobsB[intIndexB].currentContourB = currentFrameBlobB.currentContourB;
	existingBlobsB[intIndexB].currentBoundingRectB = currentFrameBlobB.currentBoundingRectB;

	existingBlobsB[intIndexB].centerPositionsB.push_back(currentFrameBlobB.centerPositionsB.back());

	existingBlobsB[intIndexB].BCurrentDiagonalSize = currentFrameBlobB.BCurrentDiagonalSize;
	existingBlobsB[intIndexB].BCurrentAspectRatio = currentFrameBlobB.BCurrentAspectRatio;

	existingBlobsB[intIndexB].BStillBeingTracked = true;
	existingBlobsB[intIndexB].BCurrentMatchFoundOrNewBlob = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void addNewBlobB(BlobB& currentFrameBlobB, vector<BlobB>& existingBlobsB) {

	currentFrameBlobB.BCurrentMatchFoundOrNewBlob = true;

	existingBlobsB.push_back(currentFrameBlobB);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
double distanceBetweenPoints(Point point1, Point point2) {

	int intX = abs(point1.x - point2.x);
	int intY = abs(point1.y - point2.y);
	//pyhtagorean theorem to find the shortest distance between predictions
	return(sqrt(pow(intX, 2) + pow(intY, 2)));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool checkIfBlobsCrossedLineA(vector<BlobA>& blobsA, int& LinePos, int& triggerOut) {
	bool lineCrossedA = false;

	for (auto blobA : blobsA) {

		if (blobA.AStillBeingTracked == true && blobA.centerPositionsA.size() >= 2) {
			int prevFrameIndexA = (int)blobA.centerPositionsA.size() - 2;
			int currFrameIndexA = (int)blobA.centerPositionsA.size() - 1;

			if (blobA.centerPositionsA[prevFrameIndexA].y < LinePos && blobA.centerPositionsA[currFrameIndexA].y >= LinePos) {
				triggerOut++;
				lineCrossedA = true;
			}
		}

	}

	return lineCrossedA;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
bool checkIfBlobsCrossedLineB(vector<BlobB>& blobsB, int& LinePos, int& triggerOut) {
	bool lineCrossedB = false;

	for (auto blobB : blobsB) {

		if (blobB.BStillBeingTracked == true && blobB.centerPositionsB.size() >= 2) {
			int prevFrameIndexB = (int)blobB.centerPositionsB.size() - 2;
			int currFrameIndexB = (int)blobB.centerPositionsB.size() - 1;

			if (blobB.centerPositionsB[prevFrameIndexB].y < LinePos && blobB.centerPositionsB[currFrameIndexB].y >= LinePos) {
				triggerOut--;
				if (triggerOut <= 0) {
					triggerOut = 0; 
				}
				lineCrossedB = true;
			}
		}

	}

	return lineCrossedB;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawBlobInfoOnA(vector<BlobA>& blobsA, Mat& AimgFrame2Copy) {

	for (unsigned int i = 0; i < blobsA.size(); i++) {

		if (blobsA[i].AStillBeingTracked == true) {
			rectangle(AimgFrame2Copy, blobsA[i].currentBoundingRectA, GREEN, 2);
	
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawBlobInfoOnB(vector<BlobB>& blobsB, Mat& BimgFrame2Copy) {

	for (unsigned int i = 0; i < blobsB.size(); i++) {

		if (blobsB[i].BStillBeingTracked == true) {
			rectangle(BimgFrame2Copy, blobsB[i].currentBoundingRectB, GREEN, 2);

		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawtriggerOnImage(int& triggerOut, Mat& AimgFrame2Copy, Mat& BimgFrame2Copy) {

	int intFontFace = FONT_HERSHEY_SIMPLEX;
	double dblFontScale = (AimgFrame2Copy.rows * AimgFrame2Copy.cols) / 300000.0;
	int intFontThickness = (int)round(dblFontScale * 1.5);

	Size textSizeOut = getTextSize(to_string(triggerOut), intFontFace, dblFontScale, intFontThickness, 0);
	Point textOut;

	textOut.x = AimgFrame2Copy.cols - 1 - (int)((double)textSizeOut.width * 1.25);
	textOut.y = (int)((double)textSizeOut.height * 1.25);
	putText(AimgFrame2Copy, to_string(triggerOut), textOut, intFontFace, dblFontScale, GREEN, intFontThickness);
	putText(BimgFrame2Copy, to_string(triggerOut), textOut, intFontFace, dblFontScale, GREEN, intFontThickness);


}

///////////////////////////////////////////////////////////////////////////////////////////////////
void matchCurrentFrameBlobsToExistingBlobsA(vector<BlobA>& existingBlobsA, vector<BlobA>& currentFrameBlobsA) {

	for (auto& existingBlobA : existingBlobsA) {

		existingBlobA.ACurrentMatchFoundOrNewBlob = false;

		existingBlobA.predictNextPositionA();
	}

	for (auto& currentFrameBlobA : currentFrameBlobsA) {

		int intIndexOfLeastDistance = 0;
		double dblLeastDistance = 100000.0;

		for (unsigned int i = 0; i < existingBlobsA.size(); i++) {

			if (existingBlobsA[i].AStillBeingTracked == true) {

				double dblDistance = distanceBetweenPoints(currentFrameBlobA.centerPositionsA.back(), existingBlobsA[i].predictedNextPositionA);

				if (dblDistance < dblLeastDistance) {
					dblLeastDistance = dblDistance;
					intIndexOfLeastDistance = i;
				}
			}
		}

		if (dblLeastDistance < currentFrameBlobA.ACurrentDiagonalSize * 0.5) {
			addBlobToExistingBlobsA(currentFrameBlobA, existingBlobsA, intIndexOfLeastDistance);
		}
		else {
			addNewBlobA(currentFrameBlobA, existingBlobsA);
		}

	}

	for (auto& existingBlobA : existingBlobsA) {

		if (existingBlobA.ACurrentMatchFoundOrNewBlob == false) {
			existingBlobA.ANumOfConsecutiveFramesWithoutAMatch++;
		}

		if (existingBlobA.ANumOfConsecutiveFramesWithoutAMatch >= 5) {
			existingBlobA.AStillBeingTracked = false;
		}

	}

}

///////////////////////////////////////////////////////////////////////////////////////////////////
void matchCurrentFrameBlobsToExistingBlobsB(vector<BlobB>& existingBlobsB, vector<BlobB>& currentFrameBlobsB) {

	for (auto& existingBlobB : existingBlobsB) {

		existingBlobB.BCurrentMatchFoundOrNewBlob = false;

		existingBlobB.predictNextPositionB();
	}

	for (auto& currentFrameBlobB : currentFrameBlobsB) {

		int intIndexOfLeastDistance = 0;
		double dblLeastDistance = 100000.0;

		for (unsigned int i = 0; i < existingBlobsB.size(); i++) {

			if (existingBlobsB[i].BStillBeingTracked == true) {

				double dblDistance = distanceBetweenPoints(currentFrameBlobB.centerPositionsB.back(), existingBlobsB[i].predictedNextPositionB);

				if (dblDistance < dblLeastDistance) {
					dblLeastDistance = dblDistance;
					intIndexOfLeastDistance = i;
				}
			}
		}

		if (dblLeastDistance < currentFrameBlobB.BCurrentDiagonalSize * 0.5) {
			addBlobToExistingBlobsB(currentFrameBlobB, existingBlobsB, intIndexOfLeastDistance);
		}
		else {
			addNewBlobB(currentFrameBlobB, existingBlobsB);
		}

	}

	for (auto& existingBlobB : existingBlobsB) {

		if (existingBlobB.BCurrentMatchFoundOrNewBlob == false) {
			existingBlobB.BNumOfConsecutiveFramesWithoutAMatch++;
		}

		if (existingBlobB.BNumOfConsecutiveFramesWithoutAMatch >= 5) {
			existingBlobB.BStillBeingTracked = false;
		}

	}

}
