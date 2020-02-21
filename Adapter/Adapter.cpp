#include "Adapter.h"
#include "Router.h"

const wchar_t* RouterSpeakerID = L"{0.0.0.00000000}.{ad7b0c5c-fccd-4181-8e7d-984c05106697}";
// "Speakers (High Definition Audio Device)"
const wchar_t* routerMic = L"";


Adapter::Adapter() :
	_RenderList(nullptr),
	_CaptrueList(nullptr),
	_RenderDev(nullptr),
	_CaptureDev(nullptr),
	_SpeakerRouter(nullptr),
	_MicRouter(nullptr)
{
	initialize();
}

Adapter::~Adapter()
{
	SafeRelease(&_CaptrueList);
	SafeRelease(&_RenderList);
	SafeRelease(&_CaptureDev);
	SafeRelease(&_RenderDev);
	SafeRelease(&_SpeakerRouter);
	SafeRelease(&_MicRouter);
}

void Adapter::initialize()
{
	HRESULT hr = S_OK;
	CComPtr<IMMDeviceEnumerator> pEnumerator;
	hr = pEnumerator.CoCreateInstance(__uuidof(MMDeviceEnumerator));

	hr = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE, &_RenderList);
	hr = pEnumerator->EnumAudioEndpoints(eCapture, DEVICE_STATE_ACTIVE, &_CaptrueList);
	
	hr = pEnumerator->GetDevice(RouterSpeakerID, &_RouterRenderDev);
	hr = pEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &_RenderDev);

	_SpeakerRouter = new Router;
	_MicRouter = new Router;

	_SpeakerRouter->Initialize(_RouterRenderDev, _RenderDev);
}

int Adapter::getSpeakerCount()
{
	UINT  count;

	if (SUCCEEDED(_RenderList->GetCount(&count)))
		return count;

	return 0;
}

int Adapter::getMicCount()
{
	UINT  count;

	if (SUCCEEDED(_CaptrueList->GetCount(&count)))
		return count;

	return 0;
}

wchar_t* Adapter::getSpeakerList(int index)
{
	if (_RenderList)
		return getList(_RenderList, index);
	return nullptr;
}

wchar_t* Adapter::getMicList(int index)
{
	if (_CaptrueList)
		return getList(_CaptrueList, index);
	return nullptr;
}

bool Adapter::attachSpeaker(bool enabled)
{
	if (_SpeakerRouter)
		return enabled ? _SpeakerRouter->Start() : _SpeakerRouter->Stop();
	return false;
}

bool Adapter::attachMic(bool enabled)
{
	if (_MicRouter)
		return enabled ? _MicRouter->Start() : _MicRouter->Stop();
	return false;
}

wchar_t* Adapter::getList(IMMDeviceCollection* pList, int index)
{
	HRESULT hr;
	CComPtr<IMMDevice> pEndpoint;
	CComPtr<IPropertyStore> pProps;
	LPWSTR pwszID = NULL;

	hr = pList->Item(index, &pEndpoint);

	// Get the endpoint ID string.
	hr = pEndpoint->GetId(&pwszID);

	hr = pEndpoint->OpenPropertyStore(STGM_READ, &pProps);

	PROPVARIANT varName;
	// Initialize container for property value.
	PropVariantInit(&varName);

	// Get the endpoint's friendly-name property.
	hr = pProps->GetValue(PKEY_Device_FriendlyName, &varName);

	ULONG  ulSize = (wcslen(varName.pwszVal) * sizeof(wchar_t)) + sizeof(wchar_t);
	wchar_t* pwszReturn = (wchar_t*)::CoTaskMemAlloc(ulSize);
	if (pwszReturn)
		wcscpy_s(pwszReturn, wcslen(varName.pwszVal) + 1, varName.pwszVal);

	//wchar_t* pwszReturn = _wcsdup(varName.pwszVal);

	CoTaskMemFree(pwszID);
	pwszID = NULL;
	PropVariantClear(&varName);

	return pwszReturn;

}
