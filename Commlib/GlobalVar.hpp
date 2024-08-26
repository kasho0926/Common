#pragma once

#define PROPERTY(varType, varName)\
protected: varType m_##varName;\
public: inline varType & Get##varName(void){return  m_##varName;}\
public: inline void Set##varName(const varType &var){ m_##varName = var;};

class GlobalVar
{
public:
  static GlobalVar& Instance()
  {
    static GlobalVar instance;
    return instance;
  }

  GlobalVar(GlobalVar const&) = delete;
  void operator=(GlobalVar const&) = delete;

private:
  GlobalVar() {}
  ~GlobalVar() {}
};