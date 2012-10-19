#include <libpi/session_mq.hpp>
#include <sstream>

using namespace std;
using namespace libpi;

Session_MQ::Session_MQ(vector<Channel_MQ> &chs, int pid, int actors) // {{{
: Session(pid,actors)
{ 
  if (pid<0 || actors<=pid) throw "Session_QM::Session_MQ: pid must be between 0 and actors-1.";
  for (int i=0; i<actors; ++i) // Create receiving session-channels
    myInChannels.push_back(new Channel_MQ());
  Message msg;
  if (pid==0) // Orchestrate session initiation
  {
    myOutChannels.push_back(new Channel_MQ(myInChannels[pid]->GetAddress()));
    for (int actor=1; actor<actors; ++actor) // Receive channels from all actors
    {
      msg.Clear();
      chs[actor].SingleReceive(msg);
      myOutChannels.push_back(new Channel_MQ(msg.GetData()));
    }
    for (int actor=1; actor<actors; ++actor) // Send own reception-channels
    {
      msg.Clear();
      msg.AddData(myInChannels[actor]->GetAddress().c_str(),
                  myInChannels[actor]->GetAddress().size());
      myOutChannels[actor]->SingleSend(msg);
    }
    for (int actor=1; actor<actors; ++actor) // Receive channels from all actors
    { for (int actor2=1; actor2<actors; ++actor2)
      { if (actor==actor2)
          continue; // Skip distribution when sender and receiver is the same
        msg.Clear();
        myInChannels[actor]->SingleReceive(msg);
        myOutChannels[actor2]->SingleSend(msg);
      }
    }
  }
  else
  {
    msg.Clear();
    msg.AddData(myInChannels.front()->GetAddress().c_str(),
                myInChannels.front()->GetAddress().size());
    chs[pid].SingleSend(msg);
    msg.Clear();
    myInChannels.front()->SingleReceive(msg);
    myOutChannels.push_back(new Channel_MQ(msg.GetData()));
    for (int actor=1; actor<actors; ++actor) // Send remaining reception-channels
    { if (pid==actor)
        continue; // Skip distribution of own channel
      msg.Clear();
      msg.AddData(myInChannels[actor]->GetAddress().c_str(),
                  myInChannels[actor]->GetAddress().size());
      myOutChannels.front()->SingleSend(msg);
    }
    for (int actor=1; actor<actors; ++actor) // Receive remaining transmission-channels
    { if (pid==actor)
      { myOutChannels.push_back(new Channel_MQ(myInChannels[pid]->GetAddress()));
        continue; // Skip distribution of own channel
      }
      msg.Clear();
      myInChannels.front()->SingleReceive(msg);
      myOutChannels.push_back(new Channel_MQ(msg.GetData()));
    }
    // Session has been established!
  }
} // }}}

Session_MQ::Session_MQ(vector<Channel_MQ*> &inChannels, vector<Channel_MQ*> &outChannels, int pid, int actors) // {{{
: Session(pid,actors)
, myInChannels(inChannels)
, myOutChannels(outChannels)
{ 
} // }}}

Session_MQ::~Session_MQ() // {{{
{ Close();
} // }}}

void Session_MQ::Send(int to, Message &msg) // {{{
{ if (Closed()) throw "Session_MQ::Send: Trying to use closed session.";
  if (to<0 || to>=GetActors()) throw "Session_MQ::Send: to must be between 0 and actors-1";
  myOutChannels[to]->Send(msg);
} //}}}

void Session_MQ::Receive(int from, Message &msg) // {{{
{ if (Closed()) throw "Session_MQ::Receive: Trying to use closed session.";
  if (from<0 || from>=GetActors()) throw "Session_MQ::Receive: to must be between 0 and actors-1";
  myInChannels[from]->Receive(msg);
} //}}}

void Session_MQ::Delegate(int to, Session &s) // {{{
{ if (Closed()) throw "Session_MQ::Delegate: Trying to use closed session.";
  if (to<0 || to>=GetActors()) throw "Session::Delegate: to must be between 0 and actors-1";
  s.DelegateTo(*myOutChannels[to]);
} //}}}

void Session_MQ::DelegateTo(Channel &to) // {{{
{ stringstream ss;
  ss << "mqdel://";
  for (int i=0; i<GetActors(); ++i)
  { if (i>0)
      ss << ",";
    ss << myInChannels[i]->GetAddress();
  }
  for (int i=0; i<GetActors(); ++i)
  { ss << ",";
    ss << myOutChannels[i]->GetAddress();
  }
  ss << "@(";
  ss << GetPid();
  ss << ",";
  ss << GetActors();
  ss << ")";
  Message msg;
  msg.AddData(ss.str().c_str(),ss.str().size());
  to.Send(msg);
  Close();
} // }}}

Session *Session_MQ::ReceiveSession(int from) // {{{
{ if (Closed()) throw "Session_MQ::ReceiveSession: Trying to use closed session.";
  if (from<0 || from>=GetActors()) throw "Session::ReceiveSession: from must be between 0 and actors-1";
  Message addrMsg;
  myInChannels[from]->Receive(addrMsg);
  string addr=addrMsg.GetData();
  return Create(addr);
} //}}}

void Session_MQ::Close() // {{{
{ while (myInChannels.size()>0)
  { delete myInChannels.back();
    myInChannels.pop_back();
  }
  while (myOutChannels.size()>0)
  { delete myOutChannels.back();
    myOutChannels.pop_back();
  }
  Session::Close();
} // }}}

Session *Session_MQ::creator_del(string address, int pid, int actors) // {{{
{ vector<Channel_MQ*> inChannels;
  vector<Channel_MQ*> outChannels;
  // Create receiving channels from addresses
  for (int i=0; i<actors; ++i)
  { int pos=address.find(',');
    if (pos<0) throw "Session_MQ::creator_del: Address bad format.";
    inChannels.push_back(new Channel_MQ(address.substr(0,pos)));
    address=address.substr(pos+1);
  }
  // Create sending channels from addresses
  for (int i=0; i<actors-1; ++i)
  { int pos=address.find(',');
    if (pos<0) throw "Session_MQ::creator_del: Address bad format.";
    outChannels.push_back(new Channel_MQ(address.substr(0,pos)));
    address=address.substr(pos+1);
  }
  outChannels.push_back(new Channel_MQ(address));
  // Return session with created channels
  return new Session_MQ(inChannels,outChannels,pid,actors);
} // }}}

int _1=Session::RegisterSessionCreator("mqdel",Session_MQ::creator_del);
