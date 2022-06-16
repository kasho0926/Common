#include "WMI_control.h"
#include "event_sink.h"

using namespace std;

WMIControl::WMIControl(void)
{
}

WMIControl::~WMIControl(void)
{
}

int WMIControl::init()
{
  HRESULT hres;

  // Initialize COM. ------------------------------------------
  hres =  CoInitializeEx(0, COINIT_MULTITHREADED);
  if (FAILED(hres))
  {
    cout << "Failed to initialize COM library. Error code = 0x"
      << hex << hres << endl;
    return 1;                  // Program has failed.
  }

  // Set general COM security levels --------------------------
  hres =  CoInitializeSecurity(
    NULL,
    -1,                          // COM negotiates service
    NULL,                        // Authentication services
    NULL,                        // Reserved
    RPC_C_AUTHN_LEVEL_DEFAULT,   // Default authentication
    RPC_C_IMP_LEVEL_IMPERSONATE, // Default Impersonation
    NULL,                        // Authentication info
    EOAC_NONE,                   // Additional capabilities
    NULL                         // Reserved
    );

  if (FAILED(hres))
  {
    cout << "Failed to initialize security. Error code = 0x"
      << hex << hres << endl;
    CoUninitialize();
    return 1;                      // Program has failed.
  }

  // Obtain the initial locator to WMI -------------------------

  CComPtr<IWbemLocator> cLoc;
  hres = cLoc.CoCreateInstance(CLSID_WbemLocator,
    0,
    CLSCTX_INPROC_SERVER);

  if (FAILED(hres))
  {
    cout << "Failed to create IWbemLocator object. "
      << "Err code = 0x"
      << hex << hres << endl;
    CoUninitialize();
    return 1;                 // Program has failed.
  }

  // Connect to WMI through the IWbemLocator::ConnectServer method

  CComPtr<IWbemServices> cSvc = NULL;

  // Connect to the local root\cimv2 namespace
  // and obtain pointer pSvc to make IWbemServices calls.
  hres = cLoc->ConnectServer(
    _bstr_t(L"ROOT\\CIMV2"),
    NULL,
    NULL,
    0,
    NULL,
    0,
    0,
    &cSvc
    );

  if (FAILED(hres))
  {
    cout << "Could not connect. Error code = 0x"
      << hex << hres << endl;
    CoUninitialize();
    return 1;                // Program has failed.
  }

  cout << "Connected to ROOT\\CIMV2 WMI namespace" << endl;

  // Set security levels on the proxy -------------------------

  hres = CoSetProxyBlanket(
    cSvc,                        // Indicates the proxy to set
    RPC_C_AUTHN_WINNT,           // RPC_C_AUTHN_xxx
    RPC_C_AUTHZ_NONE,            // RPC_C_AUTHZ_xxx
    NULL,                        // Server principal name
    RPC_C_AUTHN_LEVEL_CALL,      // RPC_C_AUTHN_LEVEL_xxx
    RPC_C_IMP_LEVEL_IMPERSONATE, // RPC_C_IMP_LEVEL_xxx
    NULL,                        // client identity
    EOAC_NONE                    // proxy capabilities
    );
  if (FAILED(hres))
  {
    cout << "Could not set proxy blanket. Error code = 0x"
      << hex << hres << endl;
    CoUninitialize();
    return 1;               // Program has failed.
  }

  // Receive event notifications -----------------------------

  // Use an unsecured apartment for security
  CComPtr<IUnsecuredApartment> cUnsecApp;

  hres = cUnsecApp.CoCreateInstance(CLSID_UnsecuredApartment, NULL,
    CLSCTX_LOCAL_SERVER);

  EventSink* pSink = new EventSink;
  pSink->AddRef();

  CComPtr<IUnknown> cStubUnk ;
  cUnsecApp->CreateObjectStub(pSink, &cStubUnk);

  CComPtr<IWbemObjectSink> cStubSink;
  cStubUnk->QueryInterface(IID_IWbemObjectSink,
    (void **) &cStubSink);
  // The ExecNotificationQueryAsync method will call
  // The EventQuery::Indicate method when an event occurs
  hres = cSvc->ExecNotificationQueryAsync(
    _bstr_t("WQL"),
    _bstr_t("SELECT * "
    "FROM __InstanceCreationEvent "
    "WHERE TargetInstance ISA 'Win32_NTLogEvent' AND TargetInstance.EventIdentifier='4104'"),
    WBEM_FLAG_SEND_STATUS,
    NULL,
    cStubSink);

  // AND TargetInstance.EventCode='4104'
  //"WHERE TargetInstance ISA 'Win32_NTLogEvent' AND TargetInstance.EventIdentifier= '4104'"),
  //"select * from __instancecreationevent where targetinstance isa 'Win32_NTLogEvent'"

    // _bstr_t("SELECT * "
    //"FROM __InstanceCreationEvent WITHIN "
    //"WHERE TargetInstance ISA 'Win32_NTLogEvent'"),


  // Check for errors.
  if (FAILED(hres))
  {
    printf("ExecNotificationQueryAsync failed "
      "with = 0x%X\n", hres);
    pSink->Release();
    CoUninitialize();
    return 1;
  }

  // Wait for the event
  Sleep(1000000);

  hres = cSvc->CancelAsyncCall(cStubSink);
  pSink->Release();
  //CoUninitialize();
}