/*
 *  strokeManager.cpp
 *  CinekidRobot
 *
 *  Created by theo on 15/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "strokeManager.h"
#include "strokeUtils.h"

//-------------------------------------------------------------------
void strokeManager::clear(){
	group.clear();
	tmpCpy.clear();
	sendEvent("clear");				
}

//-------------------------------------------------------------------
void strokeManager::addPoint(float x, float y){

	if( group.size() == 0 ){
		group.addGroup();
		sendEvent("madeNewGroup");			
	}
	
	group.back().addPoint(x, y, ofGetElapsedTimef(), 1.0);
	sendEvent("addPoint");				
}

//-------------------------------------------------------------------
void strokeManager::undoLastPoint(){
	if( group.size() > 0 ){
		if( group.back().hasPoints() ){
			stroke & s = group.back().strokes.back();
			s.undo();
		}
		if( group.size() ){
			if( group.back().strokes.size() && !group.back().strokes.back().hasPoints() ){
				group.back().strokes.pop_back();
			}
			if( group.back().strokes.size() == 0 ){
				group.pop_back();
			} 	
		}
		sendEvent("undo");	
		tmpCpy = group.getVectorCopy();
		if( tmpCpy.size() ){
			tmpCpy.pop_back();
		}	
	}
}

//-------------------------------------------------------------------
void strokeManager::newStroke(){
	if( group.back().strokes.size() && group.back().getLastStrokeNumPoints() <= 1 ){
		group.back().strokes.back().pts.clear();
	}else{
		//otherwise make a new stroke.
		group.back().begin();
	}
	
	sendEvent("newStroke");	
}

//-------------------------------------------------------------------
void strokeManager::sendEvent(string eventType){
	strokeEvent tmpEvent;
	tmpEvent.strokeAction = eventType;
	ofNotifyEvent(actionEvent, tmpEvent, this);
}

//-------------------------------------------------------------------
void strokeManager::autoScaleGroups(ofRectangle targetRect, float spacing){
	strokeUtils::autoPlaceAndScaleByWidth(group.getVector(), targetRect, false, spacing);
}

//-------------------------------------------------------------------
void strokeManager::newShape(){
	if( group.back().getTotalNumPoints() > 1 ){

		tmpCpy = group.getVectorCopy();

		group.addGroup();
		group.back().begin();
		group.back().style = groupStyle(group.size());
		
		sendEvent("madeNewShape");			

	}else{
		group.back().clear();
	}

	sendEvent("newShape");
		
}

//-------------------------------------------------------------------
void strokeManager::drawAllShapes(){
	
	if( group.size() ){
		for(int i = 0; i < group.size(); i++){
			group[i].draw();
		}
	}
}

//-------------------------------------------------------------------
void strokeManager::drawCurrentShape(){
	if( group.size() ){
		group.back().draw();
	}
}

//-------------------------------------------------------------------
void strokeManager::drawGuideLine(float tx, float ty){

	if( group.size() && group.back().hasPoints() ){

		if( !group.back().newStrokeOnNext ){
			ofPoint pt = group.back().getLastPoint();

			glLineStipple(1, 0x3F07 );
			glEnable( GL_LINE_STIPPLE );
			ofLine(tx, ty, pt.x, pt.y);
			glDisable(GL_LINE_STIPPLE);
		}
	}
}

//-------------------------------------------------------------------
void strokeManager::drawAllShapesUpToPoint(int thePoint){
	ofPushStyle();
	
		int theCount = 0;
		if( group.size() ){
			for(int i = 0; i < group.size(); i++){
				for(int j = 0; j < group[i].strokes.size(); j++){
					
					int nPts = group[i].strokes[j].pts.size();
					
					ofNoFill();
					ofBeginShape();
					for(int k = 0; k < nPts; k++){
						ofVertex(group[i].strokes[j].pts[k].x, group[i].strokes[j].pts[k].y);
						theCount++;
						if( theCount >= thePoint )break;
					}
					ofEndShape();
					if( theCount >= thePoint )break;				
				}
				if( theCount >= thePoint )break;
			}
		}
	
	ofPopStyle();
	
}

//-------------------------------------------------------------------
ofPoint strokeManager::getPoint(int thePoint, bool & bFirstStroke, bool & bLastStroke){
	
	int theCount = 0;
	if( group.size() ){
		for(int i = 0; i < group.size(); i++){
			for(int j = 0; j < group[i].strokes.size(); j++){					
				int nPts = group[i].strokes[j].pts.size();
				for(int k = 0; k < nPts; k++){
					theCount++;
					if( theCount >= thePoint ){
						if( k == 0 ){
							bFirstStroke = true;
						}else{
							bFirstStroke = false;						
						}
						if( k == nPts-1 ){
							bLastStroke = true;
						}else{
							bLastStroke = false;						
						}						
						return ofPoint(group[i].strokes[j].pts[k].x, group[i].strokes[j].pts[k].y);
					}
				}
			}
		}
	}
	
	return ofPoint();
}

//-------------------------------------------------------------------
void strokeManager::drawAllShapesInRect(ofRectangle rect){

	strokeUtils::autoPlaceAndScaleByWidth(tmpCpy, rect, false);
	if( tmpCpy.size() ){
		for(int i = 0; i < tmpCpy.size(); i++){
			tmpCpy[i].draw();
		}
	}	
}



