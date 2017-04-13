#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "09";
	static const char MONTH[] = "09";
	static const char YEAR[] = "2015";
	static const char UBUNTU_VERSION_STYLE[] = "15.09";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 18;
	static const long BUILD = 0;
	static const long REVISION = 845;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 3794;
	#define RC_FILEVERSION 1,18,0,845
	#define RC_FILEVERSION_STRING "1, 18, 0, 845\0"
	static const char FULLVERSION_STRING[] = "1.18.0.845";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 65;
	

}
#endif //VERSION_H
