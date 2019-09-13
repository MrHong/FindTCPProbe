#pragma once

#include <boost\thread.hpp>
#include <opencv2\opencv.hpp>
#include <iostream>
#include <string>

using namespace std;

class CameraHandler
{
public:
	CameraHandler(int deviceID);
	CameraHandler(int deviceID, int width, int height);
	~CameraHandler();
	void ShowCamera();
	void DisposeCamera();
	void SetFilter(bool on);
private:
	static void CallBackExposure(int newValue, void * object);
	void InternalShowCamera();
	int _DeviceID;
	int _Width;
	int _Height;
	cv::VideoCapture* _Capture;
	bool _IsInitialized;
	bool _FilterOn;
	void CannyThreshold(cv::Mat* input, cv::Mat* output);
	string _WindowName;
	int _lowThreshold = 30;
	const int _max_lowThreshold = 100;
	const int _ratio = 15;
	const int _kernel_size = 5;

	int _Exposure = -8;

};

