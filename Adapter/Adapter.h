#pragma once

#include "framework.h"

class Router;
class Adapter
{
public:
	Adapter();
	~Adapter();
	void initialize();
	int  getSpeakerCount();
	int  getMicCount();
	wchar_t* getSpeakerList(int index);
	wchar_t* getMicList(int index);
	bool attachSpeaker(bool enabled);
	bool attachMic(bool enabled);

private:
	wchar_t* getList(IMMDeviceCollection* pList, int index);

private:
	IMMDeviceCollection* _RenderList;
	IMMDeviceCollection* _CaptrueList;

	IMMDevice* _RenderDev;
	IMMDevice* _CaptureDev;
	IMMDevice* _RouterRenderDev;
	IMMDevice* _RouterCaptureDev;

	Router* _SpeakerRouter;
	Router* _MicRouter;
};

