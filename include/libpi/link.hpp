#pragma once
// DOCUMENTATION: link.hpp {{{
/*! \file
 * This file defines the libpi (pi-calculus operations for C++) link
 * interface (sometimes known as channels used for linking).
 */
// }}}
#include <memory>
#include <libpi/value.hpp>
#include <libpi/session.hpp>

namespace libpi
{
// DOCUMENTATION: Link class {{{
/*!
 * Link is a value used to create sessions.
 * Using channels a session is negotiated, and the necessary session channels
 * are exchanged.
 *
 * Only important extension to being a Value,
 * is the Connect function used to create a
 * Session, which is implemented by each
 * type of link.
 */
// }}}
  class Link : public Value
  { public:
      Link(gc::GCRegistrant *registrant): Value(registrant) {}
      virtual Session *Connect(int pid, int actors, gc::GCRegistrant *registrant) = 0;
  };
}
