#include <libpi/session.hpp>
#include <libpi/bool.hpp>
#include "common.cpp"

using namespace std;

namespace libpi
{

Session::Session(int pid, int actors, std::vector<Channel*> &inChannels, std::vector<Channel*> &outChannels, gc::GCRegistrant *registrant) // {{{
: Value(registrant),
  myPid(pid),
  myActors(actors),
  myInChannels(inChannels),
  myOutChannels(outChannels)
{
} // }}}

Session::~Session() // {{{
{ if (Closed())
    DeleteVector(myInChannels);
  // Do not delete outChannels as they may be in use, and will be
  // deleted by receiver
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

libpi::Value *Session::Receive(int from, gc::GCRegistrant *registrant) // {{{
{ if (Closed()) throw string("Session::Receive: Trying to use closed session.");
  if (from<0 || from>=GetActors()) throw string("Session::Receive: to must be between 0 and actors-1");
  return myInChannels[from]->Receive(registrant);
} //}}}

bool Session::Receive(int from, task::Task *receiver, libpi::Value *&dest) // {{{
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

Bool *Session::operator==(const Value &rhs) const // {{{
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
Bool *Session::operator<=(const Value &rhs) const // {{{
{ return (*this)==rhs;
} // }}}
Bool *Session::operator<(const Value &rhs) const // {{{
{ return Bool::GetInstance(false);
} // }}}
Bool *Session::operator>=(const Value &rhs) const // {{{
{ return (*this)==rhs;
} // }}}
Bool *Session::operator>(const Value &rhs) const // {{{
{ return Bool::GetInstance(false);
} // }}}

void Session::Mark(unordered_set<Value*> &marks) // {{{
{ if (marks.count(this)>0)
    return;
  marks.insert(this);
  // Deletes channels manualy, but their queued values must be marked
  for (auto it=myInChannels.begin(); it!=myInChannels.end(); ++it)
    (*it)->Mark(marks);
  for (auto it=myOutChannels.begin(); it!=myOutChannels.end(); ++it)
    (*it)->Mark(marks);
} // }}}

}
