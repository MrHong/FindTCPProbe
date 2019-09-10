#include "pch.h"
#include "EnumerateCameras.h"


EnumerateCameras::EnumerateCameras()
{
	_DeviceID = 0;
}

/** \brief Test all grabbing drivers and fills a vector of all available cameras CAPdrivers+ID
 *
 * For each CAPdrivers+ID, opens the device. If success, push CAP+ID in \c camIdx
 * A grabbing test is done just to inform the user.
 * \param camIdx[out] a vector of all readable cameras CAP+ID
 * \note remove some cout to use as function
 */
bool EnumerateCameras::GetCameras(std::vector<int> &camIdx)
{
	camIdx.clear();
	struct CapDriver {
		int enumValue; string enumName; string comment;
	};
	// list of all CAP drivers (see highgui_c.h)
	vector<CapDriver> drivers;
	drivers.push_back({ CAP_VFW, "CV_CAP_VFW", "platform native" });
	drivers.push_back({ CAP_FIREWARE, "CV_CAP_FIREWARE", "IEEE 1394 drivers" });
	drivers.push_back({ CAP_QT, "CV_CAP_QT", "QuickTime" });
	drivers.push_back({ CAP_UNICAP, "CV_CAP_UNICAP", "Unicap drivers" });
	drivers.push_back({ CAP_DSHOW, "CV_CAP_DSHOW", "DirectShow (via videoInput)" });
	drivers.push_back({ CAP_MSMF, "CV_CAP_MSMF", "Microsoft Media Foundation (via videoInput)" });
	drivers.push_back({ CAP_PVAPI, "CV_CAP_PVAPI", "PvAPI, Prosilica GigE SDK" });
	drivers.push_back({ CAP_OPENNI, "CV_CAP_OPENNI", "OpenNI (for Kinect)" });
	drivers.push_back({ CAP_OPENNI_ASUS, "CV_CAP_OPENNI_ASUS", "OpenNI (for Asus Xtion)" });
	drivers.push_back({ CAP_ANDROID, "CV_CAP_ANDROID", "Android" });
	drivers.push_back({ CAP_XIAPI, "CV_CAP_XIAPI", "XIMEA Camera API" });
	drivers.push_back({ CAP_AVFOUNDATION, "CV_CAP_AVFOUNDATION", "AVFoundation framework for iOS" });
	drivers.push_back({ CAP_GIGANETIX, "CV_CAP_GIGANETIX", "Smartek Giganetix GigEVisionSDK" });
	drivers.push_back({ CAP_INTELPERC, "CV_CAP_INTELPERC", "Intel Perceptual Computing SDK" });

	std::string winName, driverName, driverComment;
	int driverEnum;
	Mat frame;
	bool found;
	std::cout << endl << "Searching for cameras IDs..." << endl;
	for (int drv = 0; drv < drivers.size(); drv++)
	{
		driverName = drivers[drv].enumName;
		driverEnum = drivers[drv].enumValue;
		driverComment = drivers[drv].comment;
		std::cout << endl << "Testing driver " << driverName << "...";
		found = false;

		int maxID = 100; //100 IDs between drivers
		if (driverEnum == CAP_VFW)
			maxID = 10; //VWF opens same camera after 10 ?!?
		else if (driverEnum == CAP_ANDROID)
			maxID = 98; //98 and 99 are front and back cam

		for (int idx = 0; idx < maxID; idx++)
		{
			VideoCapture cap(driverEnum + idx);  // open the camera
			if (cap.isOpened())                  // check if we succeeded
			{
				found = true;
				camIdx.push_back(driverEnum + idx);  // vector of all available cameras
				cap >> frame;
				if (frame.empty())
					std::cout << endl << driverName << "+" << idx << "[" << driverEnum + idx << "]" << "\t opens: OK \t grabs: FAIL";
				else
					std::cout << endl << driverName << "+" << idx << "[" << driverEnum + idx << "]" << "\t opens: OK \t grabs: OK";
			}
			cap.release();
		}
		if (!found) cout << "Nothing !" << endl;
		cout << endl;
	}
	cout << camIdx.size() << " camera IDs has been found ";

	return (camIdx.size() > 0); // returns success
}

EnumerateCameras::~EnumerateCameras()
{
}
