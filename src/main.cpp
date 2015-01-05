
#include "testApp.h"
#include "ofMain.h"
#include "ofAppGlutWindow.h"

//========================================================================
int main( ){
    
    ofAppGlutWindow window;

    //Setup GL window
	ofSetupOpenGL(&window, 1400,900, OF_WINDOW);
	
	ofRunApp( new testApp());
    
}
