// Blob.h 

#ifndef MY_BLOB
#define MY_BLOB

#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>


using namespace cv;
using namespace std;


class BlobA {
public:
	// member variables 
	vector<cv::Point> currentContourA;

	Rect currentBoundingRectA;

	vector<cv::Point> centerPositionsA;

	double ACurrentDiagonalSize;
	double ACurrentAspectRatio;

	bool ACurrentMatchFoundOrNewBlob;

	bool AStillBeingTracked;

	int ANumOfConsecutiveFramesWithoutAMatch;

	Point predictedNextPositionA;

	// function prototypes ////////////////////////////////////////////////////////////////////////
	BlobA(vector<Point> contourA);
	void predictNextPositionA(void);

};

class BlobB {
public:
	// member variables 
	vector<cv::Point> currentContourB;

	Rect currentBoundingRectB;

	vector<cv::Point> centerPositionsB;

	double BCurrentDiagonalSize;
	double BCurrentAspectRatio;

	bool BCurrentMatchFoundOrNewBlob;

	bool BStillBeingTracked;

	int BNumOfConsecutiveFramesWithoutAMatch;

	Point predictedNextPositionB;

	// function prototypes ////////////////////////////////////////////////////////////////////////
	BlobB(vector<Point> contourB);
	void predictNextPositionB(void);

};

#endif    // MY_BLOB
