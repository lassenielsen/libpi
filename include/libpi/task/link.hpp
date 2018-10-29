#pragma once
#include <libpi/link.hpp>
#include <libpi/task/channel.hpp>

namespace libpi
{
  namespace task
  {
// DOCUMENTATION: task::Link class {{{
/*!
 * Thread based inplementation of a link point.
 * lass holds a vector of task::Channels used for sending session
 * channels.
 */
// }}}
class Link : public libpi::Link // {{{
{ public:
    Link(int actors, libpi::gc::GCRegistrant *registrant);
    virtual ~Link();
    virtual std::string GetType() const;
    virtual void ToStream(std::ostream &dest) const;
    virtual Bool *operator==(const Value &rhs) const;
    virtual Bool *operator<=(const Value &rhs) const;
    virtual Bool *operator<(const Value &rhs) const;
    virtual Bool *operator>=(const Value &rhs) const;
    virtual Bool *operator>(const Value &rhs) const;
    virtual Session *Connect(int pid, int actors, libpi::gc::GCRegistrant *registrant);

    const std::vector<Channel*> &GetChannels() const { return myChannels; }
    std::vector<Channel*> &GetChannels() { return myChannels; }

    virtual void Mark(std::unordered_set<Value*> &marks);

  private:
    std::vector<Channel*> myChannels;
}; // }}}
  }
}
