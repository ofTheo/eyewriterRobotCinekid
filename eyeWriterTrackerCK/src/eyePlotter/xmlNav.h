/*
 *  xmlNav.h
 *  CinekidRobot
 *
 *  Created by theo on 14/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "buttonNav.h"

class xmlNav{
	
	public:
	
	void setup(string navPath, string resourcePath = "");
	void update( float x, float y, int pressed );
	void draw();
	
	buttonNav * getButton(string buttonName);
	void setState(string eventName);
	
	
	ofRectangle bounds;
	bool bHorizontal;
	vector <buttonNav> buttons;
	float defaultTrigger;
			
	ofEvent <buttonNavEvent> navEvent;

	private:
		void parseXml(string navPath, string resourcePath);	

};