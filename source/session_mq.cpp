#include <libpi/session_mq.hpp>

using namespace std;
using namespace libpi;

Session_MQ::Session_MQ(vector<Channel_MQ> &chs, int pid, int actors) // {{{
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
      { myOutChannels.push_back(new Channel_MQ(myInChannels[pid].GetAddress()));
        continue; // Skip distribution of own channel
      }
      msg.Clear();
      myInChannels.front().SingleReceive(msg);
      myOutChannels.push_back(new Channel_MQ(msg.GetData()));
    }
    // Session has been established!
  }
} // }}}

Session_MQ::~Session_MQ() // {{{
{
} // }}}
