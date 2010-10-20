/*
 *  temptTypeWriter.cpp
 *  RemoteEyeTracker
 *
 *  Created by theo on 20/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#include "temptTypeWriter.h"


void temptTypeWriter::loadTypeFace(string gmlPath, strokeManager & manager){
	//group.clear();
	manager.clear();
	
	ofxXmlSettings xml;	
	if( xml.loadFile(gmlPath) ){
		
		if( xml.pushTag("GML") ){
			if( xml.pushTag("tag") ){
				
				int numLetters = xml.getNumTags("drawing");
				printf("%i letters\n", numLetters);
				
				for(int k = 0; k < numLetters; k++){
					if( xml.pushTag("drawing", k)){

						//group.addGroup();
						manager.newShape();
					
						int numStrokes = xml.getNumTags("stroke");
					
						for(int j =0; j < numStrokes; j++){
							if( xml.pushTag("stroke", j) ){
							
								manager.newStroke();							
							
								stroke s;								
								int numPts = xml.getNumTags("pt");
								for(int i = 0; i < numPts; i++){
									
									manager.addPoint(xml.getValue("pt:x", 0.0, i), xml.getValue("pt:y", 0.0, i));
									
									//s.addPoint(xml.getValue("pt:x", 0.0, i), xml.getValue("pt:y", 0.0, i), 0, 1);
								}
								//group.back().strokes.push_back(s);
								
								xml.popTag();
							}
						}
						
						xml.popTag();
					}
				}
			}
		}
		
	}
	
	manager.newShape();

}

void temptTypeWriter::loadTypeFaceForMsg(string gmlPath, string msg, strokeManager & manager){
	//group.clear();
	manager.clear();
	
	ofxXmlSettings xml;	
	if( xml.loadFile(gmlPath) ){
		
		if( xml.pushTag("GML") ){
			if( xml.pushTag("tag") ){
				
				int numLetters = xml.getNumTags("drawing");
				printf("%i letters\n", numLetters);
								
				for(int k = 0; k < msg.size(); k++){
					
					int whichLetter = 0;
					int character  = msg[k];
					
					bool bSpace = false;
					if( character == 32 ){
						bSpace = true;
						
						printf("space!\n");
						
						if( manager.group.size() ){
							manager.group.back().setRightSpacing(manager.group.back().getBoundingRect().width);
							printf("setting right space!\n");
						}
						
						continue;
					}else{
						character -= 97;
					}
					
					character = ofClamp(character, 0, 25);
					
					if( xml.pushTag("drawing", character)){

						//group.addGroup();
						manager.newShape();
					
						int numStrokes = xml.getNumTags("stroke");
					
						for(int j =0; j < numStrokes; j++){
							if( xml.pushTag("stroke", j) ){
							
								manager.newStroke();							
							
								stroke s;								
								int numPts = xml.getNumTags("pt");
								for(int i = 0; i < numPts; i++){
									
									manager.addPoint(xml.getValue("pt:x", 0.0, i), xml.getValue("pt:y", 0.0, i));
									
									//s.addPoint(xml.getValue("pt:x", 0.0, i), xml.getValue("pt:y", 0.0, i), 0, 1);
								}
								//group.back().strokes.push_back(s);
								
								xml.popTag();
							}
						}
						
						xml.popTag();
					}
				}
			}
		}
		
	}
	
	manager.newShape();

}

void temptTypeWriter::setStrokesFromString(string sentance, groupCollection &group){

}