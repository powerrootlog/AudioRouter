// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <atlbase.h>
#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <AudioPolicy.h>
#include <Functiondiscoverykeys_devpkey.h>

#define _CRT_SECURE_CPP_OVERLOAD_SECURE_NAMES 1
#include <new>
#include <windows.h>
#include <strsafe.h>
#include <objbase.h>
#pragma warning(push)
#pragma warning(disable : 4201)

template <class T> void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}
