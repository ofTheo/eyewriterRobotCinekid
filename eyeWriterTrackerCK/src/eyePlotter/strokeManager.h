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
		
		ofPoint getPoint(int thePoint, bool & bFirstStroke, bool & bLastStroke);
		
		void autoScaleGroups(ofRectangle targetRect);
				
		void draw();
		void drawGuideLine(float tx, float ty);
		void drawCurrentShape();
		void drawAllShapes();
		void drawAllShapesInRect(ofRectangle rect);
		void drawAllShapesUpToPoint(int thePoint);
		
		ofEvent <strokeEvent> actionEvent;

		vector <strokeGroup> tmpCpy;
		
		groupCollection group;
		
	private:
		void sendEvent(string eventType);		

};