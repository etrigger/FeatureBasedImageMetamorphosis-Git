/*
 * File Name : CMetamorphosis.cpp
 *
 * Created by "Jeong Ji-Ung" on 2012. 12. 10. 토.
 * Copyright 2012. All rights reserved.
 */
#include "CMetamorphosis.h"

void compute(float x1, float y1, float &x2, float &y2, float& u, float& v, CvPoint Q1, CvPoint P1, CvPoint Q2, CvPoint P2)
{
	//TODO:: 하나의 라인쌍으로 계산.
	float					QPx = Q1.x - P1.x;
	float					QPy = Q1.y - P1.y;

	float					XPx = x1 - P1.x;
	float					XPy = y1 - P1.y;

	float					QPx2 = Q2.x - P2.x;
	float					QPy2 = Q2.y - P2.y;

	float					dist1 = sqrt(QPx * QPx + QPy * QPy);	// distance |Q - P|
	float					dist2 = sqrt(QPx2 * QPx2 + QPy2 * QPy2); // distance |Q2 - P2|

	float					dot1 = QPx * XPx + QPy * XPy;			// (Q - P) * (X - P)
	float					pdot1 = XPx * (-QPy) + XPy * QPx;		// (X - P) * perpendicular(Q - P)

	u						= dot1 / (dist1 * dist1);
	v						= pdot1 / dist1;

	x2						= P2.x + u * QPx2 + (v * (-QPy2)) / dist2;
	y2						= P2.y + u * QPy2 + (v * QPx2) / dist2;
}


void multipleLineCompute(float _x1, float _y1, float _x2, float _y2, float u, float v, float& _rDSUMx, float& _rDSUMy, float& _rWeightsum, CvPoint _Q1, CvPoint _P1)
{
	//TODO:: 여러 라인쌍으로 가중치 계산.
	float					a = 0.001;
	float					b = 2;
	float					p = 1;

	float					Dx = _x2 - _x1;							// Di = Xi' - Xi
	float					Dy = _y2 - _y1;							// Di = Xi' - Xi
	
	float					QPx = _Q1.x - _P1.x;
	float					QPy = _Q1.y - _P1.y;

	float					dist;
	if( 0.0 <= u && u <= 1.0)
		dist				= abs(v);
	else if( u < 0.0 )
		dist				= sqrt((_x1 - _P1.x) * (_x1 - _P1.x) + (_y1 - _P1.y) * (_y1 - _P1.y));
	else if( u > 1.0 )
		dist				= sqrt((_x1 - _Q1.x) * (_x1 - _Q1.x) + (_y1 - _Q1.y) * (_y1 - _Q1.y));

	float					length = sqrt(QPx * QPx + QPy * QPy);	// length of a line, |Q - P|
	length					= pow(length, p);

	float					weight = pow(length / (a + dist), b);

	_rDSUMx					+= Dx * weight;
	_rDSUMy					+= Dy * weight;

	_rWeightsum				+= weight;
}


void morphing(float _width, float _height, CLineList* _pList1, CLineList* _pList2, const CvArr* _pImage1, CvArr* _pImage2, float _alpha)
{
	// TODO: invers warping
	float					x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	float					u, v;

	CvPoint					P, Q, P2, Q2;

	cvSet(_pImage2, cvScalar(0, 0, 0));

	for( x2 = 0; x2 < _width; x2++ )
		for( y2 = 0; y2 < _height; y2++ )
		{
			float			DSUMx = 0;
			float			DSUMy = 0;
			float			weightsum = 0;

			for( CLineList* temp_A = _pList1, *temp_B = _pList2; 
				temp_A != NULL && temp_B != NULL; temp_A = temp_A->getNext(), temp_B = temp_B->getNext() )
			{
				P			= temp_A->mStartPoint;
				Q			= temp_A->mEndPoint;
				P2			= temp_B->mStartPoint;
				Q2			= temp_B->mEndPoint;

				P.x			= ((1 - _alpha) * P2.x) + (_alpha * P.x);
				P.y			= ((1 - _alpha) * P2.y) + (_alpha * P.y);
				Q.x			= ((1 - _alpha) * Q2.x) + (_alpha * Q.x);
				Q.y			= ((1 - _alpha) * Q2.y) + (_alpha * Q.y);

				compute(x2, y2, x1, y1, u, v, Q2, P2, Q, P);
				multipleLineCompute(x2, y2, x1, y1, u, v, DSUMx, DSUMy, weightsum, Q2, P2);
			}

			x1				= x2 + DSUMx / weightsum;
			y1				= y2 + DSUMy / weightsum;

			if( x1 < 0 || x1 > _width - 1 ) continue;
			if( y1 < 0 || y1 > _height - 1 ) continue;

			CvScalar		f = cvGet2D(_pImage1, y1, x1);
			cvSet2D(_pImage2, y2,  x2, f);
		}
}


void synthesis(const CvArr* _pImageA, const CvArr* _pImageB, IplImage* _pDst, float _alpha)
{
	IplImage*				temp = cvCreateImage(cvGetSize(_pDst), 8, 3);
	cvSet(temp, cvScalar(0, 0, 0)); 
	cvSet(_pDst, cvScalar(0, 0, 0));

	for( float x = 0; x < temp->width; x++ )
		for( float y = 0; y < temp->height; y++ )
		{
			CvScalar		g;
			CvScalar		f_A = cvGet2D(_pImageA, y, x);
			CvScalar		f_B = cvGet2D(_pImageB, y, x);

			for( int k = 0; k < 3; k++ )
				g.val[k]	= ((1.0 - _alpha) * f_A.val[k]) + (_alpha * f_B.val[k]);

			cvSet2D(temp, y, x, g);
		}
		cvSmooth(temp, _pDst, CV_MEDIAN);
}