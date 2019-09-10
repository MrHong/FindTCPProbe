#include "CameraHandler.h"


CameraHandler::CameraHandler(int deviceID, int width, int height)
{
	_DeviceID = deviceID;
	_Width = width;
	_Height = height;
	_Capture = nullptr;
}

CameraHandler::CameraHandler(int deviceID) : CameraHandler(deviceID, 800, 600)
{
}

void CameraHandler::InternalShowCamera() {
	if (!_IsInitialized) {
		cv::VideoCapture* cap = new cv::VideoCapture(_DeviceID);

		// if not success, exit program
		if (cap->isOpened() == false)
		{
			cout << "Cannot open the video camera" << endl;
			_IsInitialized = true;
			return;
		}

		double dWidth = _Width;
		double dHeight = _Height;

		cap->set(cv::CAP_PROP_FRAME_WIDTH, dWidth); // valueX = your wanted width cap.set(cv::CAP_PROP_FRAME_HEIGHT, valueY); // valueY = your wanted heigth
		cap->set(cv::CAP_PROP_FRAME_HEIGHT, dHeight); // valueX = your wanted width cap.set(cv::CAP_PROP_FRAME_HEIGHT, valueY); // valueY = your wanted heigth

		cout << "Resolution of the video : " << dWidth << " x " << dHeight << endl;

		_WindowName = "TCPProbe " + to_string(_DeviceID);
		cv::namedWindow(_WindowName);
		
		_IsInitialized = true;
		_Capture = cap;
	}

	if (_Capture != nullptr) {
		cv::Mat frame;
		bool bSuccess = _Capture->read(frame); // read a new frame from video 

		//Breaking the while loop if the frames cannot be captured
		if (bSuccess == false)
		{
			char xc;
			cout << "Video camera is disconnected " << _DeviceID << endl;
			_Capture = nullptr;
			return;
		}

		//show the frame in the created window
		imshow(_WindowName, frame);
	}

}

void CameraHandler::ShowCamera() {
	InternalShowCamera();
}

void CameraHandler::DisposeCamera() {
	if(_Capture) _Capture->release();
	_Capture = nullptr;
}

CameraHandler::~CameraHandler()
{
}
