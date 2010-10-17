/*
 *  drawingScene.h
 *  CinekidRobot
 *
 *  Created by theo on 15/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "xmlNav.h"
#include "strokeManager.h"
#include "eyeTimer.h"
#include "strokeToABB.h"

class drawingScene{
	public:
		drawingScene(){
		
		}

		void eventsIn(buttonNavEvent & event);
		void strokeEvents(strokeEvent & event);
		
		void sendLetterToRobot(strokeGroup & g, int sleepTime);		

		void setup(float x, float y, float w, float h);
		void update(float x, float y);
		void draw();
		
		ofColor bgColor;
		ofRectangle bounds;
		
		strokeToABB abb;
		serverMode abbMode;		
		ofPoint lastPoint;
		bool bSentToRobot;
		int betweenPointMillis;		
		
		strokeManager manager;
		eyeTimer timer;
		
		bool bPaused;
		ofPoint current;
		bool bInsideNav;
		
		xmlNav drawingNav;
};