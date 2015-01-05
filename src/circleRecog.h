//#ifndef _CIRCLE_RECOG
#define _CIRCLE_RECOG

#include "ofMain.h"

class circleRecog {

	//LIKELIHOOD
	float likelihood;

	//LIKELIHOOD THRESHOLD
	float threshold;

	//SEQUENCE BUFFER
	vector<int> states;

	vector<ofPoint> points;
	//vector<ofPoint> avPoints;

	//CONSTANTS FOR NORMALISATION
	float avX, avY, avZ, sumX, sumY, sumZ;

	public:
		
		circleRecog();

		bool isCircle(ofPoint pt);
		int getCentroid(ofPoint pt);

		float getLike(vector<int> states);

		int E [5][5];
		float T [5][5];

		vector<ofPoint> centroids;

};