/*
 *  eyeTimer.h
 *  CinekidRobot
 *
 *  Created by theo on 15/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxVec2f.h"

class timePoint : public ofxVec2f{
	public:
		timePoint(float _x, float _y, float _t){
			x = _x;
			y = _y;
			t = _t;
		}
		float t;
};


class eyeTimer{

	public:
		eyeTimer();
		
		void setWaitTime(float newWaitTime);
		void clear();
		void update(float x, float y);
		bool isPointStationary(float radiusThreshold );
		
		float waitTime;
		vector <timePoint> ptHistory;
		bool needsMove;
};