#pragma once

#include <vector>
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;


class EnumerateCameras
{
public:
	EnumerateCameras();
	~EnumerateCameras();

	bool GetCameras(std::vector<int> &camIdx);

private:
	int _DeviceID;
};

