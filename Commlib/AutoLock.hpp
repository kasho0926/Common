#ifndef _AUTO_LOCK
#define _AUTO_LOCK

#include <memory>
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

namespace Sync {
  class CriticalSection
  {
  public:
    CriticalSection() throw()
    {
      m_sec = std::unique_ptr<CRITICAL_SECTION>(new CRITICAL_SECTION());
    }

    ~CriticalSection()
    {
      m_sec.reset();
    }

    HRESULT Lock() throw()
    {
      EnterCriticalSection(m_sec.get());
      return S_OK;
    }

    HRESULT Unlock() throw()
    {
      LeaveCriticalSection(m_sec.get());
      return S_OK;
    }

    HRESULT Init() throw()
    {
      HRESULT hRes = E_FAIL;
      try
      {
        InitializeCriticalSection(m_sec.get());
        hRes = S_OK;
      }
      catch(...)
      {
      }

      return hRes;
    }

    HRESULT Term() throw()
    {
      if(m_sec)
      {
        DeleteCriticalSection(m_sec.get());
      }
      return S_OK;
    }

  protected:
    std::unique_ptr<CRITICAL_SECTION> m_sec;
  };

  // 封裝 Critical Section，自動呼叫 Init() / Term()
  class AutoCriticalSection : public CriticalSection
  {
  public:
    AutoCriticalSection()
    {
      CriticalSection::Init();
    }

    AutoCriticalSection(AutoCriticalSection &&target)
    {
      *this = std::move(target);
    }

    ~AutoCriticalSection() throw()
    {
      CriticalSection::Term();
    }

    AutoCriticalSection & operator= (AutoCriticalSection &&target)
    {
      m_owner = std::move(target.m_owner);
      m_sec = std::move(target.m_sec);
      return *this;
    }

    void TryUnlock()
    {
      if(GetCurrentThreadId() == m_owner)
      {
        m_owner = 0;
        Unlock();
      }
    }
    void SetOwner(const DWORD &tid)
    {
      m_owner = tid;
    }

  private:
    DWORD m_owner;
  };

  // 封裝 Auto Critical Section，自動呼叫 Lock() / Unlock()
  class AutoLock
  {
  public:
    AutoLock(AutoCriticalSection& cs) : m_cs(cs)
    {
      m_cs.Lock();
      m_cs.SetOwner(GetCurrentThreadId());
    }

    virtual ~AutoLock()
    {
      m_cs.SetOwner(0);
      m_cs.Unlock();
    }

  private:
    AutoCriticalSection & m_cs;
  };
}

#endif