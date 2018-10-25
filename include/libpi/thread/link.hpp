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
    virtual void ToStream(std::ostream &dest) const;
    virtual Bool *operator==(const Value &rhs) const;
    virtual Bool *operator<=(const Value &rhs) const;
    virtual Bool *operator<(const Value &rhs) const;
    virtual Bool *operator>=(const Value &rhs) const;
    virtual Bool *operator>(const Value &rhs) const;
    virtual Session *Connect(int pid, int actors);

    const std::vector<const Channel*> &GetChannels() const { return myChannels; }
    std::vector<const Channel*> &GetChannels() { return myChannels; }

  private:
    std::vector<const Channel*> myChannels;
}; // }}}
  }
}
