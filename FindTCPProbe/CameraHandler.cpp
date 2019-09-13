#include "CameraHandler.h"

void CameraHandler::CallBackExposure(int newValue, void * object) {
	CameraHandler* handler = (CameraHandler*)object;
	handler->_Exposure = 0 - newValue;
	handler->_Capture->set(cv::CAP_PROP_EXPOSURE, handler->_Exposure);
	cout << "Exposure of the video : " << handler->_Exposure << endl;
}

CameraHandler::CameraHandler(int deviceID, int width, int height)
{
	_DeviceID = deviceID;
	_Width = width;
	_Height = height;
	_Capture = nullptr;
	_FilterOn = false;
}

CameraHandler::CameraHandler(int deviceID) : CameraHandler(deviceID, 800, 600)
{
}

void CameraHandler::SetFilter(bool on) {
	_FilterOn = on;
}

void CameraHandler::CannyThreshold(cv::Mat* input, cv::Mat* output)
{

	cv::Mat blurred;
	cv::Mat detectEdges;

	cv::blur(*input, blurred, cv::Size(5, 5));
	cv::Canny(blurred, detectEdges, _lowThreshold, _lowThreshold*_ratio, _kernel_size);
	*output = cv::Scalar::all(0);
	input->copyTo(*output, detectEdges);
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
		double dExposure = cap->get(cv::CAP_PROP_EXPOSURE);
		cout << "Exposure of the video : " << dExposure << endl;

		_Exposure = -8;

		cap->set(cv::CAP_PROP_FRAME_WIDTH, dWidth);
		cap->set(cv::CAP_PROP_FRAME_HEIGHT, dHeight);
		cap->set(cv::CAP_PROP_EXPOSURE, _Exposure);

		cout << "Resolution of the video : " << dWidth << " x " << dHeight << endl;

		_WindowName = "TCPProbe " + to_string(_DeviceID);
		cv::namedWindow(_WindowName);
		cv::createTrackbar("Exposure:", _WindowName, &_Exposure, _max_lowThreshold, &CameraHandler::CallBackExposure, this);
		cv::setTrackbarMin("Exposure:", _WindowName, 0);
		cv::setTrackbarMax("Exposure:", _WindowName, 20);
		cv::createTrackbar("Min Threshold:", _WindowName, &_lowThreshold, _max_lowThreshold);

		_IsInitialized = true;
		_Capture = cap;
	}

	if (_Capture != nullptr) {
		cv::Mat frame;
		cv::Mat frameDisplay;

		bool bSuccess = _Capture->read(frame); // read a new frame from video 

		if (_FilterOn) {
			CannyThreshold(&frame, &frameDisplay);
		}
		else {
			frame.copyTo(frameDisplay);
		}

		//Breaking the while loop if the frames cannot be captured
		if (bSuccess == false)
		{
			char xc;
			cout << "Video camera is disconnected " << _DeviceID << endl;
			_Capture = nullptr;
			return;
		}

		//show the frame in the created window
		imshow(_WindowName, frameDisplay);
	}

}

void CameraHandler::ShowCamera() {
	InternalShowCamera();
}

void CameraHandler::DisposeCamera() {
	if (_Capture) _Capture->release();
	_Capture = nullptr;
}

CameraHandler::~CameraHandler()
{
}
