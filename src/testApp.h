#ifndef _TEST_APP
#define _TEST_APP

#include "ofxOpenNI.h"
#include "ofMain.h"
#include "circleRecog.h"
#include "ofxMaxim.h"

class testApp : public ofBaseApp{
    
public:
    
	void setup();
	void update();
	void draw();
    void exit();
    
	void keyPressed  (int key);
    
    float calcTempo(float timeForCalc);
    float tempoMatch();
    float calcSpeed(float A, float B);
    float crossFade(float fadeAmount);

    void  drawInterface(int opacity);
    
    void audioOut(float * output, int bufferSize, int nChannels);
    
    void userEvent(ofxOpenNIUserEvent & event);
    void handEvent(ofxOpenNIHandEvent & event);
    
    //SOUNDSTREAM SETUP
    ofSoundStream soundStream;
    int initialBufferSize;
    int	sampleRate;
	
    
    //MAXIM SOUND LIBRARY VARIABLES
    double track1, track2, tempTrack, headphoneOut[2], frontOut[2];
    ofxMaxiMix headphoneMix, frontMix;
    ofxMaxiSample tempSample;

    ofxMaxiFilter filter;
    ofxMaxiFFT fftTrack1, fftTrack2;
    ofxMaxiFFTOctaveAnalyzer oct;
    
    float volume;
    float cutoff;
    float resonance;

    vector<ofFile> playlistFiles;
    vector<ofxMaxiSample> playlist;
    
    //Kinect object
	ofxOpenNI openNIDevice;
    
    ofTrueTypeFont verdana;
    
    //Images for interface
    ofImage loadICON, titleICON, volICON, tempICON, eqICON, startICON, nextICON,lastICON, tempHELP, helpICON, onICON, offICON;

    //SYSTEM BOOLS
    bool loaded, shouldStart, testing, running, help;

    //INTERFACE BOOLS
    bool startnext;

    //BEAT MATCHING BOOLS
    bool match, measure, isBeat;
    float firstTempo, currentTempo, startTime, stopTime, soundTime, bpm, multiplier;
    int beatcount;

    //CROSS FADE VARIABLES
    int fadeCount;
    bool startFade, processSecondTrack;
    float fadeAmount1, fadeAmount2, startPos, tempPos;
    bool tempFadeTrack;
    bool fadeOutTrackTempo;

    //Relevant int variables
    int numUsers, numHands, i, fade, angle;

    //Relevant float variables
    float prev, y1;
    
    //MANUAL CONTROL VARIABLES
    int manVolCount, manTempoCount, manEQCount;
    bool manVol, manTempo, manEQ, inUse, controlsON;
    
    float controlX, volY, tempoY, eqY, controlRad;
    float scaleUI;

    //TRACK SELECTION VARIABLES
    int countA, countB = 0;
    bool boolNext, boolXFade, firstTrack, firstTrackMatch;
    float trackX, trackY, trackRad;
    int currenttrackNo;

    //SET UP GESTURE RECOGNITION
    circleRecog recogniser;

    //SKIP BACK VARIABLES
    bool skipback, reverse, revStart;
    float timeElapsedS,timeElapsedF;

    //FILE IO FOR TESTING
    string filepath;
    string tempX, tempY, tempZ;
    int filecount;
    
    //Buffers for reading/writing data to file for Matlab
    ofBuffer bufferIn, bufferOut;

    //Joints from skeleton and relative points in space
    ofxOpenNIJoint rightHand;
    ofPoint rightHandPt;
    ofxOpenNIJoint leftHand;
    ofPoint leftHandPt;
    ofxOpenNIJoint head;
    ofPoint headPt;
    ofxOpenNIJoint rightShoulder;
    ofPoint rightShoulderPt;


    
};

#endif
