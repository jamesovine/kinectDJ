//CLASS FOR RECOGNISING CIRCLE GESTURES    
#include "circleRecog.h"

circleRecog::circleRecog ()
{

    //INITIALISE TRANSITION AND EMISSION MATRICES, WITH DATA FROM TRAINING IN MATLAB

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            T[i][j] = 0;
            if (i == j) E[i][j] = 1;
            else E[i][j] = 0;
        }
    }

    //T[0][0]=0.0909;
    T[0][2]=1;
    //T[0][3]=0.4545;
    T[1][4]=1;
    T[2][0]=0.1250;
    T[2][4]=0.8750;
    //T[2][0]=1;
    //T[3][1]=0.0909;
    T[3][1]=0.7273;
    //T[3][3]=0.0909;
    T[3][4]=0.0909;
    T[4][0]=1;

    //INITIALISE CENTROIDS

    centroids.push_back(ofPoint( -82.4519,  -26.9923,    45.5083));
    centroids.push_back(ofPoint(  -0.5962,  -40.1515,    -3.9982));
    centroids.push_back(ofPoint( -43.3377,   33.4831,    -4.6255));
    centroids.push_back(ofPoint(  -4.6570,   22.8990,   -14.4753));
    centroids.push_back(ofPoint( -39.2687,  -90.9638,    45.9134));

    //INITIALISE AVERAGE X,Y,Z VALUES FOR NORMALISTAION
    //VALUES AGAIN GATHEREED FORM MATLAB ANALYSIS

    avX = 347.3861;
    avY = 167.1207;
    //avZ = 1257.4;
    
    sumX, sumY, sumZ = 0;

    //threshold=-100;
    threshold = -20.7078;
    
} 

//DETERMINE STATE/CENTROID THAT CURRENT POINT BELONGS TO
int circleRecog::getCentroid(ofPoint pt)
{

    float minDistance=1000000;
    int label=0;

    //NORMALISE
    pt.x -= avX;
    pt.y -= avY;
    
    //cout << "Point X is: " << pt.x << endl;
    //pt.z -= avZ;

    for (int i=0; i<centroids.size(); i++)
    {
        float dist = sqrt(pow(centroids[i].x-pt.x,2) + pow(centroids[i].y-pt.y,2)); // + pow(centroids[i].z-pt.z,2));

        if (dist<minDistance)
        {
            minDistance=dist;
            label= i;
        }
    }
    
    cout << "Label is: " << label << endl;

    return label;
}

//LIKELIHOOD THAT CURRENT STATE BUFFER CONTAINS CIRCLE GESTURE
float circleRecog::getLike(vector<int> states)
{
    //BUFFER WILL BE OF SIZE 60
    //NEED TO DETERMINE LIKELIHOOD A SEQUENCE IS A CIRCLE
    float lik=1;


    //CHECK BUFFER IS CORRECT SIZE    
    if (states.size() == 5)
    {   
        
        //IF STATES[0] != 3, WE DONT HAVE A GESTURE
        //From Matlab data, we have that state 3 represents the top of circle (first state)
        if (states[0] == 3)
        {   

            //PROBABILITY OF A SEQUENCE AT EACH TIME STEP REDUCES TO MARKOV CHAIN
            //SINCE WE KNOW EMISSIONS WITH CERTAINTY
            //int count = 0;
            for (int i=1; i<states.size(); i++)
            {
                
                //if (states[i] == 3) count++;
                lik *= T[states[i-1]][states[i]];

            }

        }
    }

    cout << "LIK IS: " << lik << endl;

    if (lik==1 || lik == 0) return -1000;

    else
    {
        //VALUES SO SMALL, NEED TO TAKE LOG
        lik = log(lik);

        return lik;
    }
}

bool circleRecog::isCircle(ofPoint pt)
{
    
    vector<ofPoint> avPoints;

    points.push_back(pt);
    if (points.size() == 61) points.erase(points.begin());

    //GET AV POINT VECTOR SIZE 5
    if (points.size() == 60)
    {
        //cout << "In here" << endl;
        
        for (int i = 0; i<5; i++)
        {
            for (int j = 0; j<12; j++)
            {
                sumX += points[j+12*i].x;
                sumY += points[j+12*i].y;
                sumZ += points[j+12*i].z;
            }
            
            avPoints.push_back(ofPoint(sumX/12,sumY/12,sumZ/12));
            
            sumX=0;
            sumY=0;
            sumZ=0;
        }
        for (int i = 0; i < 5; i++)
        {
            int state = getCentroid(avPoints[i]);
    
            states.push_back(state);   
        }
    }
    
    //cout << "Size of states is: " << states.size() << endl;

    likelihood=getLike(states);

    //KEEP BUFFER SIZE 5
    states.clear();

    //FOUND A CIRCLE
    if (likelihood > threshold) return 1;

    //NO CIRCLE
    else return 0;
    
}
