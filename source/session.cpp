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

int Session::GetActors() // {{{
{ return myActors;
} // }}}

int Session::GetPid() // {{{
{ return myPid;
} // }}}

//map<string,Session::session_creator> Session::ourSessionCreators;

//Session *Session::Create(const string &address) // {{{
//{ // Split address into its components
//  int pos=address.find("://");
//  if (pos<0) throw "Session::Create: address is not formatted correctly, missing ://";
//  string protocol=address.substr(0,pos);
//  string addr=address.substr(pos+3);
//  pos=addr.find('@');
//  if (pos<0) throw "Session::Create: address is not formatted correctly, missing @";
//  string meta=addr.substr(pos+2,addr.size()-pos-3);
//  addr=addr.substr(0,pos);
//  pos=meta.find(',');
//  if (pos<0) throw "Session::Create: address is not formatted correctly, missing , in metadata";
//  string pid_str=meta.substr(0,pos);
//  string actors_str=meta.substr(pos+1);
//  int pid=str2int(pid_str);
//  int actors=str2int(actors_str);
//  session_creator create=ourSessionCreators[protocol];
//  if (create==NULL) throw (string)"Session::Create: Unknown protocol: " + protocol;
//  return create(addr,pid,actors);
//} // }}}
//int Session::RegisterSessionCreator(string protocol,session_creator creator) // {{{
//{ ourSessionCreators[protocol]=creator;
//}
//
}
