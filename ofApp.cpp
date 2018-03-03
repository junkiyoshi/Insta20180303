#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofBackground(0);
	ofSetWindowTitle("Insta");

	this->cap.open(1);
	this->cap_size = cv::Size(1280, 720);

	this->image.allocate(this->cap_size.width, this->cap_size.height, OF_IMAGE_COLOR);
	this->frame = cv::Mat(this->image.getHeight(), this->image.getWidth(), CV_MAKETYPE(CV_8UC3, this->image.getPixels().getNumChannels()), this->image.getPixels().getData(), 0);

	this->bgfs = cv::createBackgroundSubtractorKNN();
}

//--------------------------------------------------------------
void ofApp::update() {

	cv::Mat src, fgmask;
	this->cap >> src;
	if (src.empty()){

		return;
	}

	cv::resize(src, this->frame, cv::Size(1280, 720));
	cv::cvtColor(this->frame, this->frame, CV_BGR2RGB);
	cv::flip(this->frame, this->frame, 1);

	if (this->save_frame.empty()) {
		this->frame.copyTo(this->save_frame);
	}

	this->bgfs->apply(this->frame, fgmask, 0.5);

	for (int x = 0; x < this->frame.cols; x++) {

		for (int y = 0; y < this->frame.rows; y++) {

			unsigned char* value = &fgmask.at<unsigned char>(y, x);
			if (*value > 0) {

				this->frame.at<cv::Vec3b>(y, x) = this->save_frame.at<cv::Vec3b>(y, x);
			}
		}
	}

	this->image.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->image.draw(0, 0);
}


//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	if (key = 's') {

		this->frame.copyTo(this->save_frame);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(1280, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}