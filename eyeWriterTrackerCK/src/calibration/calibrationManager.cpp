#include "calibrationManager.h"

//--------------------------------------------------------------
void calibrationManager::setup(){

	calibrationInfo.loadImage("images/calibrationInfo.png");
	font.loadFont("fonts/HelveticaNeueMed.ttf", 32);

	nDivisionsWidth		= 4;
	nDivisionsHeight	= 3;

	nPosition = 0;
	pos  = 0;

	inputEnergy = 0;			// we need it?

	bAutomatic = false;
	bPreAutomatic = false;
	bAmInAutodrive = false;
	bShowAutoRecording = false;
	
	startTime = ofGetElapsedTimef();
	preTimePerDot = 10.0;
	recordTimePerDot = 0.4;
	postTimePerDot = 0.1;
	totalTimePerDot = preTimePerDot + recordTimePerDot;
	bInAutoRecording = false;
	autoPct = 0;

	calibrationRectangle.x = 0;
	calibrationRectangle.y = 0;
	calibrationRectangle.width = 1024;
	calibrationRectangle.height = 768;

	setupControlPanel();
	panel.hide();
	
	fitter.setup(nDivisionsWidth, nDivisionsHeight);
	
	int total = nDivisionsWidth * nDivisionsHeight;
	randomOrder.assign(total, int());
	for(int k = 0; k < randomOrder.size(); k++){
		randomOrder[k] = k;
	}

	smoothing = 1.0f;
	menuEnergy = 1;
	
	bPreAutomatic = false;

}

//--------------------------------------------------------------
void calibrationManager::start(){

	bAutomatic = true;
	bAmInAutodrive = true;
	startTime = ofGetElapsedTimef();
	menuEnergy = 1;	
	
	random_shuffle(randomOrder.begin(), randomOrder.end());
	
	fitter.startCalibration();
}

//--------------------------------------------------------------
void calibrationManager::stop(){
	
	bAutomatic = false;
	bPreAutomatic = false;
	bAmInAutodrive = false;
	startTime = ofGetElapsedTimef();
	nPosition = 0;
	pos  = 0;
	
}

//--------------------------------------------------------------
void calibrationManager::update(){
	
	panel.hidden?ofHideCursor():ofShowCursor();
	
	
	// TODO: rewrite the code to control calibration mode.
	// bAuto--, bPre--- are not friendly to understand how it works.

	calibrationRectangle.x = 0;
	calibrationRectangle.y = 0;
	calibrationRectangle.width = ofGetWidth();
	calibrationRectangle.height = ofGetHeight();

	float widthPad = ofGetWidth() * 0.025f;
	float heightPad = ofGetHeight() * 0.025f;

	calibrationRectangle.x += widthPad;
	calibrationRectangle.y += heightPad;
	calibrationRectangle.width -= widthPad*2;
	calibrationRectangle.height -= heightPad*2;
	
	updateControlPanel();

	// TODO:make it possible to change nDivisions?
//	nDivisionsWidth = panel.getValueI("N_DIV_W");
//	nDivisionsHeight = panel.getValueI("N_DIV_H");;

	fitter.removeOutliers();
	
	totalTimePerDot = preTimePerDot + recordTimePerDot;

	if ((bAutomatic == true && bAmInAutodrive == true) || bPreAutomatic){
		menuEnergy = 0.94f * menuEnergy + 0.06f * 0.0f;
	} else {
		menuEnergy = 0.94f * menuEnergy + 0.06f * 1.0f;
	}
	
	bShowAutoRecording = bInAutoRecording;
	
	// do the auto:
	if (bAutomatic == true && bAmInAutodrive == true){
		int nPts = nDivisionsWidth * nDivisionsHeight;
		float totalTime = totalTimePerDot * nPts;
		
		if (ofGetElapsedTimef() - startTime > totalTime){		// calibration finish.
			bAmInAutodrive = false;
			bInAutoRecording = false;
			bPreAutomatic = false;
			fitter.calculate(calibrationRectangle);
			bShowAutoRecording = bInAutoRecording;
		} else {

			float diffTime = ofGetElapsedTimef() - startTime ;
			pt = (int)(diffTime / totalTimePerDot);
			float diffDotTime = diffTime - pt * totalTimePerDot;
			// cout << diffTime << " " << pt <<  " " << diffDotTime << endl;
			if (diffDotTime < preTimePerDot){

				autoPct = (diffDotTime / preTimePerDot);
				bInAutoRecording = false;
				bShowAutoRecording = bInAutoRecording;

			} else {
				autoPct = (diffDotTime - preTimePerDot) / recordTimePerDot;
				
				if( diffDotTime > (totalTimePerDot-postTimePerDot) ){
					bInAutoRecording = false;
					
					//here we say still show it even though we are not recording
					//trick people!!
					bShowAutoRecording = true;
				}else{
					bInAutoRecording = true;
					bShowAutoRecording = bInAutoRecording;
				}
			}
			pos = pt;
		}
	}
						//TODO: this works but pointFitter is expecting the points in order. 
	int randPos = pos ;	//randomOrder[pos];

	inputEnergy *= 0.98f;

	int xx = (randPos % nDivisionsWidth);
	int yy = (randPos / nDivisionsWidth);

	bool bEven = false;
	if (yy % 2 == 0)  bEven = true;

	xp = bEven ? calibrationRectangle.x + ((float)calibrationRectangle.width / (float)(nDivisionsWidth-1)) * xx :
				 calibrationRectangle.x + (calibrationRectangle.width - ((float)calibrationRectangle.width / (float)(nDivisionsWidth-1)) * xx);

	yp = calibrationRectangle.y + calibrationRectangle.height - ((float)calibrationRectangle.height / (float)(nDivisionsHeight-1)) * yy;
	
	fitter.update(randPos, xp, yp);		// raw data is comming from testApp directly to fitter.
}

//--------------------------------------------------------------
void calibrationManager::draw(){

	// draw a light grid:

	if (!bPreAutomatic || !bAutomatic) panel.draw();

	ofEnableAlphaBlending();
	ofSetColor(30, 30, 30, (int) (255 - 255.0 * menuEnergy));
	ofRect(0,0,ofGetWidth(), ofGetHeight());

//	ofSetColor(255, 255, 255, 40);
//	for (int i = 0; i < nDivisionsWidth; i++){
//		float xLine = calibrationRectangle.x + ((float)calibrationRectangle.width / (float)(nDivisionsWidth-1)) * i;
//		ofLine(xLine, calibrationRectangle.y,xLine, calibrationRectangle.y + calibrationRectangle.height);
//	}
//
//	for (int i = 0; i < nDivisionsHeight; i++){
//		float yLine = calibrationRectangle.y + calibrationRectangle.height - ((float)calibrationRectangle.height / (float)(nDivisionsHeight-1)) * i;
//		ofLine(calibrationRectangle.x,yLine, calibrationRectangle.x + calibrationRectangle.width,yLine);
//	}

	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------
	if (fitter.bBeenFit != true){

		ofEnableAlphaBlending();

		float rad = 16;
		glLineWidth(4);
		ofSetColor(255, 255, 255, 100);
		ofLine (xp, yp-rad, xp, yp+rad);
		ofLine (xp-rad, yp, xp+rad, yp);;


		ofSetColor(255, 255, 255, 255);

		if (bAutomatic == true && bAmInAutodrive == true && bInAutoRecording){
		} else {
			ofCircle(xp, yp, rad*0.33);
		}

		ofFill();

		//
		// TODO: do some animation stuff here:
		if (bAutomatic == true && bAmInAutodrive == true){

			if (bShowAutoRecording){

				if( bInAutoRecording ){
					ofSetColor(255, 0, 0, 200);
				}else{
					ofSetColor(180, 70, 30, 200);
				}
				ofCircle(xp, yp, 14);

				ofSetColor(255, 255,255);
				ofCircle(xp, yp, 5);

			} else {
				ofNoFill();
				ofSetColor(255, 255, 255, 150);
				ofCircle(xp, yp, 200 - 200* autoPct);
				ofFill();
			}
		}

		glLineWidth(1);
		ofFill();

		if (!bAutomatic)
		if (inputEnergy > 0){
			ofSetColor(255, 0, 127, (int) (60*inputEnergy));
			ofCircle(xp, yp, rad * 3.5 );

			ofSetColor(255, 0, 127, (int) (150*inputEnergy));
			ofCircle(xp, yp, rad );
		}

		ofSetColor(255, 255, 255);
		//calibrationInfo.draw(100,100);
	}
	
	//-----------------------------------------------------------------------
	//-----------------------------------------------------------------------

	if (!bAutomatic && !bPreAutomatic){

//		if (!fitter.bBeenFit)	{
//			ofSetColor(255, 0, 0,127);
//			font.drawString("not calibrated", 100,100);
//		} else {
//			ofSetColor(0, 255, 0, 127);
//			font.drawString("calibrated", 100,100);
//		}
	}

	if (bPreAutomatic == true){
		ofSetColor(255, 255, 255);
		calibrationInfo.draw(100,100);
//		cout << bPreAutomatic << endl;
	}

	fitter.draw();
}

//--------------------------------------------------------------
void calibrationManager::clear(){
	pos = 0;
	fitter.clear();
}

//--------------------------------------------------------------
void calibrationManager::advancePosition(){
	pos++;
	pos %= (nDivisionsWidth*nDivisionsHeight);
	inputEnergy = 0;
}

//--------------------------------------------------------------
void calibrationManager::mouseDragged(int x, int y, int button){
	panel.mouseDragged(x,y,button);
}

//--------------------------------------------------------------
void calibrationManager::mousePressed(int x, int y, int button){
	panel.mousePressed(x,y,button);
}

//--------------------------------------------------------------
void calibrationManager::mouseReleased(int x, int y, int button){
	panel.mouseReleased();
}

//--------------------------------------------------------------
void calibrationManager::keyPressed(int key) {
	if (bPreAutomatic == true){
		if (key == ' '){
			bPreAutomatic = false;
			clear();
			stop();
			start();
		}
	}else if (bPreAutomatic == false && !bAmInAutodrive) { 
		if (key == ' '){
			bPreAutomatic = true;
		}
	}else if (key == ' '){
		clear();
		stop();
	}
	
	if(key == 'D' ){
		panel.toggleView();
	}
}

//--------------------------------------------------------------

