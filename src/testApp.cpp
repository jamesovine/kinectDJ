#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
   
    //SET BACKGROUND COLOUR 
    ofBackground(15);
    
    //LOAD ICONS
    titleICON.loadImage("UI/TITLE.png");
    loadICON.loadImage("UI/LOAD.png");
    volICON.loadImage("UI/VOLUME.png");
    tempICON.loadImage("UI/SPEED.png");
    eqICON.loadImage("UI/EQ.png");
    startICON.loadImage("UI/START.png");
    nextICON.loadImage("UI/NEXT.png");
    lastICON.loadImage("UI/LAST.png");
    tempHELP.loadImage("UI/TEMPHELP.png");
    helpICON.loadImage("UI/HELP.png");
    onICON.loadImage("UI/ON.png");
    offICON.loadImage("UI/OFF.png");
    
    //BEAT MATCHING
    fade, beatcount = 0;
    match, measure,isBeat = 0;

    //MAXIMILIAN FFT
    //Initialise FFT with: fft size, window size, hop size
    fftTrack1.setup(1024,512,256);
    fftTrack2.setup(1024,512,256);
    oct.setup(44100,512,12);

    //GESTURE RECOG
    skipback, reverse = 0;
    revStart = 1;

    //TIME FOR REVERSE PLAY
    timeElapsedS, timeElapsedF = 0;
    
    //INITIALISE MANUAL CONTROL VARIABLES
    //INTS
    manVolCount, manTempoCount, manEQCount = 0;
    //BOOLS
    manVol, manTempo, manEQ , inUse = 0;
    controlsON = 1;

    //TRACK SELECTION VARIABLES
    countA, countB = 0;
    boolNext, boolXFade = 0;
    currenttrackNo = 0;
    firstTrack = 1;
    firstTrackMatch = 0;

    //INTERFACE
    startnext=0;
    scaleUI=2.25;

    //COORDINATES
    controlX = 140;
    volY = 225;
    tempoY = 450;
    eqY = 675;
    controlRad = 100;
    trackX = 1230;
    trackY = 450;
    trackRad = 125;
    
    //CROSS FADE VARIABLES
    fadeCount = 0;
    fadeAmount1 = 1;
    fadeAmount2 = 0;
    processSecondTrack = 0;
    startPos, tempPos = 0;
    tempFadeTrack = 0;
    fadeOutTrackTempo=0;
    
    //SYSTEM BOOLS
    running, shouldStart, help=0;

    //AUDIO FLOATS
    fade, firstTempo, currentTempo, bpm, prev = 0;
    multiplier = 1;
    volume = 1;
    cutoff = 1;

    //TO GATHER DATA FOR TRAINING
    testing = 0;
    filecount = 0;
    
    ofSetLogLevel(OF_LOG_VERBOSE);

    //Set up Kinect
    openNIDevice.setup();
    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
    openNIDevice.addUserGenerator();

    //ONLY ALLOW ONE USER AT A TIME
    openNIDevice.setMaxNumUsers(1);
    
    //LOAD FONT FOR PRINTING TO SCREEN   
    verdana.loadFont(ofToDataPath("verdana.ttf"), 24);
    
    //SET UP THE SOUNDSTREAM
    
    sampleRate = 44100;
	initialBufferSize = 512;
    ofSoundStreamSetup(2,0,this, sampleRate, initialBufferSize, 4);
    
    
}

//--------------------------------------------------------------
void testApp::update(){

    if (shouldStart)
    {
        openNIDevice.start();
        shouldStart=0;
    }

    if (running)
    {
         openNIDevice.update();

        //GET NUMBER OF USERS
        numUsers = openNIDevice.getNumTrackedUsers();
        
        // IF THERE IS A USE
        if (numUsers!=0)
        {
            //GET THE USER
            ofxOpenNIUser & user = openNIDevice.getTrackedUser(0);
            
            //GET NECESSARY SKELETON DATA
            rightHand = user.getJoint(JOINT_RIGHT_HAND);
            rightHandPt = rightHand.getProjectivePosition();
            leftHand = user.getJoint(JOINT_LEFT_HAND);
            leftHandPt = leftHand.getProjectivePosition();
            rightShoulder = user.getJoint(JOINT_RIGHT_SHOULDER);
            rightShoulderPt = rightShoulder.getProjectivePosition();
            head=user.getJoint(JOINT_HEAD);
            headPt=head.getProjectivePosition();
            
            //IF WE ARE TRACKING THE USER
            if (user.isTracking()) 
            {        
                //IF RIGHT HAND PASSES THROUGH SUITABLE Z THRESHOLD
                if (rightHandPt.z<1600)
                {
                    //Check that we are not in test mode
                    if (!testing) {

                        /*************************************** //TRACK SELECTION// **************************************/
                        //IF WE HAVE HOVER OVER 'NEXT' BUTTON
                        if (boolNext)
                        {

                            //NOTHING IS PLAYING
                            if (firstTrack)
                            {
                                //PLAY THE FIRST TRACK
                                cout << "PLAYING!" << endl;

                                //CHANGE "START" ICON TO "NEXT"
                                startnext=1;

                                boolNext=0;
                                firstTrack=0;
                                firstTrackMatch=1;
                             }    

                            //SOMETHING IS PLAYING ALREADY
                            else 
                            {
                                //GO TO CROSS FADE
                                boolNext=0;
                                boolXFade=1;
                            }   
                        }

                        /*************************************** //CROSS FADE// **************************************/

                        //WHEN 'NEXT' BUTTON IS HOVERED OVER
                        //AUDIO MANPULATION NOW HAPPENS IN audioOut()

                        if (boolXFade)
                        {
                            if (fadeAmount1==1)
                            {
                                //SET POSTION OF NEXT TRACK
                                playlist[currenttrackNo+1].setPosition(startPos);

                                fadeOutTrackTempo = multiplier;

                                //COMPUTE NEXT TEMPO
                                multiplier=calcSpeed(firstTempo,currentTempo);
                            }

                            //ADJUST THE FADE VARIABLES
                            fadeAmount1-=0.005;
                            fadeAmount2+=0.005;

                            cout << "fade2 is: " << fadeAmount2 << endl;

                            if (fadeAmount2>1)
                            {
                                //FADE IS COMPLETE
                                boolXFade=0;

                                //RESET FADE AMOUNTS
                                fadeAmount1=1;
                                fadeAmount2=0;

                                //RESET START POSITION AND TEMPO VARIABLES
                                startPos=0;
                                currentTempo=0;

                                //INCREMENT TRACK COUNTER
                                currenttrackNo++;

                            }
                        }


                        /*************************************** //TEMPO MATCHING// **************************************/

                        if (currenttrackNo<playlist.size()-1)
                        {
                            //MATCH FIRST 2 TRACKS
                            if (firstTrackMatch==1)
                            {
                                // IF FIRST TRACK HASNT BEEN MATCHED YET
                                if (firstTempo==0)
                                {
                                    controlsON=0;
                                    firstTempo=tempoMatch();
                                    if (firstTempo != 0)
                                    {
                                        firstTempo=roundf(firstTempo);
                                    }
                                }

                                //TEMPO MATCH NEXT TRACK
                                if (firstTempo!=0)
                                {
                                    currentTempo=tempoMatch();
                                    if (currentTempo != 0)
                                    {
                                        currentTempo=roundf(currentTempo);
                                        
                                        firstTrackMatch=0;
                                        controlsON=1;
                                    }
                                }
                            }
                            
                            else if (firstTempo!=0)
                            {
                                //TRIGGERED BY THE END OF CROSS FADE STAGE
                                if (currentTempo==0)
                                {
                                    controlsON=0;
                                    currentTempo=tempoMatch();
                                    if (currentTempo != 0)
                                    {
                                        currentTempo=roundf(currentTempo);

                                        controlsON=1;
                                    }
                                }

                            }
                        }

                        /*************************************** //SKIP BACK GESTURE RECOGNITION// **************************************/

                        //ONLY WANT TO SEARCH FOR CIRCLE GESTURES WHEN MUSIC IS PLAYING AND WHEN NOT TEMPO MATCHING
                        if (startnext && controlsON)
                        {
                            //MAKE SURE WE DONT RECORD GESTURE WHEN IN CONTROLS
                            if (!manTempo && !manVol && !manEQ)
                            {
                                //FEED CURRENT POINT TO circleRecog CLASS.
                                skipback = recogniser.isCircle(rightHandPt);
                                
                                //IF GESTURE, SKIP BACK TO START OF CURRENT TRACK (DONE IN AUDIO OUT FN)
                                if (skipback) cout << "CIRCLE FOUND!!!" << endl;
                            }

                        }


                        /*************************************** //MANUAL CONTROLS// **************************************/

                        // IF HAND HOVERS OVER RELEVANT ICON ON SIDE EG VOLUME
                        if (controlsON)
                        {
                            ofPushMatrix();
                            
                            ofScale(scaleUI, scaleUI);

                            //VOLUME
                            //IF WE ARE IN THE VOL ICON
                            if (pow(leftHandPt.x-(controlX/scaleUI),2) + pow(leftHandPt.y-(volY/scaleUI),2) < pow(controlRad/scaleUI,2))
                            {
                                manVolCount++;

                                if (manVolCount>50)
                                    {
                                        manVol=!manVol;
                                        manVolCount=0;
                                    }
                            }

                            //RESET SELECTION COUNTER WHEN HAND IS OUT OF ZONE
                            else if (manVolCount>0) manVolCount=0;

                            if (manVol)
                            {
                                //Distance between hands as volume
                                float scale =ofGetWidth();
                                float x1= (float)rightHandPt.x;
                                float x2= (float)leftHandPt.x;
                                x1/=scale;
                                x2/=scale;
                                volume = (x1-x2)*(x1-x2);
                                volume *= 4;
                            
                            }

                            //TEMPO
                            if (pow(leftHandPt.x-(controlX/scaleUI),2) + pow(leftHandPt.y-(tempoY/scaleUI),2) < pow(controlRad/scaleUI,2))
                            {
                                manTempoCount++;
                                if (manTempoCount>50)
                                    {
                                        manTempo=!manTempo;
                                        manTempoCount=0;
                                    }
                            }
                            
                            //RESET SELECTION COUNTER WHEN HAND IS OUT OF ZONE
                            else if (manTempoCount>0) manTempoCount=0;

                            if (manTempo)
                            {
                                y1=(((float)ofGetHeight()-rightHandPt.y) / (float)ofGetHeight());
                                y1*=y1;
                            }

                            //EQ
                            if (pow(leftHandPt.x-(controlX/scaleUI),2) + pow(leftHandPt.y-(eqY/scaleUI),2) < pow(controlRad/scaleUI,2))
                            {
                                manEQCount++;
                                if (manEQCount>50)
                                    {
                                        manEQ=!manEQ;
                                        manEQCount=0;
                                    }
                            }
                            
                            //RESET SELECTION COUNTER WHEN HAND IS OUT OF ZONE
                            else if (manEQCount>0) manEQCount=0;

                            if (manEQ)
                            {
                                //COMPUTE CUTOFF FOR HI PASS FILTER
                                cutoff=(((float)ofGetHeight()-rightHandPt.y) / (float)ofGetHeight());
                                cutoff*=cutoff;
                                
                                //CONVERT INTO SUITABLE HZ FOR FREQ CUTOFF
                                cutoff*=3000;

                                //DISTANCE BETWEEN HANDS AS RESOLUTION
                                float scaleX =ofGetWidth();
                                float scaleY =ofGetHeight();
                                float x1= (float)rightHandPt.x;
                                float x2= (float)leftHandPt.x;
                                float y1= (float)rightHandPt.y;
                                float y2= (float)leftHandPt.y;

                                x1/=scaleX;
                                x2/=scaleX;

                                y1/=scaleY;
                                y2/=scaleY;

                                resonance = sqrt(pow((x1-x2),2) + pow((x1-x2),2));
                                cout << "RES: " << resonance;
                                resonance *= 100;
                            }

                            if (currenttrackNo<playlist.size()-1)
                            {

                                //START/NEXT BUTTON
                                if (pow(rightHandPt.x-(trackX/scaleUI),2) + pow(rightHandPt.y-(trackY/scaleUI),2) < pow(trackRad/scaleUI,2))
                                {
                                    countA++;
                                    if (countA>50)
                                    {
                                        boolNext=1;
                                        countA=0;
                                    }
                                }
                                
                                //RESET SELECTION COUNTER WHEN HAND IS OUT OF ZONE
                                else if (countA>0) countA=0;

                            }

                            ofPopMatrix();
                        }

                    }
                    
                    //Test mode: read in point(x,y,z) data for matlab training
                    if (testing) {
                        i++;
                        if (i==1)
                        {
                            tempX = ofToString(rightHandPt.x) + "\n";
                            tempY = ofToString(rightHandPt.y) + "\n";
                            tempZ = ofToString(rightHandPt.z) + "\n";
                        }

                        if (i>1) 
                        {
                            tempX = tempX + ofToString(rightHandPt.x) + "\n";
                            tempY = tempY + ofToString(rightHandPt.y) + "\n";
                            tempZ = tempZ + ofToString(rightHandPt.z) + "\n";
                        }
                    }
                }
                
                //WHEN HANDS MOVE FAR ENOUGH BACK TURN ANY MANUAL CONTROLS OFF
                else
                {
                    manVol=0;
                    manEQ=0;

                    //RESET TEMPO UNLESS FINAL SONG OR REWIND GESTURE
                    manTempo=0;


                    //BUFFER DATA AND WRITE TO FILE
                    if (testing && i>0)
                    {
                        if (tempX.size()>0)
                        {
                            filecount++;
                            ofBuffer Xfile (tempX.c_str(), tempX.length());
                            ofBuffer Yfile (tempY.c_str(), tempY.length());
                            ofBuffer Zfile (tempZ.c_str(), tempZ.length());
                            stringstream sX, sY, sZ;
                            sX << "dataX" << filecount << ".dat";
                            sY << "dataY" << filecount << ".dat";
                            sZ << "dataZ" << filecount << ".dat";
                            ofBufferToFile(sX.str(), Xfile);
                            ofBufferToFile(sY.str(), Yfile);
                            ofBufferToFile(sZ.str(), Zfile);
                            tempX.clear();
                            tempY.clear();
                            tempZ.clear();   
                        }
                        

                    }

                }
            }
        }
    } 
}

//--------------------------------------------------------------
void testApp::draw(){
    
    //INITIAL LOAD SCREEN
    if (!loaded && !running) {
        ofSetColor(255, 255, 255, fade);
        titleICON.draw(ofGetWidth()/2-titleICON.getWidth()/2,ofGetHeight()/2-titleICON.getHeight()/2 - 150);
        loadICON.draw(ofGetWidth()/2-loadICON.getWidth()/2,ofGetHeight()/2-loadICON.getHeight()/2);
        helpICON.draw(ofGetWidth()/2-helpICON.getWidth()/2,ofGetHeight()/2-helpICON.getHeight()/2+200);

        if (help==1) onICON.draw(ofGetWidth()/2+helpICON.getWidth()/2 + onICON.getWidth()/2,ofGetHeight()/2-helpICON.getHeight()/2+200);
        else if (help==0) offICON.draw(ofGetWidth()/2+helpICON.getWidth()/2 + offICON.getWidth()/2,ofGetHeight()/2-helpICON.getHeight()/2+200);

        if (fade != 255) fade+=5;
    }

    //FADE INTERFACE IN
    else if (loaded)
    {
        ofSetColor(255, 255, 255, fade);
        titleICON.draw(ofGetWidth()/2-titleICON.getWidth()/2,ofGetHeight()/2-titleICON.getHeight()/2 - 150);
        loadICON.draw(ofGetWidth()/2-loadICON.getWidth()/2,ofGetHeight()/2-loadICON.getHeight()/2);
        
        if (fade > 0) fade-=5;

        drawInterface(255-fade);

        if (fade==0) 
        {
            running=1;
            shouldStart=1;
            loaded=0;
        }
    }

    //MAIN PROGRAM CYCLE
    if (running)
    {
        if (controlsON) drawInterface(255);
        

        //DRAW FFT DATA. 
        ofNoFill();
        ofSetColor(255,255,0,255);

        float width = 20;

        for (int i = 0; i < sizeof(fftTrack1.magnitudes); i++)
        {
            //DRAW EQ CENTRE BOTTOM. *5 just for scale
            ofRect(ofGetWidth()/2+(i-8)*width,ofGetHeight(),width,-(fftTrack1.magnitudes[i]*5));
        }

        ofFill();

        ofPushMatrix();
        
        openNIDevice.drawImage(ofGetWidth()-320, ofGetHeight()-240, 320, 240);
        
        ofPopMatrix();

        ofPushMatrix();

        ofScale(scaleUI, scaleUI);

        //FOR ALL USERS (AT THE MOMENT MAX IS SET TO ONE)
        for (int i = 0; i < numUsers; i++){
            
            // DRAW USER SKELETON
            ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);
            openNIDevice.drawSkeleton(0);
            
            //user.drawMask();

            ofSetColor(0,255,0);
            ofEllipse(headPt.x,headPt.y,30,30);
            
            //SUITABLE DEPTH THRESHOLD FOUND THROUGH TESTING
            if (rightHandPt.z<1600) 
            {
                //DRAW A RED CIRCLE ON RIGHT HAND
                ofSetColor(255, 0, 0);
                ofEllipse(rightHandPt.x, rightHandPt.y, 30, 30);
                //DRWA A BLUE CIRCLE ON LEFT HAND
                ofSetColor(0, 0, 255);
                ofEllipse(leftHandPt.x, leftHandPt.y, 30, 30);
            }
            
        }

        ofPopMatrix();
    }

}


//CALCULATE BPM FOR NEXT TRACK
float testApp::calcTempo(float timeforCalc)
{

    cout << "IN TEMPO FN" << endl;

        //(60/timeforCalc) * 8
        bpm=480/timeforCalc;
        return bpm;
}

//SPEED MULTIPLIER
float testApp::calcSpeed(float A, float B)
{
    float speed = A/B;
    return speed;
}

//CALCULATE TEMPO FROM USER MOVEMENT
float testApp::tempoMatch()
{

    if (rightHandPt.y<headPt.y)
    {
        match=1;
        measure=1;
        cout<<"Above Head" << endl;
    }

    if (match==1)
    {
        if (rightHandPt.y>rightShoulderPt.y && measure == 1)
        {
            float tempo=0;
            measure=0;
            if (beatcount==0)
            {
                startTime=ofGetElapsedTimef();  
            }

            beatcount++;
            
            if (beatcount==9)
            {
                stopTime=ofGetElapsedTimef();
                stopTime-=startTime;

                tempo = calcTempo(stopTime);
                beatcount=0;
                measure=0;
                match=0;
                return tempo;
            }
            
        }
        if (rightHandPt.y<rightShoulderPt.y)
        {
            measure=1;
        }
    }
    return 0;

}

//HANDLE ALL AUDIO OUTPUT TO SOUNDCARD
void testApp::audioOut(float * output, int bufferSize, int nChannels)
{
    
    //PLAY THE FIRST TWO TRACKS
    if (startnext)
    {
        for (int i = 0; i < bufferSize; i++)
        {

            //IF CIRCLE GESTURE FOUND. SKIP BACK TO BEGINNING OF TRACK
            if (skipback) reverse = 1;

            // NEED ANOTHER BOOL SINCE AUDIO OUT CALLED INDEP. OF MAIN RUN LOOP
            if (reverse)
            {

                //GET TIME
                if (revStart)
                {
                    timeElapsedS=ofGetElapsedTimef();
                    revStart = 0;
                }

                timeElapsedF = ofGetElapsedTimef();
                timeElapsedF -= timeElapsedS;
                
                //COMPUTE DECAYING SPEED
                float revSpeed = -20 + timeElapsedF*20;

                //SET SPEED
                track1=playlist[currenttrackNo].play(revSpeed);
                
                if (revSpeed>0)
                {
                    //SET POSTITION TO BEGINNING
                    playlist[currenttrackNo].trigger();

                    //RESET TRACK 1
                    track1=playlist[currenttrackNo].play(multiplier);
                    revStart = 1;
                    reverse = 0;
                }
            }
            
            //IF MANUAL TEMPO ARMED
            else if (manTempo) track1=playlist[currenttrackNo].play(y1);

            //IF WE ARE FADING
            else if (boolXFade)
            {
                track1 = playlist[currenttrackNo].play(fadeOutTrackTempo);
                tempTrack = playlist[currenttrackNo+1].play(multiplier);
            }

            //PLAY NORMALLY
            else  
            {
                //LIMIT THE SPEED MULTIPLIER TO ENSURE WE DON'T GET UNPLEASANT RESULTS
                if (multiplier>2) track1=playlist[currenttrackNo].play(2);
                
                //PLAY CURRENT TRACK AT DEFINED TEMPO
                else track1=playlist[currenttrackNo].play(multiplier);
                
            }

            //ONSET DETECTION TRACK 1 (SPEAKER TRACK) FFT
            if (fftTrack1.process(track1))
            {
                float sum = 0;

                for (int i = 0; i < sizeof(fftTrack1.magnitudes); i++)
                {
                    sum = sum + fftTrack1.magnitudes[i];
                }

                //GET AVERAGE INTENSITY
                sum /= sizeof(fftTrack1.magnitudes);

                //IF ABOVE AVERGE INTENSITY IN LOWEST SUBBAND --> KICK DRUM = BEAT
                if (fftTrack1.magnitudes[0]>sum)
                {
                    //TRIGGER CROSS FADE ON BEAT
                    startFade = 1;
                }
            }


            //ONSET DETECTION TRACK 2 (HEADPHONE TRACK) FFT
            if (processSecondTrack)
            {
                if (fftTrack2.process(track2))
                {
                    float sum = 0;

                    for (int i = 0; i < sizeof(fftTrack1.magnitudes); i++)
                    {
                        sum = sum + fftTrack1.magnitudes[i];
                    }

                    sum /= sizeof(fftTrack1.magnitudes);

                    if (fftTrack1.magnitudes[0]>sum)
                    {
                        //GET POSTITION OF FIRST BEAT
                        startPos = playlist[currenttrackNo].position;
                        processSecondTrack = 0;
                    }
                }    
            }

            //MANUAL EQ
            if (manEQ)  track1=filter.hires(track1, cutoff, resonance);

            //CROSS FADE
            if (boolXFade)
            {
                frontMix.stereo(track1*fadeAmount1 + tempTrack*fadeAmount2, frontOut, 0.5);
            }

            //GET STEREO MIX FOR SPEAKERS
            else frontMix.stereo(track1, frontOut, 0.5);

            if (currenttrackNo<playlist.size()-1)
            {
                //PLAY NEXT TRACK
                if (!boolXFade) track2=playlist[currenttrackNo+1].play();

                headphoneMix.stereo(track2, headphoneOut, 0.5);

                if (currentTempo == 0 && firstTempo != 0)
                {
                    //SEND NEXT TRACK MIX TO HEADPHONES FOR TEMP MATCH
                    output[i*nChannels + 0] = headphoneOut[0];
                    output[i*nChannels + 1] = headphoneOut[1];
                }
                
                else
                {
                    //SEND FRONT TRACK TO HEADPHONES
                    output[i*nChannels + 0] = frontOut[0]*volume;
                    output[i*nChannels + 1] = frontOut[1]*volume;
                }
            }

            //SEND MIX TO SPEAKERS
            output[i*nChannels + 1] = frontOut[0]*volume;
            output[i*nChannels + 1] = frontOut[1]*volume;
           
        }
    }
}

//DRAW MAIN PROGRAMME INTERFACE
void  testApp::drawInterface(int opacity)
{
        ofSetColor(230,220,193,opacity);
        ofSetCircleResolution(100);

        //MANUAL CONTROLS OUTLINES
        //Volume
        ofCircle(controlX, volY, controlRad+2);
        //Speed
        ofCircle(controlX, tempoY, controlRad+2);
        //EQ
        ofCircle(controlX, eqY, controlRad+2);
        
        //TRACK SELECTION
        //Start/Next
        ofCircle(trackX, trackY, trackRad+2);
        
        ofSetColor(50,50,50,opacity);
        //MANUAL CONTROLS
        //Volume
        if (manVol) ofSetColor(255,0,0,opacity);
        else ofSetColor(50,50,50,opacity);
        ofCircle(controlX, volY, controlRad);
       
        //Speed
        if (manTempo) ofSetColor(255,0,0,opacity);
        else ofSetColor(50,50,50,opacity);
        ofCircle(controlX, tempoY, controlRad);
    
        //EQ
        if (manEQ) ofSetColor(255,0,0,opacity);
        else ofSetColor(50,50,50,opacity);
        ofCircle(controlX, eqY, controlRad);

        ofSetColor(255, 255, 255,opacity);
        volICON.draw(controlX-(volICON.getWidth())/2 , volY-(volICON.getHeight())/2);
        tempICON.draw(controlX-(tempICON.getWidth())/2 , tempoY-(tempICON.getHeight())/2);
        eqICON.draw(controlX-(eqICON.getWidth())/2 , eqY-(eqICON.getHeight())/2);
        
        
        //TRACK SELECTION
        //START/NEXT BUTTON
        if (boolNext) ofSetColor(255,0,0,opacity);
        else ofSetColor(50,50,50,opacity);
        ofCircle(trackX, trackY, trackRad);

        ofSetColor(255, 255, 255,opacity); 
        if (currenttrackNo<playlist.size()-1)
        {
            if (startnext==0) startICON.draw(trackX-(startICON.getWidth())/2 , trackY-(startICON.getHeight())/2);
            else nextICON.draw(trackX-(nextICON.getWidth())/2 , trackY-(nextICON.getHeight())/2);
        }
        else lastICON.draw(trackX-(lastICON.getWidth())/2 , trackY-(lastICON.getHeight())/2);

        if (help==1) tempHELP.draw(ofGetWidth()/2 - (tempHELP.getWidth())/2 , tempHELP.getHeight()/2);
        
}
//--------------------------------------------------------------
void testApp::userEvent(ofxOpenNIUserEvent & event){

    // show user event messages in the console
    ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
}

//--------------------------------------------------------------
void testApp::exit(){
    openNIDevice.stop();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    
    //CLEAN SHUTDOWN.
    if (key=='q') {
        exit();
    }
    
    //CHOOSE AND LOAD FILES
    if (key == ' ')
    {
        
        ofFileDialogResult fileResult = ofSystemLoadDialog("Choose a file");
        loaded=1;
        filepath = fileResult.getPath();
        ofFile tempFile(filepath);
        
        string tempDir = tempFile.getEnclosingDirectory();
        ofDirectory dir;
        dir.open(tempDir);
        int fileNo= dir.listDir();
        playlistFiles= dir.getFiles();
        
        //PRE-LOAD THE SET
        for (int i=0; i<playlistFiles.size(); i++)
        {
            tempSample.load(playlistFiles[i].path());
            tempSample.getLength();
            playlist.push_back(tempSample);
        }
        
        loaded=1;
        
    }
    
    //TOGGLE HELP MESSAGES
    if (key == 'h')
    {
        help=!help;
    }
    
}
//--------------------------------------------------------------