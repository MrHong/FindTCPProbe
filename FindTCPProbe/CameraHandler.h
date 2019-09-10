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
private:
	void InternalShowCamera();
	int _DeviceID;
	int _Width;
	int _Height;
	cv::VideoCapture* _Capture;
	bool _IsInitialized;
	string _WindowName;

};

