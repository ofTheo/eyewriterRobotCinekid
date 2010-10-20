/*
 *  temptTypeWriter.h
 *  RemoteEyeTracker
 *
 *  Created by theo on 20/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once
#include "groupCollection.h"
#include "ofxXmlSettings.h"
#include "strokeManager.h"
#include "ofMain.h"

class temptTypeWriter{
	public:
		void loadTypeFace(string gmlPath, strokeManager & manager);
		void loadTypeFaceForMsg(string gmlPath, string msg, strokeManager & manager);
		void setStrokesFromString(string sentance, groupCollection &groupIn);
		
		groupCollection fontGroup;
}; 