#ifndef _REV_SETUP_DATA_H
#define _REV_SETUP_DATA_H

struct RevSetupData
{

	//The basic engine setup data, gets send to engine when it's being created
	RevSetupData(	const char* aWindowTitle, 
					const unsigned int& aWidth,
					const unsigned int& aHeight,
					const bool& aIsFullScreen )
	:myWindowTitle( aWindowTitle), 
	myWidth( aWidth ),
	myHeight( aHeight ),
	myIsWindowed( aIsFullScreen )
	{ }

	unsigned int myWidth;
	unsigned int myHeight;
	const char* myWindowTitle;
	bool	myIsWindowed;
};

#endif