/*
 *  drawingScene.cpp
 *  CinekidRobot
 *
 *  Created by theo on 15/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "drawingScene.h"
#include "ofxXmlSettings.h"

//------------------------------------------------------------------------
void drawingScene::setup(float x, float y, float w, float h){
	bgColor.r = 50;
	bgColor.g = 50;
	bgColor.b = 50;
	bgColor.a = 255;
	
	offset  = 0.0;
	bOffsetCheck	= false;

	bPaused			= true;
	
	bounds	= ofRectangle(x, y, w, h);
	timer.setWaitTime(0.7);
	
	drawingNav.setup("nav/drawingBar.xml", "nav/icons/");
	ofAddListener(drawingNav.navEvent,this,&drawingScene::eventsIn);
	ofAddListener(manager.actionEvent,this,&drawingScene::strokeEvents);

	buttonNav * b = drawingNav.getButton("PAUSED");
	b->triggerState("PAUSED");
	
	
	// -- abb robot stuff
	ofxXmlSettings xml;
	if( !xml.loadFile("robotSettings.xml") ){
		printf("unable to load settings!\n");
	}
	
	lastPoint.set(0,0,0);
	bSentToRobot		= false;
	bSendingToRobot		= false;
	
	int port            = xml.getValue("PORT", 1600);
	
	int mode			= xml.getValue("DRAW_MODE", 0);
	if( mode == 0 ){
		abbMode = ABB_DELAYED;
	}else if( mode == 1 ){
		abbMode = ABB_SEMI;	
	}else if( mode == 2 ){
		abbMode = ABB_IMMEDIATE;	
	}
	
	betweenPointMillis  = xml.getValue("BETWEEN_MILLIS", 20);
	int scaleW			= xml.getValue("SCALE_X", 1024);
	int scaleH			= xml.getValue("SCALE_Y", 768);
	int endChar			= xml.getValue("END_CHAR", -1);
	int tcpMode			= xml.getValue("ABB_TCP_MODE", 0);
	
	abb.setup(port, (serverComMode)tcpMode, scaleW, scaleH, 1.0);
    abb.setEndChar(endChar);	
	
	// --
}

//------------------------------------------------------------------------
void drawingScene::checkOffset(){
	offset = 0;
	numCheck = 0;
	bOffsetCheck = true;
	offsetCheckTill = ofGetElapsedTimef() + 1.5;
	
	buttonNav * b = drawingNav.getButton("PAUSED");
	b->triggerState("PAUSED");	
}

//------------------------------------------------------------------------
void drawingScene::eventsIn(buttonNavEvent & event){
	printf("received event %s\n", event.eventName.c_str());
	
	string e = event.eventName;
	
	if( e == "PAUSED" ){
		bPaused = true;
		
		for(int k = 0; k < drawingNav.buttons.size(); k++){
			if( drawingNav.buttons[k].buttonName != "PAUSED" ){
				drawingNav.buttons[k].disable();
			}
		}
		
	}
	if( e == "DRAWING" ){
		bPaused = false;
		for(int k = 0; k < drawingNav.buttons.size(); k++){
			drawingNav.buttons[k].enable();
		}
		
		//if we have just sent the drawing and then started a new one
		//clear everything.
		if( bSentToRobot ){
			manager.clear();
			abb.clearHistory();
			if( abb.bDrawingStarted ){
				abb.endDrawing();
			}
			bSentToRobot = false;
		}
				
	}
	if( e == "NEW_SHAPE" ){
		manager.newShape();
	}	
	if( e == "NEW_STROKE" ){
		manager.newStroke();
	}		
	if( e == "UNDO_DRAW" ){
		manager.undoLastPoint();
	}			
	
	if( e == "SEND_TO_ROBOT" ){
		if( abbMode == ABB_DELAYED ){
			manager.autoScaleGroups(ofRectangle(80, 160, ofGetWidth()-160, ofGetHeight()-200) );
		}
	
		buttonNav * b = drawingNav.getButton("PAUSED");
		b->triggerState("PAUSED");
		
		manager.newShape();		
		
		if( abbMode == ABB_DELAYED ){
			b->disable();
			bSendingToRobot = true;
			timeStarted = ofGetElapsedTimef();
			currentPoint = 0;
			targetPoint  = 0;
			
			for(int k = 0; k < manager.group.size(); k++){
				targetPoint += manager.group[k].getTotalNumPoints();
				//sendLetterToRobot(manager.group[k], betweenPointMillis);
			}
		}else{
		
			if( abb.bDrawingStarted ){
				abb.endDrawing();
				bSentToRobot = true;
			}	
		}
			
	}
}

//------------------------------------------------------------------------
void drawingScene::strokeEvents(strokeEvent & event){
	
	if( event.strokeAction == "madeNewShape"){
		if( abbMode == ABB_SEMI ){
			sendLetterToRobot(manager.group[manager.group.size()-2], betweenPointMillis);
		}
	}
	
	if( event.strokeAction == "newStroke" ||  event.strokeAction == "newShape" ){
		if( abbMode == ABB_IMMEDIATE ){
			abb.clearHistory();			
			abb.moveUp(lastPoint.x/bounds.width, lastPoint.y/bounds.height);
		}
	}
	
}

//
//-----------------------------------------------------------------
void drawingScene::sendLetterToRobot(strokeGroup & g, int sleepTime){
	//abb.clearHistory();

	int numStrokes = g.strokes.size();
	for(int k = 0; k < numStrokes; k++){
		int numPts = g.strokes[k].pts.size();

		if( numPts < 2 ){
			continue;
		}
		for(int j = 0; j < numPts; j++){
			float x = g.strokes[k].pts[j].x;
			float y = g.strokes[k].pts[j].y;

			x	/= bounds.width;
			y	/= bounds.height;

			if( j == 0 ){
				if( !abb.bDrawingStarted ){
					abb.startDrawingAtPoint(x, y);
				}else{
					if( !abb.bIsUp ) abb.moveUp(x, y);

					ofSleepMillis(sleepTime);
					abb.moveDown(x, y);
				}
			}else{
				abb.moveDown(x, y);
			}

			ofSleepMillis(sleepTime);
			if( j == numPts-1 ){
				abb.moveUp(x, y);
			}
		}
	}
}

//------------------------------------------------------------------------
void drawingScene::update(float mx, float my){
	abb.update();

	current.set(mx, my);

	if( bSendingToRobot ){
		
		if( ofGetElapsedTimef() - timeStarted > currentPoint ){
		
			bool bFirst = false;
			bool bLast  = false;
			ofPoint pt = manager.getPoint(currentPoint, bFirst, bLast);
			
			if( currentPoint == 0 ){
				abb.startDrawingAtPoint(pt.x/bounds.width, pt.y/bounds.height);
			}else{
				abb.moveDown(pt.x/bounds.width, pt.y/bounds.height);
			}
			
			if( bLast ){
				ofSleepMillis(betweenPointMillis);
				abb.moveUp(pt.x/bounds.width, pt.y/bounds.height);			
			}
			
			currentPoint++;
		}
				
		if( currentPoint > targetPoint ){
			currentPoint = targetPoint;

			if( abb.bDrawingStarted ){
				abb.endDrawing();
			}	

			bSentToRobot	= true;
			bSendingToRobot = false;
			
			buttonNav * b = drawingNav.getButton("PAUSED");
			b->enable();
		}
		
	}

	if( bOffsetCheck ){
	
		if( ofGetElapsedTimef() >= offsetCheckTill ){
			offset /= numCheck;
			bOffsetCheck = false;
		}else if( ofGetElapsedTimef() >= offsetCheckTill - 0.7){
			offset += ofPoint(ofGetWidth()/2, ofGetHeight()/2) - current;
			numCheck++;
		}
	}
	
	timer.update(current.x, current.y);
	drawingNav.update(current.x, current.y, true);
	
	bInsideNav = drawingNav.bounds.inside(current.x, current.y);
	
	if( !bInsideNav && !bPaused ){
		if( timer.isPointStationary(9) ){
			manager.addPoint(current.x, current.y);
			
			lastPoint = current;
			
			if( abbMode == ABB_IMMEDIATE ){
				if( !abb.bDrawingStarted ){
					abb.startDrawingAtPoint(lastPoint.x/bounds.width, lastPoint.y/bounds.height);
				}else{
					abb.moveDown(lastPoint.x/bounds.width, lastPoint.y/bounds.height);
				}
			}
		}
		//draw here
	}	
	
}

//------------------------------------------------------------------------
void drawingScene::draw(){
	
	ofFill();
	ofSetColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
	ofRect(bounds.x, bounds.y, bounds.width, bounds.height);

	if( ! bOffsetCheck ){
		
		ofPushStyle();
			ofSetLineWidth(1.0);
			ofEnableAlphaBlending();
			ofSetColor(0, 0, 0, 80);

			for(int x = 0; x < bounds.width; x+= 20){
				if( x % 100 == 0 ){
					ofSetLineWidth(2);
				}else{
					ofSetLineWidth(1);			
				}
				ofLine(x, 0, x, bounds.height);
			}

			for(int y = 0; y < bounds.height; y+= 20){
				if( y % 100 == 0 ){
					ofSetLineWidth(2);
				}else{
					ofSetLineWidth(1);			
				}		
				ofLine(0, y, bounds.width, y);
			}
		ofPopStyle();
		
		
		if( !bPaused && !bInsideNav){
			ofSetColor(240, 240, 240);
			manager.drawGuideLine(current.x, current.y);
		}	

		if( bSendingToRobot ){
		
			ofEnableAlphaBlending();
			ofSetColor(240, 240, 240, 30);
			manager.drawAllShapes();
			
			ofSetColor(240, 240, 240, 200);			
			manager.drawAllShapesUpToPoint(currentPoint);
		
		}else if(  bSentToRobot ){
		
			ofEnableAlphaBlending();
			ofSetColor(240, 240, 240, 200);
			manager.drawAllShapes();
					
		}else{
			ofPushStyle();
				ofSetColor(240, 240, 240);
				manager.drawCurrentShape();
				
				ofEnableAlphaBlending();
				ofSetColor(240, 240, 240, 60);
				ofRectangle bRect = ofRectangle(10, 130, ofGetWidth()-20, 140);
				manager.drawAllShapesInRect(bRect);
				ofNoFill();
				//ofRect(bRect.x, bRect.y, bRect.width, bRect.height);
			ofPopStyle();
		}
		
		ofPushStyle();

			ofEnableAlphaBlending();
			ofSetColor(200, 200, 200, 80);

			ofDrawBitmapString("Mode: " + ( abbMode == ABB_IMMEDIATE ? string("immediate") : string("delayed") ) , 15, drawingNav.bounds.height+20);
			ofDrawBitmapString("Server Status: " + abb.getStateString() , 15, drawingNav.bounds.height+35);

			for(int k = 0; k < abb.msgHistory.size(); k++){
				ofDrawBitmapString(abb.msgHistory[k], 15, drawingNav.bounds.height+50+18*k);
			}

		ofPopStyle();	

	}else{
		
		ofSetColor(0, 0, 0, 255);
		ofCircle(ofGetWidth()/2, ofGetHeight()/2, 2 + ofMap(ofGetElapsedTimef(), offsetCheckTill-1.5, offsetCheckTill-0.75, 40.0, 0.0, true));
	
	}
	
	drawingNav.draw();
}