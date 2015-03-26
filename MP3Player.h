/********************************
*  Programmer: Austin Ivicevic  *
*  2_Hour_game					*
*  Date Created: 12/1/14        *
*********************************///MP3PLAYER.h
#include <dshow.h>
#include "DirectX.h"
//DirectShow components

class MP3
{
private:
	IGraphBuilder *graphBuilder;
	IMediaControl *mediaControl;
	IMediaEvent *mediaEvent;
public:
	MP3();
	~MP3() { MP3Release(); }
	BOOL PlaySongMp3(char *filename);
	void MP3Stop();
	void MP3Pause();
	void MP3Run();
	BOOL MP3Init();
	void MP3Release();
};
MP3::MP3()
{
	graphBuilder = NULL;
	mediaControl = NULL;
	mediaEvent = NULL;
}

BOOL MP3::PlaySongMp3(char *filename)
{
	// convert filename to wide-character string
	WCHAR wFilename[MAX_PATH];
	mbstowcs(wFilename, filename, MAX_PATH);
	// render the file
	graphBuilder->RenderFile(wFilename, NULL);
	//Play the file
	mediaControl->Run();
	return true;
}
 
BOOL MP3::MP3Init()
{
	// Initialize and configure DirectShow
	if(FAILED(CoCreateInstance(CLSID_FilterGraph, NULL,
			CLSCTX_INPROC_SERVER, IID_IGraphBuilder,
						(void **)&graphBuilder)))
	{
		MessageBox(NULL, "Unable to create DirectShow Graph Builder object","Error", MB_OK);
		return FALSE;
	}
	//Query for media control
	graphBuilder->QueryInterface(IID_IMediaControl,
					(void **)&mediaControl);
	graphBuilder->QueryInterface(IID_IMediaEvent,
					(void **)&mediaEvent);
	return TRUE;
}
void MP3::MP3Pause()
{
	mediaControl->Pause();
}
void MP3::MP3Run()
{
	mediaControl->Run();
}
void MP3::MP3Stop()
{
	mediaControl->Stop();
}
void MP3::MP3Release()
{
	//stop music and release DirectShow objects
	mediaControl->Stop();
	mediaControl->Release();
	mediaControl = NULL;
	mediaEvent->Release();
	mediaEvent = NULL;
	graphBuilder->Release();
	graphBuilder = NULL;
}
