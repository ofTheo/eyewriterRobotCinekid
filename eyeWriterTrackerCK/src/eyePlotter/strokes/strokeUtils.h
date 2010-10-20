/*
 *  strokeUtils.h
 *  openFrameworks
 *
 *  Created by theo on 21/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include "strokeGroup.h"

class strokeUtils{

	public:


	static ofRectangle getBoundingRectFromGroups( vector <strokeGroup> & strokeGroups, bool bCheckSelectionFlag = true){
	
			ofRectangle rect; 			
			float minX, maxX, minY, maxY = 0;
		
			int c = 0;
			for(int k = 0; k < strokeGroups.size(); k++){
				if( !strokeGroups[k].selected() && bCheckSelectionFlag )continue;
				
				if( !strokeGroups[k].hasPoints() )continue;
			
				rect = strokeGroups[k].getBoundingRect();
				
				if( rect.width == 0 || rect.height == 0)continue;
				
				if( c == 0 ){
					minX = rect.x;
					minY = rect.y;
					maxX = rect.x + rect.width;
					maxY = rect.y + rect.height;
				}else{
					if( rect.x < minX )					minX = rect.x;
					if( rect.y < minY )					minY = rect.y;
					if( rect.x + rect.width > maxX)		maxX =  rect.x + rect.width;
					if( rect.y + rect.height > maxY)	maxY =  rect.y + rect.height;						
				}
				
				c++;
			}
			
			return ofRectangle(minX, minY, maxX-minX, maxY-minY);
	}


	static void scaleGroups( vector <strokeGroup> & strokeGroups, float sx, float sy, bool bCheckSelectionFlag = true){
			
			ofRectangle rect = strokeUtils::getBoundingRectFromGroups(strokeGroups, bCheckSelectionFlag);
			
			float nWidth  = rect.width  * sx ;
			float nHeight = rect.height * sy ;

			ofRectangle newRect( rect.x, rect.y, nWidth, nHeight );
			newRect.x -= (nWidth-rect.width)   * 0.5;
			newRect.y -= (nHeight-rect.height) * 0.5;
			
			for(int j = 0; j < strokeGroups.size(); j++){

				if( !strokeGroups[j].selected() && bCheckSelectionFlag ) continue;
			
				strokeGroup & group = strokeGroups[j];
				
				for(int k = 0; k < group.strokes.size(); k++){
				
					for(int i = 0; i < group.strokes[k].pts.size(); i++){
						group.strokes[k].pts[i].x -= rect.x;
						group.strokes[k].pts[i].y -= rect.y;
						group.strokes[k].pts[i].x /= rect.width;
						group.strokes[k].pts[i].y /= rect.height;
						
						group.strokes[k].pts[i].x *= newRect.width;
						group.strokes[k].pts[i].y *= newRect.height;	
						group.strokes[k].pts[i].x += newRect.x;
						group.strokes[k].pts[i].y += newRect.y;			
					}
					group.strokes[k].updateBoundingRect();
				}
				
				group.updateBoundingRect();
			
			}

	}

	
	static void autoPlaceAndScaleByWidth(vector <strokeGroup> & strokeGroups, ofRectangle fitRect, bool bCheckSelectionFlag = true, float letterSpacing = 0.0){
		
		float posX = fitRect.x;
		float posY = fitRect.y;
		
		float newHeight = fitRect.height;
		
		for(int i = 0; i < strokeGroups.size(); i++){
			if( bCheckSelectionFlag && !strokeGroups[i].selected() )continue;
		
			ofRectangle bounding = strokeGroups[i].getBoundingRect();
			if( bounding.height == 0 )continue;
			
			float scaleAmnt = newHeight/bounding.height;
			
			strokeGroups[i].scale(scaleAmnt, scaleAmnt);
			bounding = strokeGroups[i].getBoundingRect();
			
			float xDiff = posX - bounding.x;
			float yDiff = posY - bounding.y;
			
			strokeGroups[i].shift(xDiff, yDiff);
			
			posX += bounding.width + letterSpacing;
			
			if( strokeGroups[i].rightSpacing > 0.0 ){
			
				printf("shifting by %f\n", strokeGroups[i].rightSpacing * scaleAmnt);
				posX += strokeGroups[i].rightSpacing * scaleAmnt;
			
			}
			
		}
		
		ofRectangle rect = strokeUtils::getBoundingRectFromGroups(strokeGroups, bCheckSelectionFlag);
				
		if( rect.width == 0 || rect.height == 0 )return; 
				
		float scaleAmntX = fitRect.width / rect.width;
		float scaleAmntY = fitRect.height / rect.height;

		float scaleAmnt	 = scaleAmntX < scaleAmntY ? scaleAmntX : scaleAmntY;
		
		strokeUtils::scaleGroups( strokeGroups, scaleAmnt, scaleAmnt, bCheckSelectionFlag );
		rect = strokeUtils::getBoundingRectFromGroups(strokeGroups, bCheckSelectionFlag);

		float yDiff		 = (fitRect.y - rect.y) + fabs(fitRect.height - rect.height)/2.0;
		float xDiff		 =  fitRect.x - rect.x;
	
		for(int k = 0; k < strokeGroups.size(); k++){
			if( !strokeGroups[k].selected() && bCheckSelectionFlag ) continue;			
			strokeGroups[k].shift(xDiff, yDiff);
		}
	
	}


	static void rotateGroups(vector <strokeGroup> & strokeGroups, float angle, bool bCheckSelectionFlag = true){

		ofRectangle rect = strokeUtils::getBoundingRectFromGroups(strokeGroups, bCheckSelectionFlag);

		ofxPoint2f mid(rect.x + rect.width/2, rect.y + rect.height/2);
		ofxPoint2f tmp;

		for(int j = 0; j < strokeGroups.size(); j++){

			if(!strokeGroups[j].selected() && bCheckSelectionFlag) continue;

			strokeGroup & group = strokeGroups[j];

			for(int k = 0; k < group.strokes.size(); k++){
				for(int i = 0; i < group.strokes[k].pts.size(); i++){
					tmp.set(group.strokes[k].pts[i].x, group.strokes[k].pts[i].y);
					tmp.rotate(angle, mid);
					group.strokes[k].pts[i].x = tmp.x;
					group.strokes[k].pts[i].y = tmp.y;
				}
				group.strokes[k].updateBoundingRect();
			}
			
			group.updateBoundingRect();
		
		}			
	}


};