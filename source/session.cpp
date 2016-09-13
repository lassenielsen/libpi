#include <libpi/session.hpp>
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

void Session::Send(int to, shared_ptr<libpi::Value> value) // {{{
{ if (Closed()) throw string("Session::Send: Trying to use closed session.");
  if (to<0 || to>=GetActors()) throw string("Session::Send: to must be between 0 and actors-1");
  myOutChannels[to]->Send(value);
} //}}}

shared_ptr<libpi::Value> Session::Receive(int from) // {{{
{ if (Closed()) throw string("Session::Receive: Trying to use closed session.");
  if (from<0 || from>=GetActors()) throw string("Session::Receive: to must be between 0 and actors-1");
  return myInChannels[from]->Receive();
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

void Session::ToString(ostream &dest) const // {{{
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

}
