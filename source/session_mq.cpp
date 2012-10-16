#include <libpi/session_mq.hpp>

Session_MQ::Session_MQ(vector<Channel_MQ> &chs, int pid, int actors) // {{{
{ 
  if (pid<0 || actors<=pid) throw "Session_QM::Session_MQ: pid must be between 0 and actors-1.";
  for (int i=0; i<actors; ++i) // Create receiving session-channels
    myInChannels.push_back(new Channel_MQ());
  for (int i=0; i<actors; ++i) // Send receiving channels 
  if (pid==0) // Orchestrate session initiation
  {
    for (int actor=1; actor<actors; ++actor) // Receive channels from all actors
    {
    }
  }
  else
  {
    Message msg;
    msg.AddData(com.GetAddress().c_str(),com.GetAddress().size());
    chs[pid].SingleSend(msg);
  }
} // }}}

Session_MQ::~Session_MQ() // {{{
{
} // }}}
