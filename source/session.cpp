#include <libpi/session.hpp>
#include <libpi/bool.hpp>
#include "common.cpp"

using namespace std;

namespace libpi
{

Session::Session(int pid, int actors, std::vector<Channel*> &inChannels, std::vector<Channel*> &outChannels) // {{{
: myPid(pid),
  myActors(actors),
  myInChannels(inChannels),
  myOutChannels(outChannels)
{
} // }}}

Session::~Session() // {{{
{
} // }}}

void Session::Send(int to, libpi::Value *value) // {{{
{ if (Closed()) throw string("Session::Send: Trying to use closed session.");
  if (to<0 || to>=GetActors()) throw string("Session::Send: to must be between 0 and actors-1");
  myOutChannels[to]->Send(value);
} //}}}

void Session::Send(int to, task::Task *sender, libpi::Value *value) // {{{
{ if (Closed()) throw string("Session::Send: Trying to use closed session.");
  if (to<0 || to>=GetActors()) throw string("Session::Send: to must be between 0 and actors-1");
  myOutChannels[to]->Send(sender,value);
} //}}}

void Session::Send(int to, task::Task *sender, long int value) // {{{
{ if (Closed()) throw string("Session::Send: Trying to use closed session.");
  if (to<0 || to>=GetActors()) throw string("Session::Send: to must be between 0 and actors-1");
  myOutChannels[to]->Send(sender,value);
} //}}}

libpi::Value *Session::Receive(int from) // {{{
{ if (Closed()) throw string("Session::Receive: Trying to use closed session.");
  if (from<0 || from>=GetActors()) throw string("Session::Receive: to must be between 0 and actors-1");
  return myInChannels[from]->Receive();
} //}}}

bool Session::Receive(int from, task::Task *receiver, libpi::Value **dest) // {{{
{ if (Closed()) throw string("Session::Receive: Trying to use closed session.");
  if (from<0 || from>=GetActors())
    throw string("Session::Receive: to must be between 0 and actors-1");
  return myInChannels[from]->Receive(receiver,dest);
} //}}}

bool Session::Receive(int from, task::Task *receiver, long int *dest) // {{{
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

bool Session::operator==(const Value &rhs) const // {{{
{ const Session *rhsptr=dynamic_cast<const Session*>(&rhs);
  if (rhsptr==NULL)
    return false;
  if (rhsptr->GetPid()!=GetPid())
    return false;
  if (rhsptr->GetActors()!=GetActors())
    return false;
  if (rhsptr->myInChannels!=myInChannels)
    return false;
  if (rhsptr->myOutChannels!=myOutChannels)
    return false;
  return true;

} // }}}
bool Session::operator<=(const Value &rhs) const // {{{
{ return (*this)==rhs;
} // }}}
bool Session::operator<(const Value &rhs) const // {{{
{ return false;
} // }}}
bool Session::operator>=(const Value &rhs) const // {{{
{ return (*this)==rhs;
} // }}}
bool Session::operator>(const Value &rhs) const // {{{
{ return false;
} // }}}

}
