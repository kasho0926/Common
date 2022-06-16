#ifndef _ARGUMENT_H
#define _ARGUMENT_H

using namespace std;

#include <vector>
#include <string>

/*!@class IArgument argument.h
 * @brief The class parse argument list
 * Example:
 *   IArgument * arg = new Argument();
 *   arg->SetArg( arc, argv );
 *   arg->ParseArg();
 *   arg->getFileName();
 *   delete arg;
 */
class IArgument
{
public:
  IArgument();
  /*!@brief Constructor
   *   Read arg list into Vector
   * @param argc
   *   number of arg
   * @param argv
   *   arg list
   */
  IArgument(int argc, _TCHAR * argv[]);
  /// destructor
  ~IArgument();
  /*!@brief
   * parse arg list
   * @retval 0 succed
   * @retval other error
   */
  virtual int ParseArg() = 0;
  //!@brief SetArg
  //!  parse arg list into m_arg
  //!@param argc
  //!  number of arg
  //!@param argv
  //!  arg array
  //!retval 0 succeed
  //!retval other error code
  int SetArg(int argc, _TCHAR * argv[]);
protected:
  vector<wstring> m_arg; /// ¦s©ñ arg list
};

#endif 