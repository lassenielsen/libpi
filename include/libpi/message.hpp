#ifndef libpi_message
#define libpi_message
// DOCUMENTATION: message.hpp {{{
/*! \file
 * This file defines the cpi (pi-calculus operations for C++) message
 * structure and interface.
 */
// }}}

#include <vector>
#include <string>

namespace libpi
{
// DOCUMENTATION: Message class {{{
/*!
 * Message represents a single untyped message.
 * Data can be extracted and added using raw char buffers.
 */
// }}}
  class Message
  { public:
      Message();
      Message(const std::string &str);
      ~Message();

      void Clear();
      const char *GetData();
      int GetSize();
      void AddData(const char *data, int size);
      operator std::string ();

    private:
      std::vector<std::pair<char*,int> > myData;
  };
}

#endif
