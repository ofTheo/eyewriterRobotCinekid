/*
 *  typingScene.cpp
 *  openFrameworks
 *
 *  Created by andrea bradshaw on 4/10/2010.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "typingScene.h"
#include "testApp.h"

//switch to gui

buttonTrigger enterButton;


//--------------------------------------------------------------
void typingScene::setup(){
	
	franklinBook.loadFont("fonts/HelveticaNeueMed.ttf", 32);
	
	franklinBookSmall.loadFont("fonts/HelveticaNeueMed.ttf", 16);
	carriageReturnCounter = 0;
	//bCount=390;
	ofBackground(255, 255, 255);
	shiftOn = false;
	
	bCount = 56;
	
	panel.setup("type panel", ofGetWidth()-250, 10, 250, 400);
	panel.addPanel("settings", 1, false);
	panel.addSlider("buttonCount", "buttonCount", bCount, 10, 200, true);
	panel.hide();
	panel.loadSettings("Settings/typeSettings.xml");
	
	string buttons[26] = 
	{ "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", 
		"K", "L", "M", "N", "O", "P", "Q", "R", "S", 
		"T", "U", "V", "W", "X", "Y", "Z"
	};
	
	
	bSpeakWords = false;
	
	float xadd2 = 0;
	
	yadd2 = 130;
	
	float sx = 15;
	float spacingX = 25;
	
	float xStart  = sx;
	float yStart  = 25;
	float bWidth  = 120;
	float bHeight = 100;
	
	for (int i = 0; i < 26; i++){
		buttonTrigger nButton;
		nButton.setup(buttons[i], xStart + xadd2, yStart + yadd2, bWidth, bHeight);
		nButton.setMaxCounter(bCount);
		nButton.setRetrigger(false);
		letterButtons.push_back(nButton);
		
		xStart += spacingX+bWidth;
		if (xStart > 1024-(bWidth+sx) ){
			//if (xStart > 900){
			xStart = sx;
			yStart += 125;
		}
		
	}
	
	buttonTrigger spaceButton;
	spaceButton.setup("SPACE", xStart + xadd2, yStart + yadd2, bWidth*2 + 25, bHeight);
	spaceButton.setMaxCounter(bCount);
	spaceButton.setRetrigger(false);
	letterButtons.push_back(spaceButton);	
	
	float xadd = 150;
	float yadd = 250;
	
	bWidth  = 75;
	bHeight = 75;
	
	xadd = 350;
	
//	buttonTrigger semiButton;
//	semiButton.setup(":\n;", 825+xadd, 295+yadd, bWidth, bHeight);
//	semiButton.setMaxCounter(bCount);
//	semiButton.setRetrigger(false);
//	letterButtons.push_back(semiButton);
//	
//	buttonTrigger quoteButton;
//	quoteButton.setup("\"\n'", 915+xadd, 295+yadd, bWidth, bHeight);
//	quoteButton.setMaxCounter(bCount);
//	quoteButton.setRetrigger(false);
//	letterButtons.push_back(quoteButton);
//	
//	buttonTrigger commaButton;
//	commaButton.setup("<\n,", 735+xadd, 385+yadd, bWidth, bHeight);
//	commaButton.setMaxCounter(bCount);
//	commaButton.setRetrigger(false);
//	letterButtons.push_back(commaButton);
//	
//	buttonTrigger periodButton;
//	periodButton.setup(">\n.", 825+xadd, 385+yadd, bWidth, bHeight);
//	periodButton.setMaxCounter(bCount);
//	periodButton.setRetrigger(false);
//	letterButtons.push_back(periodButton);
//	
//	buttonTrigger questionButton;
//	questionButton.setup("?\n/", 915+xadd, 385+yadd, bWidth, bHeight);
//	questionButton.setMaxCounter(bCount);
//	questionButton.setRetrigger(false);
//	letterButtons.push_back(questionButton);
	
	//buttonTrigger deleteButton;
//	deleteButton.setup("ENTER", 735+xadd, 475+yadd, bWidth*2+15, bHeight);
//	deleteButton.setMaxCounter(bCount);
//	deleteButton.setRetrigger(true);
//	letterButtons.push_back(deleteButton);
	
	float shiftX = 100;
	float shiftY = -50;
	
	enterButton.setup("DELETE", ofGetWidth()-bWidth*2.5, 5, bWidth*2+15, bHeight+32);
	enterButton.setMaxCounter(bCount);
	enterButton.setRetrigger(true);
	letterButtons.push_back(enterButton);
	
	buttonTrigger speakAllButton;
	speakAllButton.setup("SEND TO ROBOT", 282+xadd - 230, 475+yadd+shiftY-10, bWidth*2+98, bHeight+9);
	speakAllButton.setMaxCounter(bCount);
	speakAllButton.setRetrigger(false);
	letterButtons.push_back(speakAllButton);
	
	
	buttonTrigger clearButton;
	clearButton.setup("CLEAR ALL", (ofGetWidth()-bWidth*2.5) - 25, 475+yadd+shiftY-18, bWidth*2+39 , bHeight+10);
	clearButton.setMaxCounter(bCount);
	clearButton.setRetrigger(false);
	letterButtons.push_back(clearButton);
	
	
	
//	buttonToggle capsButton;
//	capsButton.setup("CAPS ON", "CAPS OFF", false, 800+xadd+100, 475+yadd, bWidth+15, bHeight);
//	capsButton.setMaxCounter(bCount);
//	actionButtons.push_back(capsButton);
//	
//	
//	buttonToggle speakButton;
//	speakButton.setup("SPEAK\nWORDS\nON", "SPEAK\nWORDS\nOFF", false, 800+xadd+100, 175+yadd, bWidth+15, bHeight);
//	speakButton.setMaxCounter(bCount);
//	actionButtons.push_back(speakButton);
	
	
	letterButtons_lower.push_back("a");
	letterButtons_lower.push_back("b");
	letterButtons_lower.push_back("c");
	letterButtons_lower.push_back("d");
	letterButtons_lower.push_back("e");
	letterButtons_lower.push_back("f");
	letterButtons_lower.push_back("g");
	letterButtons_lower.push_back("h");
	letterButtons_lower.push_back("i");
	letterButtons_lower.push_back("j");
	
	letterButtons_lower.push_back("k");
	letterButtons_lower.push_back("l");
	
	letterButtons_lower.push_back("m");
	
	letterButtons_lower.push_back("n");
	letterButtons_lower.push_back("o");
	letterButtons_lower.push_back("p");
	letterButtons_lower.push_back("q");
	letterButtons_lower.push_back("r");
	letterButtons_lower.push_back("s");
	
	letterButtons_lower.push_back("t");
	letterButtons_lower.push_back("u");
	letterButtons_lower.push_back("v");
	letterButtons_lower.push_back("w");
	letterButtons_lower.push_back("x");
	letterButtons_lower.push_back("y");
	letterButtons_lower.push_back("z");
	letterButtons_lower.push_back(";");
	
//	letterButtons_lower.push_back("'");
//	letterButtons_lower.push_back(",");
//	letterButtons_lower.push_back(".");
//	letterButtons_lower.push_back("/");
	
	
	for (int i = 0; i < letterButtons.size(); i++){
		
		letterButtons[i].setDisplayFont(&franklinBookSmall);
	}
	
	mx = 0.0;
	my = 0.0; 
	
	
	
	
}

void typingScene::sendToRobot(){
	if (displayMessage.size() > 0){
		//speakMe(displayMessage.c_str());
		testApp * tApp = (testApp*)ofGetAppPtr();
		tApp->eyeApp.drawScene.loadStringToRobot(displayMessage);
		tApp->mode = MODE_DRAW;
		displayMessage.clear();
		carriageReturnCounter = 0;
	}	
}

//--------------------------------------------------------------
void typingScene::update(float mouseX, float mouseY){
	mx = mouseX;
	my = mouseY;
	
	panel.update();
	bCount = panel.getValueI("buttonCount");
	
	//ofSetFrameRate(100);
	
	for(int i = 0; i < letterButtons.size(); i++) {
		letterButtons[i].setMaxCounter(bCount);
		if(letterButtons[i].update(mx, my)) {
			if ((carriageReturnCounter == 32)|(carriageReturnCounter == 31)) {
				//displayMessage.push_back("\n");
				//carriageReturnCounter = 0;
			}
			//else if (letterButtons[i].displayText == "ENTER"){
//				displayMessage.push_back("\n");
//				carriageReturnCounter = 0;
//			}
			else if (letterButtons[i].displayText == "SPACE"){
				
				
				if (bSpeakWords == true){
				vector <string> wordsToSpeak = ofSplitString(displayMessage, " ");
				
				if (wordsToSpeak.size() > 0 && displayMessage.size() >= 1){
					if (displayMessage[displayMessage.size()-1] != ' '){
						
					if (wordsToSpeak[wordsToSpeak.size()-1].size() > 0){
						speakMe(	wordsToSpeak[wordsToSpeak.size()-1].c_str() );
					}
					}
				}
				}
				displayMessage.push_back(' ');
				carriageReturnCounter++;
			}
			else if (letterButtons[i].displayText == "CLEAR ALL"){
				displayMessage.clear();
				carriageReturnCounter = 0;
			}
			
			else if (letterButtons[i].displayText == "SEND TO ROBOT"){
				if (displayMessage.size() > 0){
					//speakMe(displayMessage.c_str());
					testApp * tApp = (testApp*)ofGetAppPtr();
					tApp->eyeApp.drawScene.loadStringToRobot(displayMessage);
					tApp->mode = MODE_DRAW;
					displayMessage.clear();
					carriageReturnCounter = 0;
				}
			}
			
			else if (letterButtons[i].displayText == "DELETE"){
				if (displayMessage.size()> 0){
					displayMessage.resize (displayMessage.size () - 1);
				}
			}			
//			else if (shiftOn && (letterButtons[i].displayText == "!\n1")){
//				displayMessage.push_back('!');
//				carriageReturnCounter++;
//			}
//			else if (shiftOn && (letterButtons[i].displayText == "@\n2")){
//				displayMessage.push_back('@');
//				carriageReturnCounter++;
//			}
//			else if (shiftOn && (letterButtons[i].displayText == "#\n3")){
//				displayMessage.push_back('#');
//				carriageReturnCounter++;
//			}
//			else if (shiftOn && (letterButtons[i].displayText == "$\n4")){
//				displayMessage.push_back('$');
//				carriageReturnCounter++;
//			}
//			else if (shiftOn && (letterButtons[i].displayText == "%\n5")){
//				displayMessage.push_back('%');
//				carriageReturnCounter++;
//			}
//			else if (shiftOn && (letterButtons[i].displayText == "^\n6")){
//				displayMessage.push_back('^');
//				carriageReturnCounter++;
//			}
//			else if (shiftOn && (letterButtons[i].displayText == "&\n7")){
//				displayMessage.push_back('&');
//				carriageReturnCounter++;
//			}
//			else if (shiftOn && (letterButtons[i].displayText == "*\n8")){
//				displayMessage.push_back('*');
//				carriageReturnCounter++;
//			}
//			else if (shiftOn && (letterButtons[i].displayText == "(\n9")){
//				displayMessage.push_back('(');
//				carriageReturnCounter++;
//			}
//			else if (shiftOn && (letterButtons[i].displayText == ")\n0")){
//				displayMessage.push_back(')');
//				carriageReturnCounter++;
//			}
//			else if (shiftOn && (letterButtons[i].displayText == ":\n;")){
//				displayMessage.push_back(':');
//				carriageReturnCounter++;
//			}
//			else if (shiftOn && (letterButtons[i].displayText == "\"\n'")){
//				displayMessage.push_back('\"');
//				carriageReturnCounter++;
//			}
//			else if (shiftOn && (letterButtons[i].displayText == "<\n,")){
//				displayMessage.push_back('<');
//				carriageReturnCounter++;
//			}
//			else if (shiftOn && (letterButtons[i].displayText == ">\n.")){
//				displayMessage.push_back('>');
//				carriageReturnCounter++;
//			}
//			else if (shiftOn && (letterButtons[i].displayText == "?\n/")){
//				displayMessage.push_back('?');
//				carriageReturnCounter++;
//			}
			else {
				if (shiftOn){
					displayMessage.push_back(letterButtons[i].displayText.c_str()[0]);
					//carriageReturnCounter += 2;
				}
				else {
					displayMessage.push_back(letterButtons_lower[i].c_str()[0]);
					//carriageReturnCounter++;
				}
			}
		}
		
	}
	
	for (int i = 0; i < actionButtons.size(); i++){
		actionButtons[i].setMaxCounter(bCount);
		if(actionButtons[i].update(mx, my)) {
			if (actionButtons[i].displayText[1] == "CAPS ON"){
				if (shiftOn) shiftOn = false;
				else shiftOn = true;
			}
		}
	}
	
	
	bSpeakWords = false;
	for (int i = 0; i < actionButtons.size(); i++){
		if (actionButtons[i].displayText[1] == "SPEAK\nWORDS\nON"){
			if (actionButtons[i].active){
				bSpeakWords = true;	
			}
		}
	}
	
	//displayMessage = "";
	//for (int i = 0; i < displayMessage.size(); i++){
//		displayMessage += message[i];
//	}
	
	enterButton.setMaxCounter((float)bCount * 0.65);

	
}	

//--------------------------------------------------------------
void typingScene::draw(){
	
	
	ofPushStyle();	
	
	//bool bGrid = showGrid.getState();
	

	
	//float textWidth = 8.0f * displayMessage.length();
	//float remainX = (width - textWidth)/2;
	
	//float textHeight = 14.0f;
	//float remainY = (height - textHeight)/2.0f + (textHeight/2.0f);
	ofFill();
	ofSetColor(0,0,0);
	ofRect(0,0,ofGetWidth(), yadd2);
	
	ofSetColor(255,255,255);
	//ofDrawBitmapString(displayMessage, 200, 600);
	
	
	string layedoutmessage = "";
	
	if (displayMessage.size() > 0){
		int count = 0;
		for (int i = 0; i < displayMessage.size(); i++){
			layedoutmessage.push_back(displayMessage[i]);
			count++;
			if (count >  58){
				layedoutmessage += "-\n";
				count = 0;
			}
		}
	}
	
	float textX = 325;
	float textY = 68;
	
	franklinBook.drawString(layedoutmessage, textX, textY);
	
	
	vector <string> strings = ofSplitString(layedoutmessage, "\n");
	string subMessage = layedoutmessage;
	if(strings.size() > 1){
		subMessage = strings[strings.size()-1];
	}
	float xx = franklinBook.getStringBoundingBox(subMessage + ".", textX, textY).x + franklinBook.getStringBoundingBox(subMessage+ ".", textX, textY).width;
	float yy = franklinBook.getStringBoundingBox(layedoutmessage + ".", textX, textY).y + franklinBook.getStringBoundingBox(layedoutmessage+ ".", textX, textY).height;
	
	ofSetColor(127, 127, 127);
	ofRect(xx, yy, 10,3);
	
	ofPopStyle();
	
	for(int i = 0; i < letterButtons.size(); i++){
		letterButtons[i].draw();
	}
	for(int i = 0; i < actionButtons.size(); i++){
		actionButtons[i].draw();
	}
	
	panel.draw();
	
	//drawCursor();
}


