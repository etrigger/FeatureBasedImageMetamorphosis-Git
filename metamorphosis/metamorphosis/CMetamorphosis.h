/*
 * File Name : CMetamorphosis.h
 *
 * Created by "Jeong Ji-Ung" on 2012. 12. 10. 토.
 * Copyright 2012. All rights reserved.
 */
#pragma once
#include "CLineList.h"

//단일 라인 계산.
void						compute(float x1, float y1, float &x2, float &y2, float& u, float& v, CvPoint Q1, CvPoint P1, CvPoint Q2, CvPoint P2);

//다중 라인 계산.(라인들과의 가중치 계산)
void						multipleLineCompute(float _x1, float _y1, float _x2, float _y2, float u, float v, float& _rDSUMx, float& _rDSUMy, float& _rWeightsum, CvPoint _Q1, CvPoint _P1);

// 이미지 변형.
void						morphing(float _width, float _height, CLineList* _pList1, CLineList* _pList2, const CvArr* _pImage1, CvArr* _pImage2, float _alpha);

// 이미지 합성.
void						synthesis(const CvArr* _pImageA, const CvArr* _pImageB, IplImage* _pDst, float _alpha);