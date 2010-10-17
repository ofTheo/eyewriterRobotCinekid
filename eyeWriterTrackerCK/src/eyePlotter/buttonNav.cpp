/*
 *  buttonNav.cpp
 *  CinekidRobot
 *
 *  Created by theo on 15/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "buttonNav.h"

//------------------------------------------------------
void buttonNav::setup( ofEvent <buttonNavEvent> & navEventIn ){
	navEvent = &navEventIn;
}

//------------------------------------------------------
void buttonNav::update( float x, float y, int pressed){
	
	bool doFade = false;
	
	if( bounds.inside(x, y) && states.size() && !bDisabled && bVisible ){
	
		bool bTrigger = false;
		
//		if( bInside == false ){
//			bTrigger = true;
//		}else if( bRepeat ){
//			if( ofGetElapsedTimef() - lastTimeHit >= repeatTime ){ 
//				bTrigger = true;
//			}
//		}

		if( bInside == false || bRepeat ){

			currentChargeTime += ofGetLastFrameTime();
			chargePct = ofMap(currentChargeTime, 0, triggerTime, 0.0, 1.0, true);
			chargeState = BUTTON_CHARGING;

			if( currentChargeTime >= triggerTime ){
				chargePct = triggerTime;
				currentChargeTime = 0;
				bTrigger = true;
				chargeState = BUTTON_FADING;
			}
		
		}else{
			doFade = true;
		}
	
		if( bTrigger ){
			bInside		= true;
			lastTimeHit = ofGetElapsedTimef();

			if( states.size() ){
			
				if( states.size() > 1 ){
					current++;
					if( current >= states.size() ){
						current = 0;
					}
				}
			
				//printf("%s hit\n", states[current].displayName.c_str() );
				
				if( navEvent != NULL ){
					buttonNavEvent tmpEvent;
					tmpEvent.eventName = states[current].eventName;
					ofNotifyEvent(*navEvent, tmpEvent, this);
				}
			}
			
			
		}
		
	}else{
		bInside = false;
		doFade  = true;
		
		if( currentChargeTime > 0.0 ){
			currentChargeTime -= ofGetLastFrameTime();
		}		
		ofClamp(currentChargeTime, 0, triggerTime);
		
	}
	
	if( doFade ){
		chargeState = BUTTON_FADING;
		
		chargePct   *= ofClamp( powf(0.7, ofGetLastFrameTime()/0.016f ) , 0.01, 0.99 );
		
		if( chargePct <= 0.001 ){
			chargePct = 0.0;
			chargeState = BUTTON_AT_REST;
		}else{
			chargeState = BUTTON_FADING;
		}
	}
		
}

//------------------------------------------------------
void buttonNav::triggerState(string stateName){
	for(int i = 0; i < states.size(); i++){
		if( stateName == states[i].eventName ){
			
			current = i;
			if( navEvent != NULL ){
				buttonNavEvent tmpEvent;
				tmpEvent.eventName = states[current].eventName;
				ofNotifyEvent(*navEvent, tmpEvent, this);
			}
			
			break;
		}
	}
}

//------------------------------------------------------
void buttonNav::show(){
	bVisible = true;
}

//------------------------------------------------------
void buttonNav::hide(){
	bVisible = false;
}

//------------------------------------------------------
void buttonNav::enable(){
	bDisabled = false;
}

//------------------------------------------------------
void buttonNav::disable(){
	bDisabled = true;
}

//------------------------------------------------------
void buttonNav::draw(){
	if( !bVisible )return;
	
	ofFill();
	
	if( chargeState == BUTTON_FADING ){
		ofSetColor(100, 100 + chargePct * 40, 100 + chargePct * 160.0);
	}else{
		ofSetColor(100, 100 + chargePct * 140.0, 100, 255);
	}
	
	ofRect(bounds.x, bounds.y, bounds.width, bounds.height);
	
	ofNoFill();
	ofSetColor(10, 10, 10, 255);
	ofRect(bounds.x, bounds.y, bounds.width, bounds.height);
	
	if( states.size() ){
		float  textWidth = states[current].displayName.size() * 8;
		float  diff      = bounds.width - textWidth;
		
		ofPushStyle();
		if( states[current].bHasImage ){
			float drawWidth = MIN(bounds.width, bounds.height) - 5;
			ofEnableAlphaBlending();
			ofSetColor(255, 255, 255, 255);
			states[current].displayImage.setAnchorPercent(0.5, 0.5);
			states[current].displayImage.draw(bounds.x + bounds.width/2, bounds.y + bounds.height/2, drawWidth, drawWidth);
		}else{
			ofFill();
			ofSetColor(160, 160, 160, 255);
			ofDrawBitmapString(states[current].displayName, bounds.x + diff/2, 6 + bounds.y + bounds.height/2 );
		}
		ofPopStyle();
		
	}else{
		printf("buttonNav::draw() error no states found!\n");
	}
	
	if( bDisabled ){
		ofPushStyle();
			ofFill();
			ofEnableAlphaBlending();
			ofSetColor(170, 170, 170, 200);
			ofRect(bounds.x-1, bounds.y-1, bounds.width+2, bounds.height+2);
		ofPopStyle();
	}
}
