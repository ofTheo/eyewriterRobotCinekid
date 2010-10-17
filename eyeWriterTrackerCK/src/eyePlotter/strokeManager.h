/*
 *  strokeManager.h
 *  CinekidRobot
 *
 *  Created by theo on 15/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "groupCollection.h"

class strokeEvent{
	public:
		string strokeAction;
};

class strokeManager{
	public:

		void clear();

		void addPoint(float x, float y);
		void undoLastPoint();
		void newStroke();
		void newShape();
				
		void draw();
		void drawGuideLine(float tx, float ty);
		void drawCurrentShape();
		void drawAllShapes();
		
		ofEvent <strokeEvent> actionEvent;
		
		groupCollection group;
		
	private:
		void sendEvent(string eventType);		

};