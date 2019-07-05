#include "usrGameController.h"

#include <Python.h>

#include <stdlib.h>

#include <iostream>

#include <string>

#include <cstring>

#include <ctime> 

using namespace std;

bool delay(int time_total)
{
	int count = 0;
	bool quit = true;
	while (count <= time_total)
	{
		cout << count << endl;
		quit = !quit;
		count++;
	}
	system("cls");
	return quit;
}

#ifdef VIA_OPENCV
//构造与初始化
usrGameController::usrGameController(void* qtCD)
{
	qDebug() << "usrGameController online.";
	device = new deviceCyberDip(qtCD);//设备代理类
	cv::namedWindow(WIN_NAME);
	cv::setMouseCallback(WIN_NAME, mouseCallback, (void*)&(argM));
	//初始化python

	Py_Initialize();
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append('F:\desktop\C++Program\data_struct_homework\x64\Debug')");

}

//析构
usrGameController::~usrGameController()
{
	cv::destroyAllWindows();
	if (device != nullptr)
	{
		delete device;
	}
	qDebug() << "usrGameController offline.";
	/*
	grayImg.release();
	pt.release();
	erodedImg.release();
	twovalueImg.release();
	element.release();
	*/
}


int usrGameController::usrProcessImage(cv::Mat& img)
{
	//clock_t start_time = clock();
	cv::Size imgSize(img.cols, img.rows - UP_CUT);
	if (imgSize.height <= 0 || imgSize.width <= 0)
	{
		qDebug() << "Invalid image. Size:" << imgSize.width << "x" << imgSize.height;
		return -1;
	}

	cv::Mat grayImg, erodedImg, twovalueImg;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::Mat pt = img(cv::Rect(0, UP_CUT, imgSize.width, imgSize.height));;


	/***************************/
	char s[201];
	/***************************/



	cv::imshow(WIN_NAME, pt);
	// cv::imwrite("test_img.jpg", pt);

	int row = pt.rows, col = pt.cols;


	cv::cvtColor(pt, grayImg, CV_RGB2GRAY);
	cv::erode(grayImg, erodedImg, element);

	/********* Find the origin *********/

	cv::threshold(erodedImg, twovalueImg, 120, 255, 3);
	int Fx = 0, Fy = 0, oneBx = 24, oneBy = 22, grPosition = 999;

	for (int i = 0; i < 120; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			if (twovalueImg.at<uchar>(i, j) > 120)
			{
				//qDebug() << "Origin Found!" << endl;
				if (i + j < grPosition)
				{
					grPosition = j + i;
					Fx = i + 228;
					Fy = j + 72;
				}
				pt.at<cv::Vec3b>(i, j)[0] = 0;
				pt.at<cv::Vec3b>(i, j)[1] = 0;
				pt.at<cv::Vec3b>(i, j)[2] = 255;
				pt.at<cv::Vec3b>(i + 1, j)[0] = 0;
				pt.at<cv::Vec3b>(i + 1, j)[1] = 0;
				pt.at<cv::Vec3b>(i + 1, j)[2] = 255;
				pt.at<cv::Vec3b>(i, j + 1)[0] = 0;
				pt.at<cv::Vec3b>(i, j + 1)[1] = 0;
				pt.at<cv::Vec3b>(i, j + 1)[2] = 255;
				pt.at<cv::Vec3b>(i + 1, j + 1)[0] = 0;
				pt.at<cv::Vec3b>(i + 1, j + 1)[1] = 0;
				pt.at<cv::Vec3b>(i + 1, j + 1)[2] = 255;
			}
		}
	}


	/********* Get the game frame *********/

	cv::threshold(erodedImg, twovalueImg, 120, 255, 3);
	int position = 0;
	for (int j = Fy; j < Fy + oneBy * 20; j += oneBy)
	{
		for (int i = Fx + oneBx * 9; i >= Fx; i -= oneBx)
		{
			pt.at<cv::Vec3b>(i, j)[0] = 0;
			pt.at<cv::Vec3b>(i, j)[1] = 0;
			pt.at<cv::Vec3b>(i, j)[2] = 255;
			pt.at<cv::Vec3b>(i + 1, j)[0] = 0;
			pt.at<cv::Vec3b>(i + 1, j)[1] = 0;
			pt.at<cv::Vec3b>(i + 1, j)[2] = 255;
			pt.at<cv::Vec3b>(i, j + 1)[0] = 0;
			pt.at<cv::Vec3b>(i, j + 1)[1] = 0;
			pt.at<cv::Vec3b>(i, j + 1)[2] = 255;
			pt.at<cv::Vec3b>(i + 1, j + 1)[0] = 0;
			pt.at<cv::Vec3b>(i + 1, j + 1)[1] = 0;
			pt.at<cv::Vec3b>(i + 1, j + 1)[2] = 255;
			if (twovalueImg.at<uchar>(i, j) < 127)
				s[position] = '1';
			else
				s[position] = '0';
			s[position + 1] = '\0';
			position += 1;
		}
	}

	//cout << s << endl;

	cv::namedWindow("test opencv setup", CV_WINDOW_AUTOSIZE);
	cv::imshow("test opencv setup", pt);
	clock_t end_time = clock();

	//定义python类型的变量

	PyObject *pModule = NULL;
	PyObject *pFunc = NULL;
	PyObject *pArg = NULL;
	PyObject *result = NULL;
	PyObject *pDict = NULL;

	pModule = PyImport_ImportModule("tetris_python_test");

	if (!pModule)
	{
		cout << "Import Module Failed" << endl;
		system("pause");
	}

	//获取模块字典属性
	pDict = PyModule_GetDict(pModule);


	////直接获取模块中的函数
	pFunc = PyObject_GetAttrString(pModule, "main_zpd");

	// 调用直接获得的函数,并传递参数

	pArg = Py_BuildValue("(s)", s);

	result = PyEval_CallObject(pFunc, pArg);

	char* strategy = NULL;

	PyArg_Parse(result, "s", &strategy);
	
	//cout << strategy << endl;

	

	double endtime = double(end_time - start_time) / CLOCKS_PER_SEC;
	if ((endtime > 2 || isFirst ) && strcmp(strategy, "00000000"))
	{
		
		control(strategy);
		start_time = clock();
		isFirst = false;
	}
	//cout << strategy << endl;
	if (strcmp(strategy, "00000000"))
	{
		//cout << s << endl;
		cout << strategy << endl;
	}



	//Py_Finalize();

	//qDebug() << s;
	/*
	grayImg.release();
	pt.release();
	erodedImg.release();
	twovalueImg.release();
	element.release();
	*/
	img.release();

	return 0;
}



void usrGameController::control(char* strategy)
{
	/*************连接机械臂时，起始点为四个按钮的中央*************/
	int length = 13; //中央到各个点要移动的距离
	float delay_time = 600; //每次操作的延时
	float delay_time1 = 600;
	for (int i = 0; i < 8; i++)
	{
		//cout << int(strategy[i] - '0') << endl;
		switch (int(strategy[i] - '0'))
		{
		case 4: //快速下降，按下键，还未使用
			//cout << "444444" << endl;
			device->comMoveTo(-length, 0);
			if (delay_zpd(delay_time));
			device->comHitOnce();
			if (delay_zpd(delay_time));
			device->comHitOnce();
			//if (delay_zpd(delay_time));
			return;
		case 3: //翻转，按上键
			device->comMoveTo(length, 0);
			if (delay_zpd(delay_time1));
			device->comHitOnce();
			if (delay_zpd(delay_time));
			break;
		case 2: //往右，按右键
			//cout << "22222222" << endl;
			device->comMoveTo(0, length);
			if (delay_zpd(delay_time1));
			device->comHitOnce();
			if (delay_zpd(delay_time));
			break;
		case 1: //往左，按左键
			device->comMoveTo(0, -length);
			if (delay_zpd(delay_time1));
			device->comHitOnce();
			if (delay_zpd(delay_time));
			break;
		default: return;
		}
	}
}

//鼠标回调函数
void mouseCallback(int event, int x, int y, int flags, void*param)
{
	usrGameController::MouseArgs* m_arg = (usrGameController::MouseArgs*)param;
	switch (event)
	{
	case CV_EVENT_MOUSEMOVE: // 鼠标移动时
	{
		if (m_arg->Drawing)
		{
			m_arg->box.width = x - m_arg->box.x;
			m_arg->box.height = y - m_arg->box.y;
		}
	}
	break;
	case CV_EVENT_LBUTTONDOWN:case CV_EVENT_RBUTTONDOWN: // 左/右键按下
	{
		m_arg->Hit = event == CV_EVENT_RBUTTONDOWN;
		m_arg->Drawing = true;
		m_arg->box = cvRect(x, y, 0, 0);
	}
	break;
	case CV_EVENT_LBUTTONUP:case CV_EVENT_RBUTTONUP: // 左/右键弹起
	{
		m_arg->Hit = false;
		m_arg->Drawing = false;
		if (m_arg->box.width < 0)
		{
			m_arg->box.x += m_arg->box.width;
			m_arg->box.width *= -1;
		}
		if (m_arg->box.height < 0)
		{
			m_arg->box.y += m_arg->box.height;
			m_arg->box.height *= -1;
		}
	}
	break;
	}
}
#endif