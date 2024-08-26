#pragma once
#include <memory>
#include <mutex>

//Multiple Writer & Multiple Reader
template <class T>
class AutoStructure
{
public:
  AutoStructure() : main(NULL), backup(NULL) {}

  AutoStructure(AutoStructure &&target)
  {
    *this = std::move(target);
  }

  ~AutoStructure()
  {
    main = NULL;
    backup = NULL;
  }

  AutoStructure & operator= (AutoStructure &&target)
  {
    mainMutex = std::move(target.mainMutex);
    backupMutex = std::move(target.backupMutex);
    main = std::move(target.main);
    backup = std::move(target.backup);
    return *this;
  }

  inline std::shared_ptr<T> Get()
  {
    std::lock_guard<std::mutex>(this->mainMutex);
    return this->main;
  }

  inline void Set(std::shared_ptr<T> newOne)
  {
    SetBackup(newOne);
    Switch();
  }

private:
  std::mutex mainMutex;
  std::mutex backupMutex;
  std::shared_ptr<T> main;
  std::shared_ptr<T> backup;

  inline std::shared_ptr<T> GetBackup()
  {
    std::lock_guard<std::mutex>(this->backupMutex);
    return this->backup;
  }

  inline void SetBackup(std::shared_ptr<T> newOne)
  {
    std::lock_guard<std::mutex>(this->backupMutex);
    backup = newOne;
  }

  inline void SetMain(std::shared_ptr<T> newMain)
  {
    std::lock_guard<std::mutex>(this->mainMutex);
    main = newMain;
  }

  inline void Switch()
  {
    SetMain(GetBackup());
    backup = NULL;
  }
};