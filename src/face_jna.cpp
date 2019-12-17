//#include <jni.h>
//#include <android/bitmap.h>
//#include <android/log.h>
#include <string>
#include <vector>
#include <cstring>
#include<sstream>

#include "net.h"
#include "mat.h"
#include "detect.h"
#include "recognize.h"

using namespace Face;
using namespace std;
#define TAG "DetectSo"
//#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG,__VA_ARGS__)
static Detect *mDetect;
static Recognize *mRecognize;

//a flag to judge sdk's initial status
bool detection_sdk_init_ok = false;

string feastr;

extern "C"  {
	__declspec(dllexport) bool FaceModelInit(const char* faceDetectionModelPath_) {
		
		string ModelPath = faceDetectionModelPath_;
		//string faceDetectionModelPath_ = "weights";
		bool tRet = false;
		
		mDetect = new Detect(ModelPath);
		mRecognize = new Recognize(ModelPath);
		mDetect->SetThreadNum(2);
		mRecognize->SetThreadNum(2);
		
		detection_sdk_init_ok = true;
		tRet = true;
		cout << "Inital" << endl;
		return tRet;
	}
	
	
	__declspec(dllexport) int* FaceDetect(unsigned char* imageDate, int imageWidth,
                                             int imageHeight, int imageChannel) {
		//LOGD();
		cout << "start detect face from JNA" << endl;
		if (!detection_sdk_init_ok) {
			
			cout << "the MTCNN model has not been initialed，return NULL" << endl;
			return NULL;
		}

		unsigned char *faceImageCharDate = (unsigned char *) imageDate;
		/*for(int i=0;i<100;i++){
				cout << (int)faceImageCharDate[i] << endl;;
		}*/
		
		ncnn::Mat ncnn_img;
		if (imageChannel == 3) {
			ncnn_img = ncnn::Mat::from_pixels(faceImageCharDate, ncnn::Mat::PIXEL_BGR2RGB,
											  imageWidth, imageHeight);
		} else {
			ncnn_img = ncnn::Mat::from_pixels(faceImageCharDate, ncnn::Mat::PIXEL_RGBA2RGB, imageWidth,
											  imageHeight);
		}
		std::vector<Bbox> finalBbox;
		mDetect->start(ncnn_img, finalBbox);

		int32_t num_face = static_cast<int32_t>(finalBbox.size());
		//LOGD("face num：%d\n", num_face);
		cout << "face num: " << num_face << endl;
		int out_size = 1 + num_face * 14;
		
		cout << "finish detection, return face information" << endl;
		int *faceInfo = new int[out_size];
		faceInfo[0] = num_face;
		for (int i = 0; i < num_face; i++) {
			faceInfo[14 * i + 1] = finalBbox[i].x1;//left
			faceInfo[14 * i + 2] = finalBbox[i].y1;//top
			faceInfo[14 * i + 3] = finalBbox[i].x2;//right
			faceInfo[14 * i + 4] = finalBbox[i].y2;//bottom
			for (int j = 0; j < 10; j++) {
				faceInfo[14 * i + 5 + j] = static_cast<int>(finalBbox[i].ppoint[j]);
			}
		}

		return faceInfo;
	}
	
	__declspec(dllexport) char* FaceFeature(unsigned char* faceDate, int w,
                                             int h) {
		
		unsigned char *faceCharDate = (unsigned char *) faceDate;
		ncnn::Mat ncnn_img = ncnn::Mat::from_pixels_resize(faceCharDate, ncnn::Mat::PIXEL_RGBA2RGB, w, h,112,112);
		std::vector<float> feature;
		mRecognize->start(ncnn_img, feature);

		feastr = "";
		for (int i = 0;i<feature.size();++i) {
			std::ostringstream oss;
			oss<<feature[i];
			std::string str(oss.str());
			feastr += str;
			if(i<feature.size()-1){
				feastr += " ";
			}

		 }
		//res += '\0';
		char* res = (char*)feastr.c_str();
		return res;
	}

	__declspec(dllexport) void Test() {
		cout << "test" << endl;
	}
}



