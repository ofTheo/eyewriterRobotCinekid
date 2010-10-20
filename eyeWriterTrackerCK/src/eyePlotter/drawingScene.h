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
#include "ofxControlPanel.h"
#include "temptTypeWriter.h"

class drawingScene{
	public:
		drawingScene(){
		
		}

		void eventsIn(buttonNavEvent & event);
		void strokeEvents(strokeEvent & event);
		
		void sendLetterToRobot(strokeGroup & g, int sleepTime);		

		void loadStringToRobot(string msg);

		void clearOffset();

		void setup(float x, float y, float w, float h);
		
		void updatePoint(float mx, float my);		
		void update();
		void draw();
		
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void keyPressed(int key);
		
		temptTypeWriter typeWriter;
		
		void checkOffset();
		bool bOffsetCheck;
		float offsetCheckTill;
		int numCheck;		
		ofPoint offset;
		
		ofColor bgColor;
		ofRectangle bounds;
		
		ofxControlPanel panel;
		
		strokeToABB abb;
		serverMode abbMode;		
		ofPoint lastPoint;
		float timeForNextPoint;
		bool bSendingToRobot;
		int currentPoint;
		int targetPoint;
		float timeStarted;
		bool bSentToRobot;
		int betweenPointMillis;		
		
		bool bInsideDrawingArea;
		
		strokeManager manager;
		eyeTimer timer;
		
		bool bPaused;
		ofPoint current;
		bool bInsideNav;
		
		xmlNav drawingNav;
};