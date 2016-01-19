/*
 * File Name : CLineList.h
 *
 * Created by "Jeong Ji-Ung" on 2012. 11. 30. 토.
 * Copyright 2012. All rights reserved.
 */
#pragma once
#include <opencv2\opencv.hpp>


/*
 * CLASS NAME : CLineList
 * CREATED BY "JEONG JI-UNG"
 * TODO : 원본 이미지에 만들어지는 선을 관리하는 클레스. 연결리스트 기반.
 */
class CLineList
{
public:
	CLineList();
	CLineList(float _sx, float _sy, float _ex, float _ey);
	CLineList(CvPoint _st, CvPoint _ed);
	~CLineList();

public:
	CvPoint					mStartPoint;
	CvPoint					mEndPoint;

private:
	CLineList*				mNext;
	CLineList*				mPrev;

public:
	void					destroy();
	void					setPrev(CLineList* _pPrev);
	void					setNext(CLineList* _pNext);
	
	CLineList*				getPrev();
	CLineList*				getNext();
};


/*
 * CLASS NAME : CLineListManager
 * CREATED BY "JEONG JI-UNG"
 * TODO : CLineList를 관리하는 매니저 클레스. 원본 이미지에 선을 그리고 리스트 생성, 제거하는 등의 일을 함.
 */
class CLineListManager
{
public:
	CLineListManager();
	~CLineListManager();

public:
	CLineList*				mpLine;

public:
	void					entry(float _sx, float _sy, float _ex, float _ey);			// 리스트 추가
	void					entry(CvPoint _st, CvPoint _ed);
	void					clear();													// 모든 리스트 제거
	
	CLineList*				getListHeader();											// 리스트의 헤더를 반환.

	void					drawLine(CvArr* _pImg);										// 리스트에 추가되어 있는 선을 그림.
};