#pragma once

#include "framework.h"

class Router : IMMNotificationClient, IAudioSessionEvents
{
public:
	Router();

	bool Initialize(IMMDevice* RouterDev, IMMDevice* RenderDev);
	bool Start();
	bool Stop();
	void Shutdown();

	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

private:
	~Router();

	static DWORD __stdcall RouterThread(LPVOID Context);
	DWORD DoRouterThread();

	bool LoadFormat();

	STDMETHOD(OnDisplayNameChanged) (LPCWSTR /*NewDisplayName*/, LPCGUID /*EventContext*/) { return S_OK; };
	STDMETHOD(OnIconPathChanged) (LPCWSTR /*NewIconPath*/, LPCGUID /*EventContext*/) { return S_OK; };
	STDMETHOD(OnSimpleVolumeChanged) (float /*NewSimpleVolume*/, BOOL /*NewMute*/, LPCGUID /*EventContext*/) { return S_OK; }
	STDMETHOD(OnChannelVolumeChanged) (DWORD /*ChannelCount*/, float /*NewChannelVolumes*/[], DWORD /*ChangedChannel*/, LPCGUID /*EventContext*/) { return S_OK; };
	STDMETHOD(OnGroupingParamChanged) (LPCGUID /*NewGroupingParam*/, LPCGUID /*EventContext*/) { return S_OK; };
	STDMETHOD(OnStateChanged) (AudioSessionState /*NewState*/) { return S_OK; };
	STDMETHOD(OnSessionDisconnected) (AudioSessionDisconnectReason DisconnectReason) { return S_OK; };
	STDMETHOD(OnDeviceStateChanged) (LPCWSTR /*DeviceId*/, DWORD /*NewState*/) { return S_OK; }
	STDMETHOD(OnDeviceAdded) (LPCWSTR /*DeviceId*/) { return S_OK; };
	STDMETHOD(OnDeviceRemoved) (LPCWSTR /*DeviceId(*/) { return S_OK; };
	STDMETHOD(OnDefaultDeviceChanged) (EDataFlow Flow, ERole Role, LPCWSTR NewDefaultDeviceId) { return S_OK; };
	STDMETHOD(OnPropertyValueChanged) (LPCWSTR /*DeviceId*/, const PROPERTYKEY /*Key*/) { return S_OK; };

	//
	//  IUnknown
	//
	STDMETHOD(QueryInterface)(REFIID iid, void** pvObject);

private:
	LONG		_RefCount;
	HANDLE      _RouterThreadHandle;
	HANDLE      _StopEvent;
	HANDLE      _AudioSamplesReadyEvent;

	IMMDevice* _RouterDevice;
	IMMDevice* _RenderDevice;
	IAudioClient* _RouterAudioClient;
	IAudioClient* _RenderAudioClient;
	IAudioCaptureClient* _CaptureClient;
	IAudioRenderClient* _RenderClient;
	WAVEFORMATEX* _MixFormat;


};

