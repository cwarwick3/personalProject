#include "frameratecontroller.h"
#include <iostream>

FramerateController* FramerateController::mInstance = 0;

FramerateController* FramerateController::instance() {
	if (mInstance == 0) {
		mInstance = new FramerateController();
	}
	return mInstance;
}

void FramerateController::destroy() {
	delete(mInstance);
	mInstance = 0;
}

FramerateController::FramerateController() {
	mCurrentFrameRate = 0;
	mPrecision = 1.0;
	mFrequency = 0.0;
	mFrames = 0;
	mAverageFramerate = 0.0;

	// Mark our starting point
	QueryPerformanceFrequency(&mFrequencyQuery);
	QueryPerformanceCounter(&mStartingPoint);
	QueryPerformanceCounter(&mLastFrameTime);
	
	freqQuadPart = mFrequencyQuery.QuadPart;
	mFrequency = double(freqQuadPart) / mPrecision;

	startingQuadPart = mStartingPoint.QuadPart;
}

FramerateController::~FramerateController() {

}

// Query the latest performance counter and upate our time stamp
void FramerateController::update() {
}

double FramerateController::getTime() {
	double currentTime = double(getLatestQuery() - startingQuadPart) / mFrequency;
	return currentTime;
}

void FramerateController::startElapsedTimer() {
	QueryPerformanceCounter(&mElapsedPoint);
	elapsedQuadPart = mElapsedPoint.QuadPart;
}

double FramerateController::getElapsedTime() {
	double currentTime = double(getLatestQuery() - elapsedQuadPart) / mFrequency;
	return currentTime;
}

void FramerateController::endElapsedTimer() {
}

// If enough time has passed, give the okay to proceed to the next
// frame loop
bool FramerateController::proceedToNextFrame() {
	mFrames++;
	// Avg Framerate = Number of total frames / life of the application
	mAverageFramerate = mFrames / (getTime() / mPrecision);
	_int64 lastFrameQuad = mLastFrameTime.QuadPart;
	double currentTime = double(getLatestQuery() - lastFrameQuad) / mFrequency;
	QueryPerformanceCounter(&mLastFrameTime);
	if ((currentTime/mPrecision) > (1.0/MAX_FRAME_RATE)) {
		mCurrentFrameRate = (int)(1.0 / (currentTime/mPrecision));
		return true;
	} else {
		// We want to lock our frame rate at the max frame rate so sleep for however long we need to
		// TODO: DON'T WASTE THIS TIME
		DWORD millisecondsToSleep = ((1.0/MAX_FRAME_RATE) - (currentTime/mPrecision)) * 1000;
		Sleep(millisecondsToSleep);
		mCurrentFrameRate = MAX_FRAME_RATE;
	}
	return true;
}

_int64 FramerateController::getLatestQuery() {
	QueryPerformanceCounter(&mLatestQuery);	
	latestQuadPart = mLatestQuery.QuadPart;

	return latestQuadPart;
}

void FramerateController::logData() {
	std::cout << "Current time: " << getTime() << std::endl;
	std::cout << "Current framerate: " << mCurrentFrameRate << std::endl;
	std::cout << "Average framerate: " << mAverageFramerate << std::endl;
	std::cout << "Total Frames: " << mFrames << std::endl;
}
