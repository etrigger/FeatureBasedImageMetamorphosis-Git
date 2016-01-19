/*
 * File Name : CLineList.cpp
 *
 * Created by "Jeong Ji-Ung" on 2012. 11. 30. 토.
 * Copyright 2012. All rights reserved.
 */
#include "CLineList.h"

/*
 * CLineList Class Member function
 */
CLineList::CLineList()
{
	this->mStartPoint		= cvPoint(0, 0);
	this->mEndPoint			= cvPoint(0, 0);
	this->mPrev				= NULL;
	this->mNext				= NULL;
}


CLineList::CLineList(float _sx, float _sy, float _ex, float _ey)
{
	this->mStartPoint		= cvPoint(_sx, _sy);
	this->mEndPoint			= cvPoint(_ex, _ey);
	this->mPrev				= NULL;
	this->mNext				= NULL;
}


CLineList::CLineList(CvPoint _st, CvPoint _ed)
{
	this->mStartPoint		= _st;
	this->mEndPoint			= _ed;
	this->mPrev				= NULL;
	this->mNext				= NULL;
}


CLineList::~CLineList()
{
	this->destroy();
}


void CLineList::destroy()
{
	this->mPrev				= NULL;
	this->mNext				= NULL;
}


void CLineList::setPrev(CLineList* _pPrev)
{
	this->mPrev				= _pPrev;
}


void CLineList::setNext(CLineList* _pNext)
{
	this->mNext				= _pNext;
}


CLineList* CLineList::getPrev()
{
	return this->mPrev;
}


CLineList* CLineList::getNext()
{
	return this->mNext;
}


/*
 * CLineListManager Class Member function
 */
CLineListManager::CLineListManager()
{
	mpLine					= NULL;
}


CLineListManager::~CLineListManager()
{
	this->clear();
}


void CLineListManager::entry(float _sx, float _sy, float _ex, float _ey)
{
	if( this->mpLine == NULL )
		this->mpLine		= new CLineList(_sx, _sy, _ex, _ey);
	else
	{
		CLineList*			pTemp = NULL;
		CLineList*			pPrev = NULL;
		for( pTemp = this->mpLine; pTemp != NULL; pPrev = pTemp, pTemp = pTemp->getNext() );
		pTemp				= new CLineList(_sx, _sy, _ex, _ey);
		pTemp->setPrev(pPrev);
		pPrev->setNext(pTemp);
	}
}


void CLineListManager::entry(CvPoint _st, CvPoint _ed)
{
	if( this->mpLine == NULL )
		this->mpLine		= new CLineList(_st, _ed);
	else
	{
		CLineList*			pTemp = NULL;
		CLineList*			pPrev = NULL;
		for( pTemp = this->mpLine; pTemp != NULL; pPrev = pTemp, pTemp = pTemp->getNext() );
		pTemp				= new CLineList(_st, _ed);
		pTemp->setPrev(pPrev);
		pPrev->setNext(pTemp);
	}
}


void CLineListManager::clear()
{
	if( this->mpLine != NULL )
	{
		CLineList			*pTemp, *pTemp2;
		for( pTemp = this->mpLine; pTemp != NULL; )
		{
			pTemp2			= pTemp->getNext();
			delete pTemp;
			pTemp			= pTemp2;
		}
		mpLine				= NULL;
	}
}


CLineList* CLineListManager::getListHeader()
{
	// TODO: 리스트의 루트를 반환.
	return this->mpLine;
}


void CLineListManager::drawLine(CvArr* _pImg)
{
	// TODO: 리스트의 모든 라인을 그림.
	for( CLineList* temp = this->mpLine; temp != NULL; temp = temp->getNext() )
	{
		cvLine(_pImg, temp->mStartPoint, temp->mEndPoint, cvScalar(255, 255, 255));
		cvCircle(_pImg, temp->mStartPoint, 3, cvScalar(0, 0, 255), -1);
		cvCircle(_pImg, temp->mEndPoint, 3, cvScalar(0, 0, 255), -1);
	}
}