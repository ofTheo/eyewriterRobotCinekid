/*
 *  eyeTimer.cpp
 *  CinekidRobot
 *
 *  Created by theo on 15/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "eyeTimer.h"

//---------------------------------------------
eyeTimer::eyeTimer(){
	waitTime = 0.9f;
	needsMove = false;
}

//---------------------------------------------
void eyeTimer::setWaitTime(float newWaitTime){
	waitTime = newWaitTime;
}	

//---------------------------------------------
void eyeTimer::clear(){
	ptHistory.clear();
}

//---------------------------------------------
void eyeTimer::update(float x, float y){

	ptHistory.push_back(timePoint(x, y, ofGetElapsedTimef()));
	
	float currentT = ofGetElapsedTimef();
	
	for(int i = 0; i < ptHistory.size(); i++){
		int k = ptHistory.size() - (i+1);
		
		if( currentT - ptHistory[k].t > waitTime ){
			ptHistory.erase(ptHistory.begin(), ptHistory.begin() + k);
			break;	
		 } 
	}
	
}

//---------------------------------------------
bool eyeTimer::isPointStationary(float radiusThreshold ){
	
	float threshToCheck = radiusThreshold;
	
	bool isStationary	= false;
	
	if( ptHistory.size() ){
		
		ofxVec2f averagePos = 0.0f;
		
		float oldestPoint = ptHistory[0].t;
		if( ofGetElapsedTimef() - oldestPoint <= waitTime){
			return false;
		}
		
		for(int i = 0; i < ptHistory.size(); i++){
			averagePos += ptHistory[i];
		}
		averagePos /= ptHistory.size();

		isStationary = true;
		for(int i = 0; i < ptHistory.size(); i++){
			if( ( ptHistory[i] - averagePos ).lengthSquared() > threshToCheck*threshToCheck ){
				isStationary = false;
				break;
			}
		}
		
		if( isStationary && needsMove == false){
			ptHistory.clear();
			needsMove = true;
		}else if( isStationary ){
			isStationary = false;
		}else{
			needsMove = false;
		}

	}
	
	return isStationary;
}
