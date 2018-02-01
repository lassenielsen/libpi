#include <libpi/session.hpp>
#include <libpi/bool.hpp>
#include "common.cpp"

using namespace std;

namespace libpi
{

Session::Session(int pid, int actors, std::vector<std::shared_ptr<Channel> > &inChannels, std::vector<std::shared_ptr<Channel> > &outChannels) // {{{
: myPid(pid),
  myActors(actors),
  myInChannels(inChannels),
  myOutChannels(outChannels)
{
} // }}}

Session::~Session() // {{{
{
} // }}}

void Session::Send(int to, const shared_ptr<libpi::Value> &value) // {{{
{ if (Closed()) throw string("Session::Send: Trying to use closed session.");
  if (to<0 || to>=GetActors()) throw string("Session::Send: to must be between 0 and actors-1");
  myOutChannels[to]->Send(value);
} //}}}

void Session::Send(int to, const std::shared_ptr<task::Task> &sender, const shared_ptr<libpi::Value> &value) // {{{
{ if (Closed()) throw string("Session::Send: Trying to use closed session.");
  if (to<0 || to>=GetActors()) throw string("Session::Send: to must be between 0 and actors-1");
  myOutChannels[to]->Send(sender,value);
} //}}}

shared_ptr<libpi::Value> Session::Receive(int from) // {{{
{ if (Closed()) throw string("Session::Receive: Trying to use closed session.");
  if (from<0 || from>=GetActors()) throw string("Session::Receive: to must be between 0 and actors-1");
  return myInChannels[from]->Receive();
} //}}}

bool Session::Receive(int from, const std::shared_ptr<task::Task> &receiver, shared_ptr<libpi::Value> &dest) // {{{
{ if (Closed()) throw string("Session::Receive: Trying to use closed session.");
  if (from<0 || from>=GetActors())
    throw string("Session::Receive: to must be between 0 and actors-1");
  return myInChannels[from]->Receive(receiver,dest);
} //}}}

void Session::Close(bool unlink) // {{{
{ myActors=0;
  myPid=0;
} // }}}

bool Session::Closed() // {{{
{ return myActors==0;
} // }}}

int Session::GetActors() const // {{{
{ return myActors;
} // }}}

int Session::GetPid() const // {{{
{ return myPid;
} // }}}

string Session::GetType() const // {{{
{ return "ses";
} // }}}

void Session::ToStream(ostream &dest) const // {{{
{ dest << myPid << ":" << myActors;
  for (auto ch=myInChannels.begin(); ch!=myInChannels.end(); ++ch)
  { dest  << ":";
    (*ch)->Serialize(dest);
  }
  for (auto ch=myOutChannels.begin(); ch!=myOutChannels.end(); ++ch)
  { dest << ":";
    (*ch)->Serialize(dest);
  }
} // }}}

shared_ptr<Bool> Session::operator==(const Value &rhs) const // {{{
{ const Session *rhsptr=dynamic_cast<const Session*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  if (rhsptr->GetPid()!=GetPid())
    return Bool::GetInstance(false);
  if (rhsptr->GetActors()!=GetActors())
    return Bool::GetInstance(false);
  if (rhsptr->myInChannels!=myInChannels)
    return Bool::GetInstance(false);
  if (rhsptr->myOutChannels!=myOutChannels)
    return Bool::GetInstance(false);
  return Bool::GetInstance(true);

} // }}}
shared_ptr<Bool> Session::operator<=(const Value &rhs) const // {{{
{ return (*this)==rhs;
} // }}}
shared_ptr<Bool> Session::operator<(const Value &rhs) const // {{{
{ return Bool::GetInstance(false);
} // }}}
shared_ptr<Bool> Session::operator>=(const Value &rhs) const // {{{
{ return (*this)==rhs;
} // }}}
shared_ptr<Bool> Session::operator>(const Value &rhs) const // {{{
{ return Bool::GetInstance(false);
} // }}}

}
