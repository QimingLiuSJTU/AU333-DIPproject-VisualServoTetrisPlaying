#ifdef VIA_OPENCV

#ifndef USRGAMECONTROLLER_H
#define USRGAMECONTROLLER_H

#include "qtcyberdip.h"

#define WIN_NAME "Frame"

#include <Python.h>

#include <ctime>

//��Ϸ������
class usrGameController
{
private:
	deviceCyberDip* device;
	bool isFirst = true;
	clock_t start_time = clock();
	
//������Ϊ��ʵ����ʾЧ�������ӵ�����
	//���ص��ṹ��
	struct MouseArgs{
		cv::Rect box;
		bool Drawing, Hit;
		// init
		MouseArgs() :Drawing(false), Hit(false)
		{
			box = cv::Rect(0, 0, -1, -1);
		}
	};
	//���ص�����
	friend void  mouseCallback(int event, int x, int y, int flags, void*param);
	MouseArgs argM;
//������Ϊ��ʵ�ֿ�����ʾЧ�������ӵ�����
public:
	//���캯�������б����ĳ�ʼ����Ӧ�ڴ����
	usrGameController(void* qtCD);
	//�������������ձ���������Դ
	~usrGameController();
	//����ͼ������ÿ���յ�ͼ��ʱ�������
	int usrProcessImage(cv::Mat& img);
	void control(char* strategy);
};

//������Ϊ��ʵ����ʾЧ�������ӵ�����
//���ص�����
void  mouseCallback(int event, int x, int y, int flags, void*param);
//������Ϊ��ʵ�ֿ�����ʾЧ�������ӵ�����

#endif
#endif