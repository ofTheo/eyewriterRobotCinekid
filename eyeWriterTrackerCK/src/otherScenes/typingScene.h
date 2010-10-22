/*
 *  drawingScene.h
 *  openFrameworks
 *
 *  Created by theo on 20/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "buttonState.h"

#include "buttonRect.h"
#include "buttonToggle.h"
#include "buttonTrigger.h"

#include "ofxXmlSettings.h"
#include "baseScene.h"

#include "ofxControlPanel.h"

extern "C" {
#include "speak.h"
};


class typingScene : public baseScene{
	
public:
	
	void setup();
	void update(float mouseX, float mouseY);
	void draw();
	void printMessageHistory();
	void sendToRobot();
	
	buttonTrigger	doneButton;
	
	bool bSpeakWords;
	
	int bCount;
	
	
	vector <buttonTrigger> letterButtons;
	string * buttons;
	vector <string> letterButtons_lower;
	vector<string> triggeredMessageLog;
	vector<int> triggeredKeyCodes;
	//vector<string> message;
	vector<buttonToggle> actionButtons;
	string displayMessage;
	int carriageReturnCounter;
	
	ofTrueTypeFont  franklinBook;
	ofTrueTypeFont  franklinBookSmall;
	ofTrueTypeFont	verdana;
	ofTrueTypeFont  franklinBookLarge;
	
	ofxControlPanel panel;
	bool shiftOn;
	
	float yadd2;
	
private:
	void typedMessage(string msg);
	
};

