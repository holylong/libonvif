#include "H264_2_RGB.h"
//#include "base64.h"

/*opencv??*/
#include <opencv2/opencv.hpp>
//#include"opencv2/face.hpp"
//#include"opencv2/face/facerec.hpp"
//#include"opencv2/objdetect.hpp"
//#include"opencv2/core/base.hpp"
//#include"opencv2/xfeatures2d.hpp"


#include "RtspStreamAPI.h"
#include "onvif_interface.h"
#include "onvif_comm.h"

#ifdef _WIN32
#pragma comment(lib, "UsageEnvironment.lib")
#pragma comment(lib, "liveMedia.lib")
#pragma comment(lib, "groupsock.lib")
#pragma comment(lib, "BasicUsageEnvironment.lib")
#pragma comment(lib, "libonvif.lib")
#pragma comment(lib, "ws2_32.lib")
#endif

extern "C"
{
	#include "libavcodec/avcodec.h"
	#include "libavdevice/avdevice.h"
	#include "libavformat/avformat.h"
	#include "libavfilter/avfilter.h"
	#include "libavutil/avutil.h"
	#include "libavutil/time.h"
	#include "libswscale/swscale.h"
	#include "libavutil/pixdesc.h"
}

#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avdevice.lib")
#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avutil.lib")
#pragma comment(lib, "avfilter.lib")
#pragma comment(lib, "swscale.lib")
#pragma comment(lib, "swresample.lib")
// #pragma comment(lib, "postproc.lib")

using namespace std;
using namespace cv;

// Forward function definitions:
char eventLoopWatchVariable = 0;

#if 0
IplImage * MyResizeImage(IplImage* pSrc, double dScale)
{
	CvSize nSize;
	nSize.width = pSrc->width*dScale;
	nSize.height = pSrc->height*dScale;
	IplImage* pDes = cvCreateImage(nSize, pSrc->depth, pSrc->nChannels);
	cvResize(pSrc, pDes, CV_INTER_CUBIC);
	cvReleaseImage(&pSrc);
	return pDes;
}
#else
#endif

string trimSpace(string s)
{
	if (s.empty())
	{
		return s;
	}

	//cout << s << endl;
	string::size_type i = 0, j = 0;
	while (i < s.size())
	{
		j = s.find_first_of(" ", i);
		if (j > s.size())
			break;
		s.erase(j, 1);
		i++;
	}
	return s;
}

void cb_discovery(char* DeviceXAddr)
{
	int stmno = 0;                                                              
	int profile_cnt = 0;                                                        
	struct tagProfile *profiles = NULL;                                         
	struct tagCapabilities capa;                                                

	char uri[MY_ONVIF_ADDRESS_SIZE] = { 0 };                                      
	char uri_auth[MY_ONVIF_TOKEN_SIZE + 50] = { 0 };                              

	MYONVIF_GetCapabilities(DeviceXAddr, &capa);                                  

	profile_cnt = MYONVIF_GetProfiles(capa.MediaXAddr, &profiles);                

	if (profile_cnt > stmno) {
		MYONVIF_GetStreamUri(capa.MediaXAddr, profiles[stmno].token, uri, sizeof(uri)); 

		my_make_uri_withauth(uri, USERNAME, PASSWORD, uri_auth, sizeof(uri_auth));

		//open_rtsp(uri_auth);                                                    
		// Begin by setting up our usage environment:
		TaskScheduler* scheduler = BasicTaskScheduler::createNew();
		UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);
		H264_Init();
		openURL(*env, "Rtsp", uri_auth);
		// All subsequent activity takes place within the event loop:
		env->taskScheduler().doEventLoop(&eventLoopWatchVariable);
	}

	if (NULL != profiles) {
		free(profiles);
		profiles = NULL;
	}
}

int main(int argc, char** argv) 
{
	MYONVIF_DetectDevice(cb_discovery);
	cout << "hello world" << endl;
	getchar();

	// This function call does not return, unless, at some point in time, "eventLoopWatchVariable" gets set to something non-zero.

	return 0;
}
