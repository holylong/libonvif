/**********
This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 3 of the License, or (at your
option) any later version. (See <http://www.gnu.org/copyleft/lesser.html>.)

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
**********/
// Copyright (c) 1996-2018, Live Networks, Inc.  All rights reserved
// A demo application, showing how to create and run a RTSP client (that can potentially receive multiple streams concurrently).
//
// NOTE: This code - although it builds a running application - is intended only to illustrate how to develop your own RTSP
// client application.  For a full-featured RTSP client application - with much more functionality, and many options - see
// "openRTSP": http://www.live555.com/openRTSP/

#include "liveMedia.hh"
#include "BasicUsageEnvironment.hh"
#include "UsageEnvironment.hh"
#include "Boolean.hh"

#include "H264_2_RGB.h"

//#include "soapH.h"
//#include "stdsoap2.h"
//#include "soapStub.h"
//#include "base64.h"

#include <stdio.h>
#include <stdlib.h>

#include <opencv2/opencv.hpp>
//#include"opencv2/face.hpp"
//#include"opencv2/face/facerec.hpp"
#include"opencv2/objdetect.hpp"
#include"opencv2/core/base.hpp"
//#include"opencv2/xfeatures2d.hpp"

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

//Release
//#pragma comment(lib, "opencv_world3416")

using namespace std;
using namespace cv;

// Forward function definitions:

// RTSP 'response handlers':
void continueAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString);
void continueAfterSETUP(RTSPClient* rtspClient, int resultCode, char* resultString);
void continueAfterPLAY(RTSPClient* rtspClient, int resultCode, char* resultString);

// Other event handler functions:
void subsessionAfterPlaying(void* clientData); // called when a stream's subsession (e.g., audio or video substream) ends
void subsessionByeHandler(void* clientData); // called when a RTCP "BYE" is received for a subsession
void streamTimerHandler(void* clientData);
// called at the end of a stream's expected duration (if the stream has not already signaled its end using a RTCP "BYE")

// The main streaming routine (for each "rtsp://" URL):
void openURL(UsageEnvironment& env, char const* progName, char const* rtspURL);

// Used to iterate through each stream's 'subsessions', setting up each one:
void setupNextSubsession(RTSPClient* rtspClient);

// Used to shut down and close a stream (including its "RTSPClient" object):
void shutdownStream(RTSPClient* rtspClient, int exitCode = 1);

// A function that outputs a string that identifies each stream (for debugging output).  Modify this if you wish:
UsageEnvironment& operator<<(UsageEnvironment& env, const RTSPClient& rtspClient) {
	return env << "[URL:\"" << rtspClient.url() << "\"]: ";
}

// A function that outputs a string that identifies each subsession (for debugging output).  Modify this if you wish:
UsageEnvironment& operator<<(UsageEnvironment& env, const MediaSubsession& subsession) {
	return env << subsession.mediumName() << "/" << subsession.codecName();
}

void usage(UsageEnvironment& env, char const* progName) {
	env << "Usage: " << progName << " <rtsp-url-1> ... <rtsp-url-N>\n";
	env << "\t(where each <rtsp-url-i> is a \"rtsp://\" URL)\n";
}

char eventLoopWatchVariable = 0;

#if 0
std::string RtspGetUrl(char* code)
{
	struct soap soap1;
	char* str = "";
	wchar_t* str_1 = NULL;
	int b = 0;
	_ns1__startGuiResponse resultStr;
	_ns1__startGui a;
	soap_init(&soap1);
	soap_set_mode(&soap1, SOAP_C_MBSTRING);
	a.param = code;//ConvAnsiToUtf8("18600960941");
	soap_call___ns1__startGui(&soap1, "http://13.40.31.73:8080/GetRTSPURL/services/StartGuiImpl", "", &a, resultStr);
	std::string strOut;
	if (soap1.error)
	{
		soap_print_fault(&soap1, stderr);
	}
	else
	{
		str = resultStr.startGuiReturn;
		char *bindata;
		size_t inlen = 0;
		unsigned char *out = (unsigned char*)malloc(strlen(str) + 1);
		memset(out, '\0', strlen(str) + 1);
		unsigned long outlen = 0;
		//过滤掉换行符和空格

		int i = CBase64::DecodeBase64((const unsigned char*)str, out, strlen(str) + 1);
		//std::cout << str << std::endl;
		//std::cout << "-----------------------------------------------------------" << std::endl;

		strOut = (const char*)out;
		//std::cout << strOut.c_str() << std::endl;
	}
	soap_destroy(&soap1);
	soap_end(&soap1);
	soap_done(&soap1);
	return strOut;
}
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

int OpenRtsp(const char* exeName, const char* rtspUtl)
{
	// Begin by setting up our usage environment:
	TaskScheduler* scheduler = BasicTaskScheduler::createNew();
	UsageEnvironment* env = BasicUsageEnvironment::createNew(*scheduler);
	H264_Init();
	openURL(*env, exeName, rtspUtl);
	env->taskScheduler().doEventLoop(&eventLoopWatchVariable);

	printf("hello end\n");
	Sleep(100000);
	return 0;
}

int main(int argc, char** argv) {

	if (argc > 1)
	{
		OpenRtsp(argv[0], argv[1]);
	}
	else {
		OpenRtsp(argv[1], "rtsp://127.0.0.1/Titanic.mkv");
	}
	// There are argc-1 URLs: argv[1] through argv[argc-1].  Open and start streaming each one:
	//for (int i = 1; i <= argc - 1; ++i) {
	//	//通过urlcode获取rtsp地址
	//	//string strUrl = RtspGetUrl(argv[1]);
	//	//openURL(*env, argv[0], strUrl.c_str());
	//	openURL(*env, argv[0], argv[1]);
	//}

	// All subsequent activity takes place within the event loop:
	
	// This function call does not return, unless, at some point in time, "eventLoopWatchVariable" gets set to something non-zero.

	return 0;

	// If you choose to continue the application past this point (i.e., if you comment out the "return 0;" statement above),
	// and if you don't intend to do anything more with the "TaskScheduler" and "UsageEnvironment" objects,
	// then you can also reclaim the (small) memory used by these objects by uncommenting the following code:
}

// Define a class to hold per-stream state that we maintain throughout each stream's lifetime:

class StreamClientState {
public:
	StreamClientState();
	virtual ~StreamClientState();

public:
	MediaSubsessionIterator* iter;
	MediaSession* session;
	MediaSubsession* subsession;
	TaskToken streamTimerTask;
	double duration;
};

// If you're streaming just a single stream (i.e., just from a single URL, once), then you can define and use just a single
// "StreamClientState" structure, as a global variable in your application.  However, because - in this demo application - we're
// showing how to play multiple streams, concurrently, we can't do that.  Instead, we have to have a separate "StreamClientState"
// structure for each "RTSPClient".  To do this, we subclass "RTSPClient", and add a "StreamClientState" field to the subclass:

class ourRTSPClient : public RTSPClient {
public:
	static ourRTSPClient* createNew(UsageEnvironment& env, char const* rtspURL,
		int verbosityLevel = 0,
		char const* applicationName = NULL,
		portNumBits tunnelOverHTTPPortNum = 0);

protected:
	ourRTSPClient(UsageEnvironment& env, char const* rtspURL,
		int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum);
	// called only by createNew();
	virtual ~ourRTSPClient();

public:
	StreamClientState scs;
};

// Define a data sink (a subclass of "MediaSink") to receive the data for each subsession (i.e., each audio or video 'substream').
// In practice, this might be a class (or a chain of classes) that decodes and then renders the incoming audio or video.
// Or it might be a "FileSink", for outputting the received data into a file (as is done by the "openRTSP" application).
// In this example code, however, we define a simple 'dummy' sink that receives incoming data, but does nothing with it.

class DummySink : public MediaSink {
public:
	static DummySink* createNew(UsageEnvironment& env,
		MediaSubsession& subsession, // identifies the kind of data that's being received
		char const* streamId = NULL); // identifies the stream itself (optional)

private:
	DummySink(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId);
	// called only by "createNew()"
	virtual ~DummySink();

	static void afterGettingFrame(void* clientData, unsigned frameSize,
		unsigned numTruncatedBytes,
		struct timeval presentationTime,
		unsigned durationInMicroseconds);
	void afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,
		struct timeval presentationTime, unsigned durationInMicroseconds);

private:
	// redefined virtual functions:
	virtual Boolean continuePlaying();

private:
	u_int8_t* fReceiveBuffer;
	u_int8_t*  decBuffer;
	MediaSubsession& fSubsession;
	char* fStreamId;
	bool firstFrame;
	//FileSink *fFileSink;
	//Boolean bFirsttime;
};

#define RTSP_CLIENT_VERBOSITY_LEVEL 1 // by default, print verbose output from each "RTSPClient"

static unsigned rtspClientCount = 0; // Counts how many streams (i.e., "RTSPClient"s) are currently in use.

void openURL(UsageEnvironment& env, char const* progName, char const* rtspURL) {
	// Begin by creating a "RTSPClient" object.  Note that there is a separate "RTSPClient" object for each stream that we wish
	// to receive (even if more than stream uses the same "rtsp://" URL).
	RTSPClient* rtspClient = ourRTSPClient::createNew(env, rtspURL, RTSP_CLIENT_VERBOSITY_LEVEL, progName);
	if (rtspClient == NULL) {
		env << "Failed to create a RTSP client for URL \"" << rtspURL << "\": " << env.getResultMsg() << "\n";
		return;
	}

	++rtspClientCount;

	// Next, send a RTSP "DESCRIBE" command, to get a SDP description for the stream.
	// Note that this command - like all RTSP commands - is sent asynchronously; we do not block, waiting for a response.
	// Instead, the following function call returns immediately, and we handle the RTSP response later, from within the event loop:
	rtspClient->sendDescribeCommand(continueAfterDESCRIBE);
}


// Implementation of the RTSP 'response handlers':

void continueAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString) {
	do {
		UsageEnvironment& env = rtspClient->envir(); // alias
		StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias

		if (resultCode != 0) {
			env << *rtspClient << "Failed to get a SDP description: " << resultString << "\n";
			delete[] resultString;
			break;
		}

		char* const sdpDescription = resultString;
		env << *rtspClient << "Got a SDP description:\n" << sdpDescription << "\n";

		// Create a media session object from this SDP description:
		scs.session = MediaSession::createNew(env, sdpDescription);
		delete[] sdpDescription; // because we don't need it anymore
		if (scs.session == NULL) {
			env << *rtspClient << "Failed to create a MediaSession object from the SDP description: " << env.getResultMsg() << "\n";
			break;
		}
		else if (!scs.session->hasSubsessions()) {
			env << *rtspClient << "This session has no media subsessions (i.e., no \"m=\" lines)\n";
			break;
		}

		// Then, create and set up our data source objects for the session.  We do this by iterating over the session's 'subsessions',
		// calling "MediaSubsession::initiate()", and then sending a RTSP "SETUP" command, on each one.
		// (Each 'subsession' will have its own data source.)
		scs.iter = new MediaSubsessionIterator(*scs.session);
		setupNextSubsession(rtspClient);
		return;
	} while (0);

	// An unrecoverable error occurred with this stream.
	shutdownStream(rtspClient);
}

// By default, we request that the server stream its data using RTP/UDP.
// If, instead, you want to request that the server stream via RTP-over-TCP, change the following to True:
#define REQUEST_STREAMING_OVER_TCP false

void setupNextSubsession(RTSPClient* rtspClient) 
{
	UsageEnvironment& env = rtspClient->envir(); // alias
	StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias

	scs.subsession = scs.iter->next();
	if (scs.subsession != NULL) 
	{
		if (!scs.subsession->initiate()) 
		{
			env << *rtspClient << "Failed to initiate the \"" << *scs.subsession << "\" subsession: " << env.getResultMsg() << "\n";
			setupNextSubsession(rtspClient); // give up on this subsession; go to the next one
		}
		else 
		{
			env << *rtspClient << "Initiated the \"" << *scs.subsession << "\" subsession (";
			if (scs.subsession->rtcpIsMuxed()) 
			{
				env << "client port " << scs.subsession->clientPortNum();
			}
			else 
			{
				env << "client ports " << scs.subsession->clientPortNum() << "-" << scs.subsession->clientPortNum() + 1;
			}
			env << ")\n";

			// Continue setting up this subsession, by sending a RTSP "SETUP" command:
			rtspClient->sendSetupCommand(*scs.subsession, continueAfterSETUP, false, REQUEST_STREAMING_OVER_TCP);
		}
		return;
	}

	// We've finished setting up all of the subsessions.  Now, send a RTSP "PLAY" command to start the streaming:
	if (scs.session->absStartTime() != NULL) 
	{
		// Special case: The stream is indexed by 'absolute' time, so send an appropriate "PLAY" command:
		rtspClient->sendPlayCommand(*scs.session, continueAfterPLAY, scs.session->absStartTime(), scs.session->absEndTime());
	}
	else 
	{
		scs.duration = scs.session->playEndTime() - scs.session->playStartTime();
		rtspClient->sendPlayCommand(*scs.session, continueAfterPLAY);
	}
}

void continueAfterSETUP(RTSPClient* rtspClient, int resultCode, char* resultString) 
{
	do {
		UsageEnvironment& env = rtspClient->envir(); // alias
		StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias

		if (resultCode != 0) 
		{
			env << *rtspClient << "Failed to set up the \"" << *scs.subsession << "\" subsession: " << resultString << "\n";
			break;
		}

		env << *rtspClient << "Set up the \"" << *scs.subsession << "\" subsession (";
		if (scs.subsession->rtcpIsMuxed()) 
		{
			env << "client port " << scs.subsession->clientPortNum();
		}
		else 
		{
			env << "client ports " << scs.subsession->clientPortNum() << "-" << scs.subsession->clientPortNum() + 1;
		}
		env << ")\n";

		// Having successfully setup the subsession, create a data sink for it, and call "startPlaying()" on it.
		// (This will prepare the data sink to receive data; the actual flow of data from the client won't start happening until later,
		// after we've sent a RTSP "PLAY" command.)

		scs.subsession->sink = DummySink::createNew(env, *scs.subsession, rtspClient->url());
		// perhaps use your own custom "MediaSink" subclass instead
		if (scs.subsession->sink == NULL) 
		{
			env << *rtspClient << "Failed to create a data sink for the \"" << *scs.subsession
				<< "\" subsession: " << env.getResultMsg() << "\n";
			break;
		}

		env << *rtspClient << "Created a data sink for the \"" << *scs.subsession << "\" subsession\n";
		scs.subsession->miscPtr = rtspClient; // a hack to let subsession handler functions get the "RTSPClient" from the subsession 
		scs.subsession->sink->startPlaying(*(scs.subsession->readSource()),
			subsessionAfterPlaying, scs.subsession);
		// Also set a handler to be called if a RTCP "BYE" arrives for this subsession:
		if (scs.subsession->rtcpInstance() != NULL) 
		{
			scs.subsession->rtcpInstance()->setByeHandler(subsessionByeHandler, scs.subsession);
		}
	} while (0);
	delete[] resultString;

	// Set up the next subsession, if any:
	setupNextSubsession(rtspClient);
}

void continueAfterPLAY(RTSPClient* rtspClient, int resultCode, char* resultString) 
{
	Boolean success = false;

	do {
		UsageEnvironment& env = rtspClient->envir(); // alias
		StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias

		if (resultCode != 0) 
		{
			env << *rtspClient << "Failed to start playing session: " << resultString << "\n";
			break;
		}

		// Set a timer to be handled at the end of the stream's expected duration (if the stream does not already signal its end
		// using a RTCP "BYE").  This is optional.  If, instead, you want to keep the stream active - e.g., so you can later
		// 'seek' back within it and do another RTSP "PLAY" - then you can omit this code.
		// (Alternatively, if you don't want to receive the entire stream, you could set this timer for some shorter value.)
		if (scs.duration > 0) 
		{
			unsigned const delaySlop = 2; // number of seconds extra to delay, after the stream's expected duration.  (This is optional.)
			scs.duration += delaySlop;
			unsigned uSecsToDelay = (unsigned)(scs.duration * 1000000);
			scs.streamTimerTask = env.taskScheduler().scheduleDelayedTask(uSecsToDelay, (TaskFunc*)streamTimerHandler, rtspClient);
		}

		env << *rtspClient << "Started playing session";
		if (scs.duration > 0) 
		{
			env << " (for up to " << scs.duration << " seconds)";
		}
		env << "...\n";

		success = true;
	} while (0);
	delete[] resultString;

	if (!success) 
	{
		// An unrecoverable error occurred with this stream.
		shutdownStream(rtspClient);
	}
}


// Implementation of the other event handlers:

void subsessionAfterPlaying(void* clientData) 
{
	MediaSubsession* subsession = (MediaSubsession*)clientData;
	RTSPClient* rtspClient = (RTSPClient*)(subsession->miscPtr);

	// Begin by closing this subsession's stream:
	Medium::close(subsession->sink);
	subsession->sink = NULL;

	// Next, check whether *all* subsessions' streams have now been closed:
	MediaSession& session = subsession->parentSession();
	MediaSubsessionIterator iter(session);
	while ((subsession = iter.next()) != NULL) 
	{
		if (subsession->sink != NULL) return; // this subsession is still active
	}

	// All subsessions' streams have now been closed, so shutdown the client:
	shutdownStream(rtspClient);
}

void subsessionByeHandler(void* clientData) 
{
	MediaSubsession* subsession = (MediaSubsession*)clientData;
	RTSPClient* rtspClient = (RTSPClient*)subsession->miscPtr;
	UsageEnvironment& env = rtspClient->envir(); // alias

	env << *rtspClient << "Received RTCP \"BYE\" on \"" << *subsession << "\" subsession\n";

	// Now act as if the subsession had closed:
	subsessionAfterPlaying(subsession);
}

void streamTimerHandler(void* clientData) 
{
	ourRTSPClient* rtspClient = (ourRTSPClient*)clientData;
	StreamClientState& scs = rtspClient->scs; // alias

	scs.streamTimerTask = NULL;

	// Shut down the stream:
	shutdownStream(rtspClient);
}

void shutdownStream(RTSPClient* rtspClient, int exitCode) 
{
	UsageEnvironment& env = rtspClient->envir(); // alias
	StreamClientState& scs = ((ourRTSPClient*)rtspClient)->scs; // alias

	// First, check whether any subsessions have still to be closed:
	if (scs.session != NULL) 
	{
		Boolean someSubsessionsWereActive = false;
		MediaSubsessionIterator iter(*scs.session);
		MediaSubsession* subsession;

		while ((subsession = iter.next()) != NULL) 
		{
			if (subsession->sink != NULL) 
			{
				Medium::close(subsession->sink);
				subsession->sink = NULL;

				if (subsession->rtcpInstance() != NULL) 
				{
					subsession->rtcpInstance()->setByeHandler(NULL, NULL); // in case the server sends a RTCP "BYE" while handling "TEARDOWN"
				}

				someSubsessionsWereActive = true;
			}
		}

		if (someSubsessionsWereActive) 
		{
			// Send a RTSP "TEARDOWN" command, to tell the server to shutdown the stream.
			// Don't bother handling the response to the "TEARDOWN".
			rtspClient->sendTeardownCommand(*scs.session, NULL);
		}
	}

	env << *rtspClient << "Closing the stream.\n";
	Medium::close(rtspClient);
	// Note that this will also cause this stream's "StreamClientState" structure to get reclaimed.

	if (--rtspClientCount == 0) 
	{
		// The final stream has ended, so exit the application now.
		// (Of course, if you're embedding this code into your own application, you might want to comment this out,
		// and replace it with "eventLoopWatchVariable = 1;", so that we leave the LIVE555 event loop, and continue running "main()".)
		exit(exitCode);
		//return;
	}
}


// Implementation of "ourRTSPClient":

ourRTSPClient* ourRTSPClient::createNew(UsageEnvironment& env, char const* rtspURL,
	int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum) 
{
	return new ourRTSPClient(env, rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum);
}

ourRTSPClient::ourRTSPClient(UsageEnvironment& env, char const* rtspURL,
	int verbosityLevel, char const* applicationName, portNumBits tunnelOverHTTPPortNum)
	: RTSPClient(env, rtspURL, verbosityLevel, applicationName, tunnelOverHTTPPortNum, -1) 
{
}

ourRTSPClient::~ourRTSPClient() {
}


// Implementation of "StreamClientState":

StreamClientState::StreamClientState()
	: iter(NULL), session(NULL), subsession(NULL), streamTimerTask(NULL), duration(0.0) {
}

StreamClientState::~StreamClientState() {
	delete iter;
	if (session != NULL) {
		// We also need to delete "session", and unschedule "streamTimerTask" (if set)
		UsageEnvironment& env = session->envir(); // alias

		env.taskScheduler().unscheduleDelayedTask(streamTimerTask);
		Medium::close(session);
	}
}


// Implementation of "DummySink":

// Even though we're not going to be doing anything with the incoming data, we still need to receive it.
// Define the size of the buffer that we'll use:
#define DUMMY_SINK_RECEIVE_BUFFER_SIZE 100000

DummySink* DummySink::createNew(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId) {
	return new DummySink(env, subsession, streamId);
}

DummySink::DummySink(UsageEnvironment& env, MediaSubsession& subsession, char const* streamId)
	: MediaSink(env),firstFrame(true),
	fSubsession(subsession) 
{
	//fFileSink = NULL;
	fStreamId = strDup(streamId);
	fReceiveBuffer = new u_int8_t[DUMMY_SINK_RECEIVE_BUFFER_SIZE];
	decBuffer = new u_int8_t[DUMMY_SINK_RECEIVE_BUFFER_SIZE];
	//unsigned int num = 1;
	//SPropRecord * sps = parseSPropParameterSets(fSubsession.fmtp_spropparametersets(), num);
	//const char* outFileName = "hello.h264";
	//if (strcmp(fSubsession.mediumName(), "video") == 0&&strcmp(fSubsession.codecName(), "H264") == 0)
	//	fFileSink = H264VideoFileSink::createNew(env, outFileName, fSubsession.fmtp_spropparametersets());
	//struct timeval tv = { 0,0 };
	//unsigned char start_code[4] = { 0x00, 0x00, 0x00, 0x01 };
	//fileSink->addData(start_code, 4, tv);
	//fileSink->addData(sps[0].sPropBytes, sps[0].sPropLength, tv);
	//delete[] sps;
}

DummySink::~DummySink() {
	delete[] fReceiveBuffer;
	delete[] fStreamId;
	H264_Release();
}

void DummySink::afterGettingFrame(void* clientData, unsigned frameSize, unsigned numTruncatedBytes,
	struct timeval presentationTime, unsigned durationInMicroseconds) {
	DummySink* sink = (DummySink*)clientData;
	sink->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime, durationInMicroseconds);
}

// If you don't want to see debugging output for each received frame, then comment out the following line:
#define DEBUG_PRINT_EACH_RECEIVED_FRAME 1

FILE *fp = NULL;
int nSize = 0;

void DummySink::afterGettingFrame(unsigned frameSize, unsigned numTruncatedBytes,	struct timeval presentationTime, unsigned durationInMicroseconds) {
	// We've just received a frame of data.  (Optionally) print out information about it:
#ifdef DEBUG_PRINT_EACH_RECEIVED_FRAME
	if (fStreamId != NULL) envir() << "Stream \"" << fStreamId << "\"; ";
	envir() << fSubsession.mediumName() << "/" << fSubsession.codecName() << ":\tReceived " << frameSize << " bytes";
	if (numTruncatedBytes > 0) envir() << " (with " << numTruncatedBytes << " bytes truncated)";
	char uSecsStr[6 + 1]; // used to output the 'microseconds' part of the presentation time
	sprintf(uSecsStr, "%06u", (unsigned)presentationTime.tv_usec);
	envir() << ".\tPresentation time: " << (int)presentationTime.tv_sec << "." << uSecsStr;

	if (fSubsession.rtpSource() != NULL && !fSubsession.rtpSource()->hasBeenSynchronizedUsingRTCP()) {
		envir() << "!"; // mark the debugging output to indicate that this presentation time is not RTCP-synchronized
	}
#ifdef DEBUG_PRINT_NPT
	envir() << "\tNPT: " << fSubsession.getNormalPlayTime(presentationTime);
#endif
	envir() << "\n";
#endif

	// Then continue, to request the next frame of data:
	nSize = frameSize;
	cout << "guck -------------------------------====================================------------------------" << nSize << endl;
#if 0
	if (fp == NULL)
		fp = fopen("1.h264", "wb");
#endif
	//fwrite(fReceiveBuffer, 1, nSize, fp);
	//printf("%s", fReceiveBuffer);
	unsigned char* RgbBuf = NULL;
	unsigned int RgbSize = 0;
	int RgbWidth = 0;
	int RgbHeight = 0;
#if 0
	if(fp != NULL)
	{
		fwrite(fReceiveBuffer, frameSize, 1, fp);
		fflush(fp);
	}
#endif
#if 1
	memset(decBuffer, 0, DUMMY_SINK_RECEIVE_BUFFER_SIZE);
	int nTotalLen = 0;
	unsigned char start_code[4] = { 0x00, 0x00, 0x00, 0x01 };
	if (strcmp(fSubsession.mediumName(), "video") == 0 && strcmp(fSubsession.codecName(), "H264") == 0)
	{

			if (firstFrame)
			{
				unsigned numSPropRecords;
				//if (j == 0)
				//if (fReceiveBuffer[0] == 0x65 || fReceiveBuffer[0] == 0x25 || fReceiveBuffer[0] == 0x68 || fReceiveBuffer[0] == 0x67)
				{
					SPropRecord* sPropRecords = parseSPropParameterSets(fSubsession.fmtp_spropparametersets(), numSPropRecords);
					for (unsigned i = 0; i < numSPropRecords; ++i)
					{
						memcpy(decBuffer + nTotalLen, start_code, 4);
						nTotalLen += 4;
						memcpy(decBuffer + nTotalLen, sPropRecords[i].sPropBytes, sPropRecords[i].sPropLength);
						nTotalLen += sPropRecords[i].sPropLength;
						printf("==>%d\n", sPropRecords[i].sPropLength);
					}
					delete[] sPropRecords;
				}
			}
			memcpy(decBuffer + nTotalLen, start_code, 4);
			nTotalLen += 4;
			memcpy(decBuffer + nTotalLen, fReceiveBuffer, frameSize);
			nTotalLen += frameSize;
			printf("mvp=========================================%d\n", nTotalLen);
			H264_2_RGB(decBuffer, nTotalLen, RgbBuf, &RgbSize, &RgbWidth, &RgbHeight);
	}

#endif

#if 0
	if (fFileSink != NULL)
	{
		unsigned char const start_code[4] = { 0x00, 0x00, 0x00, 0x01 };

		if (!bFirsttime)
		{
			// If we have NAL units encoded in "sprop parameter strings", prepend these to the file:
			for (unsigned j = 0; j < 3; ++j)
			{
				unsigned numSPropRecords;
				SPropRecord* sPropRecords = parseSPropParameterSets(fSubsession.fmtp_spropparametersets(), numSPropRecords);
				for (unsigned i = 0; i < numSPropRecords; ++i)
				{
					fFileSink->addData(start_code, 4, presentationTime);
					fFileSink->addData(sPropRecords[i].sPropBytes, sPropRecords[i].sPropLength, presentationTime);
				}
				delete[] sPropRecords;
			}
			bFirsttime = true;
		}
		
		// Write the input data to the file, with the start code in front:
		fFileSink->addData(start_code, 4, presentationTime);

		fFileSink->addData(fReceiveBuffer, frameSize, presentationTime);
	}
#endif

	//QByteArray frameBuffer((char*)fReceiveBuffer, frameSize);
	////插入SPS PPS才能让H264解码器正确解码
	//QByteArray sps = sprop_parameter_sets;
	//QByteArray extraData;
	//QList<QByteArray> recodList = sps.split(',');
	//for (int i = 0; i < recodList.size(); ++i)
	//{
	//	extraData.append(char(0x00));
	//	extraData.append(char(0x00));
	//	extraData.append(char(0x00));
	//	extraData.append(char(0x01));
	//	extraData += QByteArray::fromBase64(recodList.at(i));
	//}
	//QByteArray endMark = QByteArray(4, 0);
	//endMark[3] = 0x01;
	//frameBuffer.insert(0, extraData);
	//frameBuffer.insert(extraData.size(), endMark);

#if 0
	if (fReceiveBuffer[0] == 0x00 && fReceiveBuffer[1] == 0x00 &&fReceiveBuffer[2] == 0x00 &&fReceiveBuffer[3] == 0x01)
	{
		cout << "fuck you mathor" << endl;
	}
	else
	{
		printf("%x---%x---%x---%x\n", fReceiveBuffer[0], fReceiveBuffer[1], fReceiveBuffer[2], fReceiveBuffer[3]);
		cout << "love you" << endl;
	}

	byte *tmpby = new byte[frameSize - 4];
	//tmpby[0] = 0x00;
	//tmpby[1] = 0x00;
	//tmpby[2] = 0x00;
	//tmpby[3] = 0x01;
	memcpy(tmpby, fReceiveBuffer + 4, frameSize - 4);
#endif
	//RTPSource* pRtpSource = fSubsession.rtpSource();
	//pRtpSource->fRe
	//fSubsession->

	//H264_2_RGB(fReceiveBuffer, frameSize, RgbBuf, &RgbSize, &RgbWidth, &RgbHeight);
	/*IplImage *pRgbImg = cvCreateImage(cvSize(RgbWidth, RgbHeight), 8, 3);
	memcpy(pRgbImg->imageData, RgbBuf, RgbWidth * 3 * RgbHeight);
	cvShowImage("GB28181Show", pRgbImg);
	cvWaitKey(10);
	cvReleaseImage(&pRgbImg);*/
	continuePlaying();
}

Boolean DummySink::continuePlaying() {
	if (fSource == NULL) return false; // sanity check (should not happen)

									   // Request the next frame of data from our input source.  "afterGettingFrame()" will get called later, when it arrives:
	fSource->getNextFrame(fReceiveBuffer, DUMMY_SINK_RECEIVE_BUFFER_SIZE,
		afterGettingFrame, this,
		onSourceClosure, this);
	return true;
}
