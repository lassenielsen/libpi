#include <iostream>
#include <libpi/thread/session.hpp>
#include <sstream>

using namespace std;

namespace libpi
{ namespace thread
  {
// Session(vector) Linking documenatation {{{
/*!
   Session initiation (linking) implementation.
   Protocol for linking on channels c0,,c2,...,cP-1 for participants 0,1,...,P-1:
   // P0 receives outChannels
   1-[c1]->0 s0->1
   2-[c2]->0 s0->2
   ...
   P-1-[cP-1]->0 s0->P-1
   // P0 sends own inChannels
   0-[s0->1]->1 s1->0
   0-[s0->2]->2 s2->0
   ...
   0-[s0->P-1]->P-1 sP-1->0
   // participant 1 sends input channels (via participant 0)
   1-[s1->0]->0 s2->1
   0-[s0->2]->2 s2->1
   1-[s1->0]->0 s3->1
   0-[s0->3]->3 s3->1
   ...
   1-[s1->0]->0 sP-1->1
   0-[s0->P-1]->P-1 sP-1->1
   // participant 2 sends input channels (via participant 0)
   // ...
   // participant P-1 sends input channels (via participant 0)
   // DONE
  */
 // }}}
Session::Session(vector<Channel*> &chs, int pid, int actors) // {{{
: Session(pid,actors)
{ 
  if (pid<0 || actors<=pid) throw "Session_QM::Session: pid must be between 0 and actors-1.";
  for (int i=0; i<actors; ++i) // Create receiving session-channels
  {
    myInChannels.push_back(new Channel_FIFO());
    //cout << "Debug: PID=" << pid << ", created channel: " << myInChannels.back()->GetAddress() << endl;
  }
  Message msg;
  if (pid==0) // Orchestrate session initiation
  {
    myOutChannels.push_back(new Channel_FIFO(myInChannels[pid]->GetAddress()));
    for (int actor=1; actor<actors; ++actor) // Receive channels from all actors
    {
      msg.Clear();
      //cout << "Debug: PID=" << pid << ", receiving on channel: " << chs[actor-1]->GetAddress() << endl;
      chs[actor-1]->SingleReceive(msg);
      //cout << "Debug: PID=" << pid << ", received outChannel: " << msg.GetData() << endl;
      myOutChannels.push_back(new Channel_FIFO(msg.GetData()));
    }
    for (int actor=1; actor<actors; ++actor) // Send own reception-channels
    {
      msg.Clear();
      msg.AddData(myInChannels[actor]->GetAddress().c_str(),
                  myInChannels[actor]->GetAddress().size()+1);
      myOutChannels[actor]->SingleSend(msg);
      //cout << "Debug: PID=" << pid << ", sent inChannel: " << msg.GetData() << endl;
    }
    for (int actor=1; actor<actors; ++actor) // Receive channels from all actors
    { for (int actor2=1; actor2<actors; ++actor2)
      { if (actor==actor2)
          continue; // Skip distribution when sender and receiver is the same
        msg.Clear();
        //cout << "Debug: PID=" << pid << ", receiving on channel: " << myInChannels[actor]->GetAddress() << endl;
        myInChannels[actor]->SingleReceive(msg);
        myOutChannels[actor2]->SingleSend(msg);
        //cout << "Debug: PID=" << pid << ", proxied channel: " << msg.GetData() << "from: " << actor << " to: " << actor2 << endl;
      }
    }
  }
  else
  {
    msg.Clear();
    msg.AddData(myInChannels.front()->GetAddress().c_str(),
                myInChannels.front()->GetAddress().size()+1);
    chs[pid-1]->SingleSend(msg);
    //cout << "Debug: PID=" << pid << ", sent inChannel: " << msg.GetData() << " on " << chs[pid-1]->GetAddress() << endl;
    msg.Clear();
    //cout << "Debug: PID=" << pid << ", receiving on channel: " << myInChannels.front()->GetAddress() << endl;
    myInChannels.front()->SingleReceive(msg);
    myOutChannels.push_back(new Channel_FIFO(msg.GetData()));
    //cout << "Debug: PID=" << pid << ", received outChannel: " << msg.GetData() << endl;
    for (int actor=1; actor<actors; ++actor) // Send remaining reception-channels
    { if (pid==actor)
        continue; // Skip distribution of own channel
      msg.Clear();
      msg.AddData(myInChannels[actor]->GetAddress().c_str(),
                  myInChannels[actor]->GetAddress().size()+1);
      myOutChannels.front()->SingleSend(msg);
      //cout << "Debug: PID=" << pid << ", sent inChannel: " << msg.GetData() << endl;
    }
    for (int actor=1; actor<actors; ++actor) // Receive remaining transmission-channels
    { if (pid==actor)
      { myOutChannels.push_back(new Channel_FIFO(myInChannels[pid]->GetAddress()));
        continue; // Skip distribution of own channel
      }
      msg.Clear();
      //cout << "Debug: PID=" << pid << ", receiving on channel: " << myInChannels.front()->GetAddress() << endl;
      myInChannels.front()->SingleReceive(msg);
      //cout << "Debug: PID=" << pid << ", received outChannel: " << msg.GetData() << endl;
      myOutChannels.push_back(new Channel_FIFO(msg.GetData()));
    }
    // Session has been established!
  }
} // }}}

Session::Session(vector<Channel_FIFO*> &inChannels, vector<Channel_FIFO*> &outChannels, int pid, int actors) // {{{
: Session(pid,actors)
{ 
  for (vector<Channel_FIFO*>::const_iterator inCh=inChannels.begin(); inCh!=inChannels.end(); ++inCh)
    myInChannels.push_back((*inCh)->Copy());
  for (vector<Channel_FIFO*>::const_iterator outCh=outChannels.begin(); outCh!=outChannels.end(); ++outCh)
    myOutChannels.push_back((*outCh)->Copy());
} // }}}

Session::~Session() // {{{
{ Close();
} // }}}

void Session::Send(int to, Message &msg) // {{{
{ if (Closed()) throw string("Session::Send: Trying to use closed session.");
  if (to<0 || to>=GetActors()) throw string("Session::Send: to must be between 0 and actors-1");
  myOutChannels[to]->Send(msg);
} //}}}

void Session::Receive(int from, Message &msg) // {{{
{ if (Closed()) throw string("Session::Receive: Trying to use closed session.");
  if (from<0 || from>=GetActors()) throw string("Session::Receive: to must be between 0 and actors-1");
  myInChannels[from]->Receive(msg);
} //}}}

void Session::Delegate(int to, Session &s) // {{{
{ if (Closed()) throw string("Session::Delegate: Trying to use closed session.");
  if (to<0 || to>=GetActors()) throw string("Session::Delegate: to must be between 0 and actors-1");
  s.DelegateTo(*myOutChannels[to]);
} //}}}

void Session::DelegateTo(Channel &to) // {{{
{ stringstream ss;
  ss << "fifosession://";
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
  msg.AddData(ss.str().c_str(),ss.str().size()+1);
  to.Send(msg);
  Close(false);
} // }}}

Session *Session::ReceiveSession(int from) // {{{
{ if (Closed()) throw string("Session::ReceiveSession: Trying to use closed session.");
  if (from<0 || from>=GetActors()) throw string("Session::ReceiveSession: from must be between 0 and actors-1");
  Message addrMsg;
  myInChannels[from]->Receive(addrMsg);
  string addr=addrMsg.GetData();
  return Create(addr);
} //}}}

void Session::Close(bool unlink) // {{{
{ while (myInChannels.size()>0)
  { if (unlink) myInChannels.back()->Unlink();
    delete myInChannels.back();
    myInChannels.pop_back();
  }
  while (myOutChannels.size()>0)
  { delete myOutChannels.back();
    myOutChannels.pop_back();
  }
  Session::Close(unlink);
} // }}}

Session *Session::creator_del(string address, int pid, int actors) // {{{
{ vector<Channel_FIFO*> inChannels;
  vector<Channel_FIFO*> outChannels;
  // Create receiving channels from addresses
  for (int i=0; i<actors; ++i)
  { int pos=address.find(',');
    if (pos<0) throw "Session::creator_del: Address bad format.";
    inChannels.push_back(new Channel_FIFO(address.substr(0,pos)));
    address=address.substr(pos+1);
  }
  // Create sending channels from addresses
  for (int i=0; i<actors-1; ++i)
  { int pos=address.find(',');
    if (pos<0) throw "Session::creator_del: Address bad format.";
    outChannels.push_back(new Channel_FIFO(address.substr(0,pos)));
    address=address.substr(pos+1);
  }
  outChannels.push_back(new Channel_FIFO(address));
  // Return session with created channels
  return new Session(inChannels,outChannels,pid,actors);
} // }}}

int _2=Session::RegisterSessionCreator("fifosession",Session::creator_del);
  }
}
