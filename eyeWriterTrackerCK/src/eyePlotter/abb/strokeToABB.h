/*
 *  strokeToABB.h
 *  EyewriterDrawer
 *
 *  Created by theo on 07/10/2010.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include "ofMain.h"
#include "ofxNetwork.h"

typedef enum{
	ABB_DELAYED,
	ABB_SEMI,
	ABB_IMMEDIATE
}serverMode;

typedef enum{
	ABB_TCP_STRING,
	ABB_TCP_RAW
}serverComMode;

typedef enum{
	ABB_SERVER_WAITING,
	ABB_SERVER_SETUP,
	ABB_SERVER_CLIENT_CONNECTED,
	ABB_SERVER_ERROR_BINDING_TO_PORT
}serverState;

class strokeToABB{

	public:
		strokeToABB(){
			xscale = 1.0;
			yscale = 1.0;
			zscale = 1.0;
			port   = 1200;
			endChar= -1;

			serverWaitTime = 0;

			bDrawingStarted = false;
			bIsUp			= true;
			state			= ABB_SERVER_WAITING;
			comMode			= ABB_TCP_STRING;
		}
		
		~strokeToABB(){
			for(int k = 0; k < tcpServer.getNumClients(); k++){
				tcpServer.disconnectClient(k);
			}
			tcpServer.close();
		}

		void setEndChar(int _endChar){
            endChar = _endChar;
		}

		void setup(int _port, int comModeIn, float xScale, float yScale, float zScale){
			port = _port;
			
			if( comModeIn == 0 ){
				comMode = ABB_TCP_STRING;
			}else{
				comMode = ABB_TCP_RAW;						
			}

			if( tcpServer.setup(port, false) ){
				state  = ABB_SERVER_SETUP;
			}else{
				state  = ABB_SERVER_ERROR_BINDING_TO_PORT;
				serverWaitTime = ofGetElapsedTimef() + 5.0;
			}

			bDrawingStarted = false;
			bIsUp			= true;

			xscale = xScale;
			yscale = yScale;
			zscale = zScale;
		}

		void clearHistory(){
			msgHistory.clear();
		}

		void update(){
			if( state == ABB_SERVER_SETUP && tcpServer.getNumClients() > 0 ){
				printf("num clients is %i\n", tcpServer.getNumClients() );
				state = ABB_SERVER_CLIENT_CONNECTED;
			}
			 
			if( state == ABB_SERVER_ERROR_BINDING_TO_PORT && ofGetElapsedTimef() > serverWaitTime ){
				if( tcpServer.setup(port, false) ){
					state = ABB_SERVER_SETUP;
				}else{
					serverWaitTime = ofGetElapsedTimef() + 5.0;				
				}
			}
		}

		string getStateString(){
			switch (state) {
				case ABB_SERVER_WAITING:
					return  "SERVER NOT SETUP";
				case ABB_SERVER_SETUP:
					return  "SETUP WAITING FOR CLIENT CONNECT ON PORT "+ ofToString(port);
				case ABB_SERVER_ERROR_BINDING_TO_PORT:
					return  "UNABLE TO SETUP SERVER ON PORT: " + ofToString(port);
				case ABB_SERVER_CLIENT_CONNECTED:
					return  "CLIENT CONNECTED PLEASE SEND CONTROL MESSAGES";
				default:
					return "OTHER SERVER STATE";
			}
		}

		void setXScale(float newXScale){
			xscale = newXScale;
		}

		void setYScale(float newYScale){
			yscale = newYScale;
		}

		void setZScale(float newZScale){
			zscale = newZScale;
		}

		void moveDown(float x, float y, float zCorrect = 0.0){
			sendCommand("MARK_DWN", x, y, zCorrect);
			bIsUp = false;
		}

		void moveUp(float x, float y){
			sendCommand("MARK__UP", x, y);
			bIsUp = true;
		}

		void startDrawingAtPoint(float x, float y){
			sendCommand("START_PR", x, y);
			bDrawingStarted = true;
			bIsUp = false;
		}

		void endDrawing(){
			sendCommand("END_PROG");
			bDrawingStarted = false;
			bIsUp = true;
		}

		void sendCommand(string cmmd, float x = 0, float y = 0, float z = 0){
			
			//we have to invert for the robot

			int tcpX = yscale - y * yscale;
			int tcpY = xscale - x * xscale;
			int tcpZ = z * zscale;
							
			if( comMode == ABB_TCP_STRING ){

				lastStr = "[";
				lastStr += cmmd + ",";
				lastStr += ofToString(tcpX) + ",";
				lastStr += ofToString(tcpY) + ",";
				lastStr += ofToString(tcpZ) + ",";
				lastStr += ofToString(tcpX + tcpY);
				lastStr += "]";

				if( endChar >= 0 ){
					lastStr.push_back((char)endChar);
				}

				tcpServer.sendToAll(lastStr);

				msgHistory.push_back(lastStr);
				
			}else{
							
				signed short tx = tcpX;
				signed short ty = tcpY;
				signed short tz = tcpZ;
				signed short ck = tx+ty;
						
				memcpy(sendData, cmmd.c_str(), 8);
				memcpy(&sendData[8], &tx, 2);
				memcpy(&sendData[10], &ty, 2);
				memcpy(&sendData[12], &tz, 2);
				memcpy(&sendData[14], &ck, 2);
								
				lastStr = cmmd + "(ASCII) " + ofToString(tx) + "(INT) " + ofToString(ty) + "(INT) " + ofToString(tz) + "(INT) " + ofToString(ck) + "(INT)";
				
				for(int k = 0; k < tcpServer.getNumClients(); k++){
					tcpServer.sendRawBytes(k, sendData, 16);
				}

				msgHistory.push_back(lastStr);
							
			}
		}

		float serverWaitTime;

		char sendData[16];

		vector <string> msgHistory;

		int port;
		serverComMode comMode;

		bool bIsUp;
		bool bDrawingStarted;
		serverState state;

		string lastStr;
		string status;

		ofxTCPServer tcpServer;

		int endChar;

		float xscale;
		float yscale;
		float zscale;
};
