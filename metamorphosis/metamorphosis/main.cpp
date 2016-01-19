/*
* File Name : main.cpp
*
* Created by "Jeong Ji-Ung" on 2012. 11. 30. 토.
* Copyright 2012. All rights reserved.
*/
#include "CMetamorphosis.h"

bool						gbEnable;								// 이미지 모핑 과정을 시작하면 true, 아니면 false
float						gAlpha;									// 이미지의 변형 과정을 순차적으로 표현하기 위한 가중치 값. (1 - alpha)

IplImage					*gSrc_A, *gSrc_B;						// 원본 A, 원본 B
IplImage					*gImage_A, *gImage_B;					// 이미지A, 이미지B
IplImage					*gMorp_A, *gMorp_B;						// 변형된 이미지.
IplImage*					gDst;									// 변형된 A 와 변형된 B의 합본.

CvSize						gSize_A, gSize_B;						// 원본 이미지의 사이즈

CLineListManager			gLine_A, gLine_B;

CvPoint						gSt, gEd;								// 마우스 시작 좌표, 마우스 끝 좌표


void onMouse_A(int event, int x, int y, int flags, void* param)
{
	// TODO: 이미지 A에 선을 그리기 위해 좌표를 지정.
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		gSt = cvPoint(x, y);
		gEd = cvPoint(x, y);
	}
	else if (event == CV_EVENT_MOUSEMOVE && flags == CV_EVENT_FLAG_LBUTTON)
	{
		gEd = cvPoint(x, y);
	}
	else if (event == CV_EVENT_LBUTTONUP)
	{
		gEd = cvPoint(x, y);
		gLine_A.entry(gSt, gEd);				// 리스트에 데이터 추가

		// 마우스 좌표 초기화
		gSt = cvPoint(-10, -10);
		gEd = cvPoint(-10, -10);
	}

	// gSrc_A에서 원본 이미지 복사.
	cvCopy(gSrc_A, gImage_A);

	// 현재 그려진 선을 그림.
	cvLine(gImage_A, gSt, gEd, cvScalar(255, 255, 255));
	cvCircle(gImage_A, gSt, 3, cvScalar(0, 0, 255), -1);
	cvCircle(gImage_A, gEd, 3, cvScalar(0, 0, 255), -1);

	gLine_A.drawLine(gImage_A);

	cvShowImage("A", gImage_A);
}


void onMouse_B(int event, int x, int y, int flags, void* param)
{
	// TODO: 이미지 B에 선을 그리기 위해 좌표를 지정.
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		gSt = cvPoint(x, y);
		gEd = cvPoint(x, y);
	}
	else if (event == CV_EVENT_MOUSEMOVE && flags == CV_EVENT_FLAG_LBUTTON)
	{
		gEd = cvPoint(x, y);
	}
	else if (event == CV_EVENT_LBUTTONUP)
	{
		gEd = cvPoint(x, y);
		gLine_B.entry(gSt, gEd);				// 리스트에 데이터 추가

		// 마우스 좌표 초기화
		gSt = cvPoint(-10, -10);
		gEd = cvPoint(-10, -10);
	}

	// gSrc_B에서 원본 이미지 복사.
	cvCopy(gSrc_B, gImage_B);

	// 현재 그려진 선을 그림.
	cvLine(gImage_B, gSt, gEd, cvScalar(255, 255, 255));
	cvCircle(gImage_B, gSt, 3, cvScalar(0, 0, 255), -1);
	cvCircle(gImage_B, gEd, 3, cvScalar(0, 0, 255), -1);

	gLine_B.drawLine(gImage_B);

	cvShowImage("B", gImage_B);
}


void metamorphosis()
{
	if (gLine_A.getListHeader() == NULL || gLine_B.getListHeader() == NULL)
		return;

	// TODO: 이곳에서 변형이 일어남.
	morphing(gImage_A->width, gImage_A->height, gLine_A.getListHeader(), gLine_B.getListHeader(), gSrc_A, gMorp_A, gAlpha);
	//gLine_B.drawLine(gMorp_A);
	morphing(gImage_B->width, gImage_B->height, gLine_B.getListHeader(), gLine_A.getListHeader(), gSrc_B, gMorp_B, (1 - gAlpha));
	//gLine_A.drawLine(gMorp_B);

	synthesis(gMorp_A, gMorp_B, gDst, gAlpha);
}


void initImage(char* _pInputA, char* _pInputB)
{
	// 이미지 로드.
	gSrc_A = cvLoadImage(_pInputA);
	gSrc_B = cvLoadImage(_pInputB);

	// 로드한 이미지가 존재하지 않거나, 잘못 로드되었을 경우 프로그램 종료.
	if (gSrc_A == NULL || gSrc_B == NULL)
		return;

	// 원본 이미지 A와 B의 사이즈를 얻어옴.
	gSize_A = cvGetSize(gSrc_A);
	gSize_B = cvGetSize(gSrc_B);

	// 화면에 보여 줄 이미지 A와 B를 원본 A와 B의 사이즈로 생성.
	gImage_A = cvCreateImage(gSize_A, 8, 3);
	gImage_B = cvCreateImage(gSize_B, 8, 3);

	// 변형되는 과정을 보여줄 IplImage의 객체를 원본 A와 B의 사이즈로 생성.
	gMorp_A = cvCreateImage(gSize_A, 8, 3);
	gMorp_B = cvCreateImage(gSize_B, 8, 3);

	// A와 B의 변형 과정이 합성되어 최종적 결과를 보여주는 이미지. 원본A의 사이즈로 초기화.
	gDst = cvCreateImage(gSize_A, 8, 3);

	// 처음 생성되는 색상을 검정색으로 초기화.
	cvSet(gMorp_A, cvScalar(0, 0, 0));
	cvSet(gMorp_B, cvScalar(0, 0, 0));
	cvSet(gDst, cvScalar(0, 0, 0));

	// 원본A, B를 화면에 보여줄 이미지 A, B에 복사.
	cvCopy(gSrc_A, gImage_A);
	cvCopy(gSrc_B, gImage_B);
}


void release()
{
	gLine_A.clear();
	gLine_B.clear();

	cvReleaseImage(&gSrc_A);
	cvReleaseImage(&gSrc_B);
	cvReleaseImage(&gImage_A);
	cvReleaseImage(&gImage_B);
	cvReleaseImage(&gDst);
}


void main()
{
	char					inputA[128];
	char					inputB[128];

	printf("이미지 A 경로 입력 : ");
	scanf("%s", &inputA);
	printf("이미지 B 경로 입력 : ");
	scanf("%s", &inputB);

	initImage(inputA, inputB);

	gbEnable = false;
	gAlpha = 0;

	CvVideoWriter*			writer = cvCreateVideoWriter("metamorphosis.avi", 0, 10, cvSize(gDst->width, gDst->height), 1);

	gSt = cvPoint(-10, -10);
	gEd = cvPoint(-10, -10);

	while (true)
	{
		int					key;

		cvShowImage("A", gImage_A);
		cvShowImage("B", gImage_B);

		cvSetMouseCallback("A", onMouse_A);
		cvSetMouseCallback("B", onMouse_B);

		if (gbEnable == false)
		{
			key = cvWaitKey();

			if (key == 32)
				gbEnable = true;
			else if (key == 96)
			{
				printf("이미지 A 경로 입력 : ");
				scanf("%s", &inputA);
				printf("이미지 B 경로 입력 : ");
				scanf("%s", &inputB);

				release();
				initImage(inputA, inputB);
			}
			else
				break;
		}
		else if (gbEnable == true)
		{
			metamorphosis();

			cvShowImage("Morp A", gMorp_A);
			cvShowImage("Morp B", gMorp_B);
			cvShowImage("dst", gDst);

			cvWriteFrame(writer, gDst);

			cvWaitKey(10);

			gAlpha += 0.05;
			if (gAlpha > 1.0)
			{
				gbEnable = false;
				gAlpha = 0;
			}
		}
	}

	release();

	cvReleaseVideoWriter(&writer);
}