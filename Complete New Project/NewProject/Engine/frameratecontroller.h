#ifndef FRAMERATECONTROLLER_H
#define FRAMERATECONTROLLER_H

#define MAX_FRAME_RATE 60

#include <windows.h>
// Singleton implementation of a framerate controller.
// There shouldn't ever be more than one of these so a singleton works fine.
// This object should cap the framerate at the defined max
class FramerateController {
public:
	static FramerateController* instance();
	static void destroy();

	void update();
	double getTime();
	void startElapsedTimer();
	double getElapsedTime();
	void endElapsedTimer();

	bool proceedToNextFrame();
	void logData();

private:
	FramerateController();
	~FramerateController();

	_int64 getLatestQuery();

	static FramerateController* mInstance;

	int mCurrentFrameRate; // Shows the current framerate, i.e. how many frames we're dealing with per second

	LARGE_INTEGER mLatestQuery;
	LARGE_INTEGER mFrequencyQuery;
	LARGE_INTEGER mStartingPoint;
	LARGE_INTEGER mElapsedPoint;
	LARGE_INTEGER mLastFrameTime; // The last time we processed a frame

	// Defined as int64's so we can deal with the large integers
	_int64 freqQuadPart;
	_int64 latestQuadPart;
	_int64 startingQuadPart;
	_int64 elapsedQuadPart;

	double mPrecision; // How precise our timer is(seconds, milliseconds, etc)
	double mFrequency; // The frequency of the processor
	
	_int64 mFrames; // Number of frames processed since application has started
	double mAverageFramerate; // The overall average framerate for the lifetime of the application
};

#endif
