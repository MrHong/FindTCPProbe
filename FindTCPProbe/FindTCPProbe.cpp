// FindTCPProbe.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "EnumerateCameras.h"
#include "CameraHandler.h"
#include <boost\program_options.hpp>
#include <algorithm>
#include <iterator>

using namespace std;
using namespace cv;
namespace po = boost::program_options;

int main(int argc, const char *argv[])
{

	po::options_description desc("Allowed options");
	desc.add_options()
		("help", "this help message")
		("list", "find and list all cameras")
		("cam", po::value<vector<int>>()->multitoken()->
			zero_tokens()->composing(), "set camera device ID(s)")
		("Width", po::value<int>()->default_value(800), "Camera Resolution X")
		("Height", po::value<int>()->default_value(600), "Camera Resolution Y")
		;

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);


	if (vm.count("help"))
		std::cout << desc << '\n';
	else if (vm.count("list")) {

		std::vector<int> Cameras;
		EnumerateCameras ec;

		if (ec.GetCameras(Cameras)) {
			std::cout << "Cameras found!";
		}
		else {
			std::cout << "NO Cameras found!";
		}
	}
	else if (vm.count("cam")) {
		int count = vm.count("cam");
		vector<int> camids = vm["cam"].as<vector<int>>();
		vector<CameraHandler*> cameras;

		int reswidth = vm["Width"].as<int>();
		int resheight = vm["Height"].as<int>();

		cout << "Press ESC to stop the program!" << endl;

		for (auto camdeviceid : camids) {
			cameras.push_back(new CameraHandler(camdeviceid, reswidth, resheight));
		}

		while (true) {
			for (auto camera : cameras) {
				camera->ShowCamera();
			}

			if (waitKey(15) == 27) {
				cout << "Esc key is pressed by user. Stoppig the video" << endl;
				break;
			}

		}
	}





}
