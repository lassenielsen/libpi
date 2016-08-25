#pragma once
// DOCUMENTATION: link.hpp {{{
/*! \file
 * This file defines the libpi (pi-calculus operations for C++) link
 * interface (sometimes known as channels used for linking).
 * Only important extension to being a Value,
 * is the Connect function used to create a
 * Session, whick is implemented by each
 * type of link.
 */
// }}}
#include <memory>
#include <libpi/value.hpp>
#include <libpi/session.hpp>

namespace libpi
{
  class Link : public Value
  { public:
      virtual std::shared_ptr<Session> Connect(int pid, int actors) = 0;
  };
}
