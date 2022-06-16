#include "event_sink.h"
#include "debug_log.h"

ULONG EventSink::AddRef()
{
  return InterlockedIncrement(&m_lRef);
}

ULONG EventSink::Release()
{
  LONG lRef = InterlockedDecrement(&m_lRef);
  if(lRef == 0)
    delete this;
  return lRef;
}

HRESULT EventSink::QueryInterface(REFIID riid, void** ppv)
{
  if (riid == IID_IUnknown || riid == IID_IWbemObjectSink)
  {
    *ppv = (IWbemObjectSink *) this;
    AddRef();
    return WBEM_S_NO_ERROR;
  }
  else return E_NOINTERFACE;
}

HRESULT EventSink::Indicate(long lObjectCount,
                            IWbemClassObject **apObjArray)
{
  HRESULT hres = S_OK;
  printf("Event rrrrr\n");
  _variant_t vtProp;

  for (int i = 0; i < lObjectCount; i++)
  {
    hres = apObjArray[i]->Get(_bstr_t(L"TargetInstance"), 0, &vtProp, 0, 0);
    if (!FAILED(hres))
    {
      IUnknown* str = vtProp;
      hres = str->QueryInterface( IID_IWbemClassObject, reinterpret_cast< void** >( &apObjArray[i] ) );
      if ( SUCCEEDED( hres ) )
      {
        _variant_t cn;

        hres = apObjArray[i]->Get( L"Category", 0, &cn, NULL, NULL );
        if ( SUCCEEDED( hres ) )
        {
          LOGWF_TRACE(L"Category VT: %d", cn.vt);
          if ((cn.vt==VT_NULL) || (cn.vt==VT_EMPTY))
          {
            LOGWF_TRACE(L"Category: %s",((cn.vt==VT_NULL) ? L"NULL" : L"EMPTY") );
          }
          else
          {
            LOGWF_WARN(L"Category: %d",cn.uintVal );
          }
        }
        VariantClear(&cn);

        hres = apObjArray[i]->Get( L"CategoryString", 0, &cn, NULL, NULL );
        if ( SUCCEEDED( hres ) )
        {
          LOGWF_TRACE(L"CategoryString VT: %d", cn.vt);
          if ((cn.vt==VT_NULL) || (cn.vt==VT_EMPTY))
          {
            LOGWF_TRACE(L"CategoryString: %s",((cn.vt==VT_NULL) ? L"NULL" : L"EMPTY") );
          }
          else
          {
            LOGWF_WARN(L"CategoryString: %s",cn.bstrVal );
          }
        }
        VariantClear(&cn);

        hres = apObjArray[i]->Get( L"ComputerName", 0, &cn, NULL, NULL );
        if ( SUCCEEDED( hres ) )
        {
          LOGWF_TRACE(L"ComputerName VT: %d", cn.vt);
          if ((cn.vt==VT_NULL) || (cn.vt==VT_EMPTY))
          {
            LOGWF_TRACE(L"ComputerName: %s",((cn.vt==VT_NULL) ? L"NULL" : L"EMPTY") );
          }
          else
          {
            LOGWF_WARN(L"ComputerName: %s",cn.bstrVal );
          }
        }
        VariantClear(&cn);

        hres = apObjArray[i]->Get( L"Data", 0, &cn, NULL, NULL );
        if ( SUCCEEDED( hres ) )
        {
          LOGWF_TRACE(L"Data VT: %d", cn.vt);
          if ((cn.vt==VT_NULL) || (cn.vt==VT_EMPTY))
          {
            LOGWF_TRACE(L"Data: %s",((cn.vt==VT_NULL) ? L"NULL" : L"EMPTY") );
          }
          else
          {
            LOGWF_TRACE(L"Data cDims: %d, fFeatures: %d, cbElements: %d",cn.parray->cDims, cn.parray->fFeatures, cn.parray->cbElements);
            UINT * pArray = (UINT *)cn.parray->pvData;
            for( int j = 0; j < cn.parray->cbElements-1; j++)
            {
              LOGWF_WARN(L"Data index: %d, pArray: %d", j, pArray[j]);
            }
          }
        }
        VariantClear(&cn);

        hres = apObjArray[i]->Get( L"EventCode", 0, &cn, NULL, NULL );
        if ( SUCCEEDED( hres ) )
        {
          LOGWF_TRACE(L"EventCode VT: %d", cn.vt);
          if ((cn.vt==VT_NULL) || (cn.vt==VT_EMPTY))
          {
            LOGWF_TRACE(L"EventCode: %s",((cn.vt==VT_NULL) ? L"NULL" : L"EMPTY") );
          }
          else
          {
            LOGWF_WARN(L"EventCode: %d",cn.uintVal );
          }
        }
        VariantClear(&cn);

        hres = apObjArray[i]->Get( L"EventIdentifier", 0, &cn, NULL, NULL );
        if ( SUCCEEDED( hres ) )
        {
          LOGWF_TRACE(L"EventIdentifier VT: %d", cn.vt);
          if ((cn.vt==VT_NULL) || (cn.vt==VT_EMPTY))
          {
            LOGWF_TRACE(L"EventIdentifier: %s",((cn.vt==VT_NULL) ? L"NULL" : L"EMPTY") );
          }
          else
          {
            LOGWF_WARN(L"EventIdentifier: %d",cn.uintVal );
          }
        }
        VariantClear(&cn);

        hres = apObjArray[i]->Get( L"EventType", 0, &cn, NULL, NULL );
        if ( SUCCEEDED( hres ) )
        {
          LOGWF_TRACE(L"EventType VT: %d", cn.vt);
          if ((cn.vt==VT_NULL) || (cn.vt==VT_EMPTY))
          {
            LOGWF_TRACE(L"EventType: %s",((cn.vt==VT_NULL) ? L"NULL" : L"EMPTY") );
          }
          else
          {
            LOGWF_WARN(L"EventType: %d",cn.uintVal );
          }
        }
        VariantClear(&cn);

        hres = apObjArray[i]->Get( L"InsertionStrings", 0, &cn, NULL, NULL );
        if ( SUCCEEDED( hres ) )
        {
          LOGWF_TRACE(L"InsertionStrings VT: %d", cn.vt);
          if ((cn.vt==VT_NULL) || (cn.vt==VT_EMPTY))
          {
            LOGWF_TRACE(L"InsertionStrings: %s",((cn.vt==VT_NULL) ? L"NULL" : L"EMPTY") );
          }
          else
          {
            LOGWF_TRACE(L"InsertionStrings cDims: %d, fFeatures: %d, cbElements: %d",cn.parray->cDims, cn.parray->fFeatures, cn.parray->cbElements);
            BSTR * pArray = (BSTR *)cn.parray->pvData;
            for( int j = 0; j < cn.parray->cbElements-1; j++)
            {
              LOGWF_WARN(L"InsertionStrings index: %d, pArray: %s", j, pArray[j]);
            }
          }
        }
        VariantClear(&cn);

        hres = apObjArray[i]->Get( L"Logfile", 0, &cn, NULL, NULL );
        if ( SUCCEEDED( hres ) )
        {
          LOGWF_TRACE(L"Logfile VT: %d", cn.vt);
          if ((cn.vt==VT_NULL) || (cn.vt==VT_EMPTY))
          {
            LOGWF_TRACE(L"Logfile: %s",((cn.vt==VT_NULL) ? L"NULL" : L"EMPTY") );
          }
          else
          {
            LOGWF_WARN(L"Logfile: %s",cn.bstrVal );
          }
        }
        VariantClear(&cn);

        hres = apObjArray[i]->Get( L"Message", 0, &cn, NULL, NULL );
        if ( SUCCEEDED( hres ) )
        {
          LOGWF_TRACE(L"Message VT: %d", cn.vt);
          if ((cn.vt==VT_NULL) || (cn.vt==VT_EMPTY))
          {
            LOGWF_TRACE(L"Message: %s",((cn.vt==VT_NULL) ? L"NULL" : L"EMPTY") );
          }
          else
          {
            LOGWF_WARN(L"Message: %s",cn.bstrVal );
          }
        }
        VariantClear(&cn);

        hres = apObjArray[i]->Get( L"RecordNumber", 0, &cn, NULL, NULL );
        if ( SUCCEEDED( hres ) )
        {
          LOGWF_TRACE(L"RecordNumber VT: %d", cn.vt);
          if ((cn.vt==VT_NULL) || (cn.vt==VT_EMPTY))
          {
            LOGWF_TRACE(L"RecordNumber: %s",((cn.vt==VT_NULL) ? L"NULL" : L"EMPTY") );
          }
          else
          {
            LOGWF_WARN(L"RecordNumber: %d",cn.uintVal );
          }
        }
        VariantClear(&cn);

        hres = apObjArray[i]->Get( L"SourceName", 0, &cn, NULL, NULL );
        if ( SUCCEEDED( hres ) )
        {
          LOGWF_TRACE(L"SourceName VT: %d", cn.vt);
          if ((cn.vt==VT_NULL) || (cn.vt==VT_EMPTY))
          {
            LOGWF_TRACE(L"SourceName: %s",((cn.vt==VT_NULL) ? L"NULL" : L"EMPTY") );
          }
          else
          {
            LOGWF_WARN(L"SourceName: %s",cn.bstrVal );
          }
        }
        VariantClear(&cn);

        hres = apObjArray[i]->Get( L"TimeGenerated", 0, &cn, NULL, NULL );
        if ( SUCCEEDED( hres ) )
        {
          LOGWF_TRACE(L"TimeGenerated VT: %d", cn.vt);
          if ((cn.vt==VT_NULL) || (cn.vt==VT_EMPTY))
          {
            LOGWF_TRACE(L"TimeGenerated: %s",((cn.vt==VT_NULL) ? L"NULL" : L"EMPTY") );
          }
          else
          {
            LOGWF_WARN(L"TimeGenerated: %s",cn.bstrVal );
          }
        }
        VariantClear(&cn);

        hres = apObjArray[i]->Get( L"TimeWritten", 0, &cn, NULL, NULL );
        if ( SUCCEEDED( hres ) )
        {
          LOGWF_TRACE(L"TimeWritten VT: %d", cn.vt);
          if ((cn.vt==VT_NULL) || (cn.vt==VT_EMPTY))
          {
            LOGWF_TRACE(L"TimeWritten: %s",((cn.vt==VT_NULL) ? L"NULL" : L"EMPTY") );
          }
          else
          {
            LOGWF_WARN(L"TimeWritten: %s",cn.bstrVal );
          }
        }
        VariantClear(&cn);

        hres = apObjArray[i]->Get( L"Type", 0, &cn, NULL, NULL );
        if ( SUCCEEDED( hres ) )
        {
          LOGWF_TRACE(L"Type VT: %d", cn.vt);
          if ((cn.vt==VT_NULL) || (cn.vt==VT_EMPTY))
          {
            LOGWF_TRACE(L"Type: %s",((cn.vt==VT_NULL) ? L"NULL" : L"EMPTY") );
          }
          else
          {
            LOGWF_WARN(L"Type: %s",cn.bstrVal );
          }
        }
        VariantClear(&cn);

        hres = apObjArray[i]->Get( L"User", 0, &cn, NULL, NULL );
        if ( SUCCEEDED( hres ) )
        {
          LOGWF_TRACE(L"User VT: %d", cn.vt);
          if ((cn.vt==VT_NULL) || (cn.vt==VT_EMPTY))
          {
            LOGWF_TRACE(L"User: %s",((cn.vt==VT_NULL) ? L"NULL" : L"EMPTY") );
          }
          else
          {
            LOGWF_WARN(L"User: %s",cn.bstrVal );
          }
        }
        VariantClear(&cn);
      }
    }
    VariantClear(&vtProp);
  }
  return WBEM_S_NO_ERROR;
}

HRESULT EventSink::SetStatus(
  /* [in] */ LONG lFlags,
  /* [in] */ HRESULT hResult,
  /* [in] */ BSTR strParam,
  /* [in] */ IWbemClassObject __RPC_FAR *pObjParam
  )
{
  if(lFlags == WBEM_STATUS_COMPLETE)
  {
    printf("Call complete. hResult = 0x%X\n", hResult);
  }
  else if(lFlags == WBEM_STATUS_PROGRESS)
  {
    printf("Call in progress.\n");
  }

  return WBEM_S_NO_ERROR;
}    // end of EventSink.cpp