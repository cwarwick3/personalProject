#ifndef IDGENERATOR_H
#define IDGENERATOR_H

class IDGenerator {
public:
	IDGenerator();
	~IDGenerator();
	static unsigned int getID();
	static void returnID(unsigned int id);
private:
	static unsigned int mCurrentID;
	static unsigned int mNumIDsDistributed;
	static unsigned int mNumIDsReturned;
};

#endif
