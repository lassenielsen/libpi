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
    virtual std::shared_ptr<Bool> operator==(const Value &rhs) const;
    virtual std::shared_ptr<Bool> operator<=(const Value &rhs) const;
    virtual std::shared_ptr<Bool> operator<(const Value &rhs) const;
    virtual std::shared_ptr<Bool> operator>=(const Value &rhs) const;
    virtual std::shared_ptr<Bool> operator>(const Value &rhs) const;
    virtual std::shared_ptr<Session> Connect(int pid, int actors);

    const std::vector<std::shared_ptr<Channel> > &GetChannels() const { return myChannels; }
    std::vector<std::shared_ptr<Channel> > &GetChannels() { return myChannels; }

  private:
    std::vector<std::shared_ptr<Channel> > myChannels;
}; // }}}
  }
}
