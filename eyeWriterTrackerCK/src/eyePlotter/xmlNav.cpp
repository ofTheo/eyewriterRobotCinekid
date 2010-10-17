/*
 *  xmlNav.cpp
 *  CinekidRobot
 *
 *  Created by theo on 14/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#include "ofxXmlSettings.h"
#include "xmlNav.h"

//------------------------------------------------------
void xmlNav::setup(string navPath, string resourcePath){	

	if( resourcePath != "" && resourcePath.length() > 0 ){
		if( resourcePath[resourcePath.length()-1] != '/' && resourcePath[resourcePath.length()-1] != '\\' ){
			resourcePath += string("/");
		 } 
	}
	parseXml(navPath, resourcePath);
}

//------------------------------------------------------
void xmlNav::parseXml(string navPath, string resourcePath){
	ofxXmlSettings xml;
	
	if( xml.loadFile(navPath) ){
		xml.pushTag("xmlNav");
		bounds.x		= xml.getValue("bounds:x", 0.0f);
		bounds.y		= xml.getValue("bounds:y", 0.0f);
		bounds.width	= xml.getValue("bounds:w", 0.0f);
		bounds.height	= xml.getValue("bounds:h", 0.0f);
		
		if( bounds.width > bounds.height ){
			bHorizontal = true;
		}else{
			bHorizontal = false;
		}
		
		if( xml.pushTag("elements") ){
		
			ofRectangle buttonBounds;
			
			buttonBounds.x			= xml.getValue("defaultX", 20);
			buttonBounds.y			= xml.getValue("defaultY", 20);
			buttonBounds.width		= xml.getValue("defaultWidth", 120);
			buttonBounds.height		= xml.getValue("defaultHeight", 80);
			
			defaultTrigger = xml.getValue("defaultTrigger", 1.2);
			
			if( bHorizontal ){
				buttonBounds.height = MIN(buttonBounds.height, bounds.height-buttonBounds.y*2);
			}else{
				buttonBounds.width = MIN(buttonBounds.width, bounds.width-buttonBounds.x*2);			
			}
		
			int numButtons = xml.getNumTags("button");
			buttons.assign(numButtons, buttonNav());
			
			for(int k = 0; k < numButtons; k++){		
					
				if( xml.pushTag("button", k) ){
				
					buttonNav & b	= buttons[k];
					b.setup(navEvent);
					
					b.bounds		= buttonBounds;
					b.bounds.x		+= bounds.x;
					b.bounds.y		+= bounds.y;
					
					b.triggerTime	= xml.getValue("triggerTime", defaultTrigger);
					b.repeatTime	= xml.getValue("repeatTime", 0.0);
					b.bRepeat		= ( b.repeatTime > 0.0 );
									
					int numStates = xml.getNumTags("state");
					for(int j = 0; j < numStates; j++){
						buttonNavStates state;
						
						state.eventName		= xml.getValue("state:title", "NO_NAME", j);
						state.displayName   = xml.getValue("state:text", state.eventName, j);
						
						if( xml.tagExists("state:img", j) ){
							state.displayImage.loadImage( resourcePath + xml.getValue("state:img", "noImage.png", j) );
							if( state.displayImage.width > 0 ){
								state.bHasImage = true;
							}else{
								state.bHasImage = false;							
							}
						}else{
							state.bHasImage = false;							
						}
						
						if( j == 0 ){
							b.buttonName = state.eventName;
						}
												
						b.states.push_back( state );
					}

					float spacing = xml.getValue("spacing", 0.0);
					
					if( bHorizontal ){
						buttonBounds.x += spacing + buttonBounds.width;
					}else{
						buttonBounds.y += spacing + buttonBounds.height;				
					}
					
					xml.popTag();
				}
			}
		
			xml.popTag();
		}
	}
}

//------------------------------------------------------
buttonNav * xmlNav::getButton(string buttonName){
	for(int k = 0; k < buttons.size(); k++){
		if( buttons[k].buttonName == buttonName ){
			return &buttons[k];
		}
	}
}

//------------------------------------------------------
void xmlNav::setState(string eventName){
	for(int k = 0; k < buttons.size(); k++){
		for(int j = 0; j < buttons[k].states.size(); j++){
			if( buttons[k].states[j].eventName == eventName ){
				buttons[k].triggerState(eventName);
				return;
			}
		}
	}
}


//------------------------------------------------------
void xmlNav::update( float x, float y, int pressed ){
	
	for(int k = 0; k < buttons.size(); k++){
		buttons[k].update( x, y, pressed );
	}
	
}


//------------------------------------------------------
void xmlNav::draw(){
	ofPushStyle();
		
		ofFill();
		ofSetColor(200, 200, 200, 255);
		ofRect(bounds.x, bounds.y, bounds.width, bounds.height);

		ofNoFill();
		ofSetColor(100, 100, 100, 255);
		ofRect(bounds.x, bounds.y, bounds.width, bounds.height);

		for(int k = 0; k < buttons.size(); k++){
			buttons[k].draw();
		}
	ofPopStyle();
}

