#include "IDGenerator.h"

unsigned int IDGenerator::mCurrentID = 0;
unsigned int IDGenerator::mNumIDsDistributed = 0;
unsigned int IDGenerator::mNumIDsReturned = 0;

IDGenerator::IDGenerator() {
}

IDGenerator::~IDGenerator() {
}

unsigned int IDGenerator::getID() {
	mNumIDsDistributed++;
	return ++mCurrentID;
}

void IDGenerator::returnID(unsigned int id) {
	mNumIDsReturned++;
}
