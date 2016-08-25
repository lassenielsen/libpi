#pragma once
#include <libpi/link.hpp>
#include <libpi/thread/channel.hpp>

namespace libpi
{
  namespace thread
  {
// DOCUMENTATION: thread::Link class {{{
/*!
 * Thread based inplementation of a link point.
 * lass holds a vector of thread::Channels used for sending session
 * channels.
 */
// }}}
class Link : public libpi::Link // {{{
{ public:
    Link(int actors);
    virtual ~Link();
    virtual std::string GetType() const;
    virtual std::string ToString() const;
    virtual bool operator==(const Value &rhs) const;
    virtual std::shared_ptr<Session> Connect(int pid, int actors);

  private:
    std::vector<std::shared_ptr<libpi::thread::Channel> > myChannels;
}; // }}}
  }
}
