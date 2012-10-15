#include <libpi/session.hpp>
#include <libpi/common.hpp>

using namespace libpi;
using namespace std;

Session::~Session() // {{{
{ Close();
} // }}}

void Session::Send(int to, const Message &msg) // {{{
{ if (Closed()) return;
  if (to<0 || to>=GetActors()) throw "Session::Delegate: to must be between 0 and actors-1";
  myOutChannels[to]->Send(msg);
} //}}}

void Session::Receive(int from, Message &msg) // {{{
{ if (Closed()) return;
  if (from<0 || from>=GetActors()) throw "Session::Delegate: to must be between 0 and actors-1";
  myInChannels[from]->Receive(msg);
} //}}}

void Session::Delegate(int to, Session &s) // {{{
{ if (Closed()) return;
  if (to<0 || to>=GetActors()) throw "Session::Delegate: to must be between 0 and actors-1";
  s.DelegateTo(*myOutChannels[to]);
} //}}}

Session *Session::ReceiveSession(int from) // {{{
{ if (Closed()) return NULL;
  if (from<0 || from>=GetActors()) throw "Session::ReceiveSession: from must be between 0 and actors-1";
  Message addrMsg;
  myInChannels[from]->Receive(addrMsg);
  string addr=addrMsg.GetData();
  return Create(addr);
} //}}}

void Session::Close() // {{{
{ while (myInChannels.size()>0)
  { delete myInChannels.back();
    myInChannels.pop_back();
  }
  while (myOutChannels.size()>0)
  { delete myOutChannels.back();
    myOutChannels.pop_back();
  }
} // }}}

bool Session::Closed() // {{{
{ return myInChannels.size()==0;
} // }}}

int Session::GetActors() // {{{
{ if (Closed()) return 0;
  return myActors;
} // }}}

int Session::GetPid() // {{{
{ if (Closed()) return 0;
  return myPid;
} // }}}

map<string,Session::session_creator> Session::ourSessionCreators;

Session *Session::Create(const string &address) // {{{
{ // Split address into its components
  int pos=address.find("://");
  if (pos<0) throw "Session::Create: address is not formatted correctly, missing ://";
  string protocol=address.substr(0,pos-1);
  string addr=address.substr(pos+3);
  pos=addr.find('@');
  if (pos<0) throw "Session::Create: address is not formatted correctly, missing @";
  string meta=addr.substr(pos+2,addr.size()-pos-2);
  addr=addr.substr(0,pos);
  pos=meta.find(',');
  if (pos<0) throw "Session::Create: address is not formatted correctly, missing , in metadata";
  string pid_str=meta.substr(0,pos);
  string actors_str=meta.substr(pos+1);
  int pid=str2int(pid_str);
  int actors=str2int(actors_str);
  session_creator create=ourSessionCreators[protocol];
  if (create==NULL) throw (string)"Session::Create: Unknown protocol: " + protocol;
  return create(address,pid,actors);
} // }}}
