// Blob.cpp 

#include "Blob.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
BlobA::BlobA(std::vector<cv::Point> contourA) {

	currentContourA = contourA;

	currentBoundingRectA = cv::boundingRect(currentContourA);

	cv::Point currentCenterA;

	currentCenterA.x = (currentBoundingRectA.x + currentBoundingRectA.x + currentBoundingRectA.width) / 2;
	currentCenterA.y = (currentBoundingRectA.y + currentBoundingRectA.y + currentBoundingRectA.height) / 2;

	centerPositionsA.push_back(currentCenterA);

	ACurrentDiagonalSize = sqrt(pow(currentBoundingRectA.width, 2) + pow(currentBoundingRectA.height, 2));

	ACurrentAspectRatio = (float)currentBoundingRectA.width / (float)currentBoundingRectA.height;

	AStillBeingTracked = true;
	ACurrentMatchFoundOrNewBlob = true;

	ANumOfConsecutiveFramesWithoutAMatch = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void BlobA::predictNextPositionA(void) {

	long numPositionsA = (long)centerPositionsA.size();

	if (numPositionsA == 1) {

		predictedNextPositionA.x = centerPositionsA.back().x;
		predictedNextPositionA.y = centerPositionsA.back().y;

	}
	else if (numPositionsA == 2) {

		int AdeltaX = centerPositionsA[1].x - centerPositionsA[0].x;
		int AdeltaY = centerPositionsA[1].y - centerPositionsA[0].y;

		predictedNextPositionA.x = centerPositionsA.back().x + AdeltaX;
		predictedNextPositionA.y = centerPositionsA.back().y + AdeltaY;

	}
	else if (numPositionsA == 3) {

		int AsumOfXChanges = ((centerPositionsA[2].x - centerPositionsA[1].x) * 2) +
			((centerPositionsA[1].x - centerPositionsA[0].x) * 1);

		int AdeltaX = (int)std::round((float)AsumOfXChanges / 3.0);

		int AsumOfYChanges = ((centerPositionsA[2].y - centerPositionsA[1].y) * 2) +
			((centerPositionsA[1].y - centerPositionsA[0].y) * 1);

		int AdeltaY = (int)std::round((float)AsumOfYChanges / 3.0);

		predictedNextPositionA.x = centerPositionsA.back().x + AdeltaX;
		predictedNextPositionA.y = centerPositionsA.back().y + AdeltaY;

	}
	else if (numPositionsA == 4) {

		int AsumOfXChanges = ((centerPositionsA[3].x - centerPositionsA[2].x) * 3) +
			((centerPositionsA[2].x - centerPositionsA[1].x) * 2) +
			((centerPositionsA[1].x - centerPositionsA[0].x) * 1);

		int AdeltaX = (int)std::round((float)AsumOfXChanges / 6.0);

		int AsumOfYChanges = ((centerPositionsA[3].y - centerPositionsA[2].y) * 3) +
			((centerPositionsA[2].y - centerPositionsA[1].y) * 2) +
			((centerPositionsA[1].y - centerPositionsA[0].y) * 1);

		int AdeltaY = (int)std::round((float)AsumOfYChanges / 6.0);

		predictedNextPositionA.x = centerPositionsA.back().x + AdeltaX;
		predictedNextPositionA.y = centerPositionsA.back().y + AdeltaY;

	}
	else if (numPositionsA >= 5) {

		int AsumOfXChanges = ((centerPositionsA[numPositionsA - 1].x - centerPositionsA[numPositionsA - 2].x) * 4) +
			((centerPositionsA[numPositionsA - 2].x - centerPositionsA[numPositionsA - 3].x) * 3) +
			((centerPositionsA[numPositionsA - 3].x - centerPositionsA[numPositionsA - 4].x) * 2) +
			((centerPositionsA[numPositionsA - 4].x - centerPositionsA[numPositionsA - 5].x) * 1);

		int AdeltaX = (int)std::round((float)AsumOfXChanges / 10.0);

		int AsumOfYChanges = ((centerPositionsA[numPositionsA - 1].y - centerPositionsA[numPositionsA - 2].y) * 4) +
			((centerPositionsA[numPositionsA - 2].y - centerPositionsA[numPositionsA - 3].y) * 3) +
			((centerPositionsA[numPositionsA - 3].y - centerPositionsA[numPositionsA - 4].y) * 2) +
			((centerPositionsA[numPositionsA - 4].y - centerPositionsA[numPositionsA - 5].y) * 1);

		int AdeltaY = (int)std::round((float)AsumOfYChanges / 10.0);

		predictedNextPositionA.x = centerPositionsA.back().x + AdeltaX;
		predictedNextPositionA.y = centerPositionsA.back().y + AdeltaY;

	}


}


// BlobBc.cpp c for counting

#include "Blob.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
BlobB::BlobB(std::vector<cv::Point> contourA) {

	currentContourB = contourA;

	currentBoundingRectB = cv::boundingRect(currentContourB);

	cv::Point currentCenterB;

	currentCenterB.x = (currentBoundingRectB.x + currentBoundingRectB.x + currentBoundingRectB.width) / 2;
	currentCenterB.y = (currentBoundingRectB.y + currentBoundingRectB.y + currentBoundingRectB.height) / 2;

	centerPositionsB.push_back(currentCenterB);

	BCurrentDiagonalSize = sqrt(pow(currentBoundingRectB.width, 2) + pow(currentBoundingRectB.height, 2));

	BCurrentAspectRatio = (float)currentBoundingRectB.width / (float)currentBoundingRectB.height;

	BStillBeingTracked = true;
	BCurrentMatchFoundOrNewBlob = true;

	BNumOfConsecutiveFramesWithoutAMatch = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void BlobB::predictNextPositionB(void) {

	long numPositionsB = (long)centerPositionsB.size();

	if (numPositionsB == 1) {

		predictedNextPositionB.x = centerPositionsB.back().x;
		predictedNextPositionB.y = centerPositionsB.back().y;

	}
	else if (numPositionsB == 2) {

		int BdeltaX = centerPositionsB[1].x - centerPositionsB[0].x;
		int BdeltaY = centerPositionsB[1].y - centerPositionsB[0].y;

		predictedNextPositionB.x = centerPositionsB.back().x + BdeltaX;
		predictedNextPositionB.y = centerPositionsB.back().y + BdeltaY;

	}
	else if (numPositionsB == 3) {

		int BsumOfXChanges = ((centerPositionsB[2].x - centerPositionsB[1].x) * 2) +
			((centerPositionsB[1].x - centerPositionsB[0].x) * 1);

		int BdeltaX = (int)std::round((float)BsumOfXChanges / 3.0);

		int BsumOfYChanges = ((centerPositionsB[2].y - centerPositionsB[1].y) * 2) +
			((centerPositionsB[1].y - centerPositionsB[0].y) * 1);

		int BdeltaY = (int)std::round((float)BsumOfYChanges / 3.0);

		predictedNextPositionB.x = centerPositionsB.back().x + BdeltaX;
		predictedNextPositionB.y = centerPositionsB.back().y + BdeltaY;

	}
	else if (numPositionsB == 4) {

		int BsumOfXChanges = ((centerPositionsB[3].x - centerPositionsB[2].x) * 3) +
			((centerPositionsB[2].x - centerPositionsB[1].x) * 2) +
			((centerPositionsB[1].x - centerPositionsB[0].x) * 1);

		int BdeltaX = (int)std::round((float)BsumOfXChanges / 6.0);

		int BsumOfYChanges = ((centerPositionsB[3].y - centerPositionsB[2].y) * 3) +
			((centerPositionsB[2].y - centerPositionsB[1].y) * 2) +
			((centerPositionsB[1].y - centerPositionsB[0].y) * 1);

		int BdeltaY = (int)std::round((float)BsumOfYChanges / 6.0);

		predictedNextPositionB.x = centerPositionsB.back().x + BdeltaX;
		predictedNextPositionB.y = centerPositionsB.back().y + BdeltaY;

	}
	else if (numPositionsB >= 5) {

		int BsumOfXChanges = ((centerPositionsB[numPositionsB - 1].x - centerPositionsB[numPositionsB - 2].x) * 4) +
			((centerPositionsB[numPositionsB - 2].x - centerPositionsB[numPositionsB - 3].x) * 3) +
			((centerPositionsB[numPositionsB - 3].x - centerPositionsB[numPositionsB - 4].x) * 2) +
			((centerPositionsB[numPositionsB - 4].x - centerPositionsB[numPositionsB - 5].x) * 1);

		int BdeltaX = (int)std::round((float)BsumOfXChanges / 10.0);

		int BsumOfYChanges = ((centerPositionsB[numPositionsB - 1].y - centerPositionsB[numPositionsB - 2].y) * 4) +
			((centerPositionsB[numPositionsB - 2].y - centerPositionsB[numPositionsB - 3].y) * 3) +
			((centerPositionsB[numPositionsB - 3].y - centerPositionsB[numPositionsB - 4].y) * 2) +
			((centerPositionsB[numPositionsB - 4].y - centerPositionsB[numPositionsB - 5].y) * 1);

		int BdeltaY = (int)std::round((float)BsumOfYChanges / 10.0);

		predictedNextPositionB.x = centerPositionsB.back().x + BdeltaX;
		predictedNextPositionB.y = centerPositionsB.back().y + BdeltaY;

	}


}
