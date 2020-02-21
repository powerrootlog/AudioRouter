// dllmain.cpp : Defines the entry point for the DLL application.
#pragma once

#ifdef ADAPTER_EXPORTS
#define ADAPTER_API __declspec(dllexport)
#else
#define ADAPTER_API __declspec(dllimport)
#endif


#include "framework.h"
#include "Adapter.h"

#define EXIT_ON_ERROR(hres)  \
              if (FAILED(hres)) { goto Exit; }

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);

Adapter adapter;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		if (FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))
			return false;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" ADAPTER_API
bool __stdcall attachSpeaker(bool enabled)
{
    return adapter.attachSpeaker(enabled);
}

extern "C" ADAPTER_API
bool __stdcall attachMic(bool enabled)
{
    return adapter.attachMic(enabled);
}

extern "C" ADAPTER_API
int __stdcall getSpeakerCount()
{
	return adapter.getSpeakerCount();
}

extern "C" ADAPTER_API
int __stdcall getMicCount()
{
	return adapter.getMicCount();
}

extern "C" ADAPTER_API
wchar_t* __stdcall getSpeakerList(int index)
{
	return adapter.getSpeakerList(index);
}

extern "C" ADAPTER_API
wchar_t* __stdcall getMicList(int index)
{
	return adapter.getMicList(index);
}
