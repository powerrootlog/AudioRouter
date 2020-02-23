#include "Router.h"
#include <assert.h>
#include <avrt.h>


Router::Router() :
	_RefCount(1),
	_RouterDevice(nullptr),
	_RenderDevice(nullptr),
	_RouterAudioClient(nullptr),
	_RenderAudioClient(nullptr),
	_CaptureClient(nullptr),
	_RenderClient(nullptr),
	_RouterThreadHandle(nullptr),
	_StopEvent(nullptr),
	_AudioSamplesReadyEvent(nullptr),
	_MixFormat(nullptr),
	_BufferSize(0),
	_FrameSize(0)
{
}

Router::~Router()
{
	Shutdown();
}

DWORD __stdcall Router::RouterThread(LPVOID Context)
{
	Router* router = static_cast<Router*>(Context);
	return router->DoRouterThread();
}

DWORD Router::DoRouterThread()
{
	bool stillPlaying = true;
	HANDLE mmcssHandle = NULL;
	DWORD mmcssTaskIndex = 0;
	bool DisableMMCSS = false;
	HANDLE waitArray[2] = { _StopEvent, _AudioSamplesReadyEvent };

	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		_RPTF1(_CRT_WARN, "Unable to initialize COM in router thread: %x\n", hr);
		return hr;
	}

	if (!DisableMMCSS)
	{
		mmcssHandle = AvSetMmThreadCharacteristics(L"Audio", &mmcssTaskIndex);		// L"Pro Audio"
		if (mmcssHandle == NULL)
		{
			_RPTF1(_CRT_WARN, "Unable to enable MMCSS on render thread: %d\n", GetLastError());
		}
	}

	while (stillPlaying)
	{
		DWORD waitResult = WaitForMultipleObjects(2, waitArray, FALSE, INFINITE);
		switch (waitResult)
		{
		case WAIT_OBJECT_0 + 0:			// _StopEvent
			stillPlaying = false;
			break;
		case  WAIT_OBJECT_0 + 1:		// _AudioSampleReadyEvent
			BYTE* pInData;
			UINT32 inFramesAvailable;
			DWORD  flags;
			BYTE* pOutData;
			UINT32 padding;
			UINT32 outFramesAvailable;

			hr = _CaptureClient->GetBuffer(&pInData, &inFramesAvailable, &flags, NULL, NULL);
			if (SUCCEEDED(hr))
			{
				hr = _RenderAudioClient->GetCurrentPadding(&padding);
				if (SUCCEEDED(hr))
				{
					outFramesAvailable = _BufferSize - padding;
					//UINT32 framesToWrite = min(inFramesAvailable, outFramesAvailable);
					if (inFramesAvailable > outFramesAvailable)
						Sleep(10);
					hr = _RenderClient->GetBuffer(inFramesAvailable, &pOutData);
					if (SUCCEEDED(hr))
					{
						CopyMemory(pOutData, pInData, inFramesAvailable * _FrameSize);
						hr = _RenderClient->ReleaseBuffer(inFramesAvailable, 0);
						if (!SUCCEEDED(hr))
						{
							printf("Unable to release render buffe: %x\n", hr);
							stillPlaying = false;
						}

					}
				}
				hr = _CaptureClient->ReleaseBuffer(inFramesAvailable);
				if (FAILED(hr))
				{
					_RPTF1(_CRT_WARN, "Unable to release capture buffer: %x!\n", hr);
				}
			}
			break;
		default:
			_RPTF1(_CRT_WARN, "Wait error: %d\n", GetLastError());
			ExitThread(0);
		}
	}

	if (!DisableMMCSS)
	{
		AvRevertMmThreadCharacteristics(mmcssHandle);
	}

	CoUninitialize();

	return 0;
}

bool Router::LoadFormat()
{
	HRESULT hr = _RenderAudioClient->GetMixFormat(&_MixFormat);
	if (FAILED(hr))
	{
		_RPTF1(_CRT_WARN, "Unable to get mix format on audio client: %x.\n", hr);
		return false;
	}
	//WAVEFORMATEX Format;
	//ZeroMemory(&Format, sizeof(Format));
	//Format.wFormatTag = WAVE_FORMAT_PCM;
	//Format.nChannels = 2;
	//Format.wBitsPerSample = 16;
	//Format.nSamplesPerSec = 48000;
	WAVEFORMATEX* Match = NULL;
	//hr = _RenderAudioClient->IsFormatSupported(AUDCLNT_SHAREMODE_EXCLUSIVE, &Format, &Match);
	hr = _RouterAudioClient->IsFormatSupported(AUDCLNT_SHAREMODE_SHARED, _MixFormat, &Match);
	if (FAILED(hr))
	{
		return false;
	}
	_FrameSize = (_MixFormat->wBitsPerSample / 8) * _MixFormat->nChannels;
	return true;
}

bool Router::InitializeAudioEngine()
{
	LONG        EngineLatencyInMS = 100;

	HRESULT hr = _RouterAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_LOOPBACK | AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_NOPERSIST, EngineLatencyInMS * 10000, 0, _MixFormat, NULL);
	if (FAILED(hr))
	{
		_RPTF1(_CRT_WARN, "Unable to initialize router audio client: %x.\n", hr);
		return false;
	}

	hr = _RouterAudioClient->GetBufferSize(&_BufferSize);
	if (FAILED(hr))
	{
		_RPTF1(_CRT_WARN, "Unable to get audio client buffer: %x. \n", hr);
		return false;
	}

	hr = _RouterAudioClient->SetEventHandle(_AudioSamplesReadyEvent);
	if (FAILED(hr))
	{
		_RPTF1(_CRT_WARN, "Unable to set ready event: %x.\n", hr);
		return false;
	}

	hr = _RouterAudioClient->GetService(IID_PPV_ARGS(&_CaptureClient));
	if (FAILED(hr))
	{
		_RPTF1(_CRT_WARN, "Unable to get new capture client: %x.\n", hr);
		return false;
	}

	hr = _RenderAudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_NOPERSIST, EngineLatencyInMS * 10000, 0, _MixFormat, NULL);
	if (FAILED(hr))
	{
		_RPTF1(_CRT_WARN, "Unable to initialize render audio client: %x.\n", hr);
		return false;
	}

	hr = _RenderAudioClient->GetService(IID_PPV_ARGS(&_RenderClient));
	if (FAILED(hr))
	{
		_RPTF1(_CRT_WARN, "Unable to get new render client: %x.\n", hr);
		return false;
	}


	return true;
}

bool Router::Initialize(IMMDevice* RouterDev, IMMDevice* RenderDev)
{
	HRESULT hr;

	_RouterDevice = RouterDev;
	_RouterDevice->AddRef();
	_RenderDevice = RenderDev;
	_RenderDevice->AddRef();

	_StopEvent = CreateEventEx(NULL, NULL, 0, EVENT_MODIFY_STATE | SYNCHRONIZE);
	if (_StopEvent == NULL)
	{
		_RPTF1(_CRT_WARN, "Unable to create stop event: %d.\n", GetLastError());
		return false;
	}

	_AudioSamplesReadyEvent = CreateEventEx(NULL, NULL, 0, EVENT_MODIFY_STATE | SYNCHRONIZE);
	if (_AudioSamplesReadyEvent == NULL)
	{
		_RPTF1(_CRT_WARN, "Unable to create samples ready event: %d.\n", GetLastError());
		return false;
	}

	hr = _RouterDevice->Activate(__uuidof(IAudioClient), CLSCTX_INPROC_SERVER, NULL, reinterpret_cast<void**>(&_RouterAudioClient));
	if (FAILED(hr))
	{
		_RPTF1(_CRT_WARN, "Unable to activate audio client for router: %x.\n", hr);
		return false;
	}

	hr = _RenderDevice->Activate(__uuidof(IAudioClient), CLSCTX_INPROC_SERVER, NULL, reinterpret_cast<void**>(&_RenderAudioClient));
	if (FAILED(hr))
	{
		_RPTF1(_CRT_WARN, "Unable to activate audio client for render: %x.\n", hr);
		return false;
	}

	if (!LoadFormat())
	{
		_RPTF0(_CRT_WARN, "Failed to load the mix format \n");
		return false;
	}

	if (!InitializeAudioEngine())
	{
		return false;
	}

	return true;
}

bool Router::Start()
{
	HRESULT hr;

	_RouterThreadHandle = CreateThread(NULL, 0, RouterThread, this, 0, NULL);
	if (_RouterThreadHandle == NULL)
	{
		_RPTF1(_CRT_WARN, "Unable to create router thread: %x.", GetLastError());
		return false;
	}

	hr = _RenderAudioClient->Start();
	if (FAILED(hr))
	{
		_RPTF1(_CRT_WARN, "Unable to start render client: %x.\n", hr);
		return false;
	}

	hr = _RouterAudioClient->Start();
	if (FAILED(hr))
	{
		_RPTF1(_CRT_WARN, "Unable to start capture client: %x.\n", hr);
		return false;
	}

	return true;
}

bool Router::Stop()
{
	HRESULT hr;

	if (_StopEvent)
	{
		SetEvent(_StopEvent);
	}

	hr = _RouterAudioClient->Stop();
	if (FAILED(hr))
	{
		_RPTF1(_CRT_WARN, "Unable to stop router audio client: %x\n", hr);
	}

	hr = _RenderAudioClient->Stop();
	if (FAILED(hr))
	{
		_RPTF1(_CRT_WARN, "Unable to stop render audio client: %x\n", hr);
	}

	if (_RouterThreadHandle)
	{
		WaitForSingleObject(_RouterThreadHandle, INFINITE);

		CloseHandle(_RouterThreadHandle);
		_RouterThreadHandle = NULL;
	}

	return false;
}

void Router::Shutdown()
{
	if (_RouterThreadHandle)
	{
		SetEvent(_StopEvent);
		WaitForSingleObject(_RouterThreadHandle, INFINITE);
		CloseHandle(_RouterThreadHandle);
		_RouterThreadHandle = NULL;
	}

	if (_StopEvent)
	{
		CloseHandle(_StopEvent);
		_StopEvent = NULL;
	}
	if (_AudioSamplesReadyEvent)
	{
		CloseHandle(_AudioSamplesReadyEvent);
		_AudioSamplesReadyEvent = NULL;
	}

	SafeRelease(&_RouterDevice);
	SafeRelease(&_RenderDevice);
	SafeRelease(&_RouterAudioClient);
	SafeRelease(&_RenderAudioClient);
	SafeRelease(&_CaptureClient);
	SafeRelease(&_RenderClient);

	if (_MixFormat)
	{
		CoTaskMemFree(_MixFormat);
		_MixFormat = NULL;
	}

}

//
//  IUnknown
//
HRESULT Router::QueryInterface(REFIID Iid, void** Object)
{
	if (Object == NULL)
	{
		return E_POINTER;
	}
	*Object = NULL;

	if (Iid == IID_IUnknown)
	{
		*Object = static_cast<IUnknown*>(static_cast<IAudioSessionEvents*>(this));
		AddRef();
	}
	else if (Iid == __uuidof(IMMNotificationClient))
	{
		*Object = static_cast<IMMNotificationClient*>(this);
		AddRef();
	}
	else if (Iid == __uuidof(IAudioSessionEvents))
	{
		*Object = static_cast<IAudioSessionEvents*>(this);
		AddRef();
	}
	else
	{
		return E_NOINTERFACE;
	}
	return S_OK;
}
ULONG Router::AddRef()
{
	return InterlockedIncrement(&_RefCount);
}
ULONG Router::Release()
{
	ULONG returnValue = InterlockedDecrement(&_RefCount);
	if (returnValue == 0)
	{
		delete this;
	}
	return returnValue;
}
