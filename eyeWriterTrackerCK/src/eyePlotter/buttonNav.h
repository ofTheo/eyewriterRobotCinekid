/*
 *  buttonNav.h
 *  CinekidRobot
 *
 *  Created by theo on 15/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
 
#pragma once
#include "ofMain.h"

class buttonNavStates{
	public:
		string eventName;
		string displayName;
		ofImage displayImage;
		bool bHasImage;
};

class buttonNavEvent{
	public:
	string eventName;
};

typedef enum{
	BUTTON_AT_REST,
	BUTTON_CHARGING,
	BUTTON_FADING,
}buttonChargeState;

class buttonNav{
	public:
		buttonNav(){
			current				= 0;
			bInside				= false;
			bRepeat				= false;
			bVisible			= true;
			lastTimeHit			= 0.0;
			currentChargeTime	= 0.0;
			triggerTime			= 1.2;
			chargePct			= 0.0;
			navEvent			= NULL;
		}
		
		void setup(ofEvent <buttonNavEvent> & navEventIn );
		void triggerState(string stateName);
		void update( float x, float y, int pressed);		
		void draw();
		
		void setTriggerTime(float time);
		
		void show();
		void hide();
		void enable();
		void disable();
				
		string buttonName;
		vector <buttonNavStates> states;
		ofRectangle bounds;
		float spacingAmnt;
		
		bool bInside;
		bool bRepeat;
		
		bool bVisible;
		bool bDisabled;
		
		float repeatTime;
		float lastTimeHit;
		float triggerTime;
		float lastCheckTime;
		float currentChargeTime;
		float chargePct;

		buttonChargeState chargeState;
		ofEvent <buttonNavEvent> * navEvent;


		int current;
};
