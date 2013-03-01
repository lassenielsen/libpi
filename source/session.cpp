#include <libpi/session.hpp>
#include "common.cpp"

using namespace libpi;
using namespace std;

Session::Session(int pid, int actors) // {{{
: myPid(pid),
  myActors(actors)
{
} // }}}

Session::~Session() // {{{
{
} // }}}

void Session::Send(int to, Message &msg) // {{{
{ throw "Session::Send: Trying to send on dummy session.";
} //}}}

void Session::Receive(int from, Message &msg) // {{{
{ throw "Session::Receive: Trying to receive on dummy session.";
} //}}}

void Session::Delegate(int to, Session &s) // {{{
{ throw "Session::Delegate: Trying to delegate over dummy session.";
} //}}}

Session *Session::ReceiveSession(int from) // {{{
{ throw "Session::ReceiveSession: Trying to receive over dummy session.";
} //}}}

void Session::Close() // {{{
{ myActors=0;
  myPid=0;
} // }}}

bool Session::Closed() // {{{
{ return myActors==0;
} // }}}

void Session::DelegateTo(Channel &to) // {{{
{ throw "Session::DelegateTo: Trying to delegate dummy session.";
} // }}}

int Session::GetActors() // {{{
{ return myActors;
} // }}}

int Session::GetPid() // {{{
{ return myPid;
} // }}}

map<string,Session::session_creator> Session::ourSessionCreators;

Session *Session::Create(const string &address) // {{{
{ // Split address into its components
  int pos=address.find("://");
  if (pos<0) throw "Session::Create: address is not formatted correctly, missing ://";
  string protocol=address.substr(0,pos);
  string addr=address.substr(pos+3);
  pos=addr.find('@');
  if (pos<0) throw "Session::Create: address is not formatted correctly, missing @";
  string meta=addr.substr(pos+2,addr.size()-pos-3);
  addr=addr.substr(0,pos);
  pos=meta.find(',');
  if (pos<0) throw "Session::Create: address is not formatted correctly, missing , in metadata";
  string pid_str=meta.substr(0,pos);
  string actors_str=meta.substr(pos+1);
  int pid=str2int(pid_str);
  int actors=str2int(actors_str);
  session_creator create=ourSessionCreators[protocol];
  if (create==NULL) throw (string)"Session::Create: Unknown protocol: " + protocol;
  return create(addr,pid,actors);
} // }}}
int Session::RegisterSessionCreator(string protocol,session_creator creator) // {{{
{ ourSessionCreators[protocol]=creator;
}
