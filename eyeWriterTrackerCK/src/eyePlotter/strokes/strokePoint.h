/*
 *  strokePoint.h
 *  openFrameworks
 *
 *  Created by theo on 17/08/2009.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
 #pragma once
 #include "ofMain.h"

class strokePoint : public ofPoint{
	public:	
		strokePoint(float _x, float _y, float _t, float _confidence){
			x			= _x;
			y			= _y;
			t			= _t;
			confidence	= _confidence;
			bCurve		= false;
			cx			= 0;
			cy			= 0;
		}

		strokePoint(float _x, float _y, float _t, float _confidence, float _cx, float _cy){
			x			= _x;
			y			= _y;
			t			= _t;
			confidence	= _confidence;
			bCurve		= true;
			cx			= _cx;
			cy			= _cy;
		}
	
		bool bCurve;
		float cx, cy;
	
		float confidence;
		float t;
};
