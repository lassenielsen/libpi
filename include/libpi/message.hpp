#ifndef libpi_message
#define libpi_message
// DOCUMENTATION: message.hpp {{{
/*! \file
 * This file defines the cpi (pi-calculus operations for C++) message
 * structure and interface.
 */
// }}}

#include <vector>

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
      ~Message();

      void Clear();
      const char *GetData();
      int GetSize();
      void AddData(const char *data, int size);

    private:
      std::vector<std::pair<char*,int> > myData;
  };
}

#endif
