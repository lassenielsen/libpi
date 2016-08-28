#include <libpi/thread/link.hpp>

using namespace std;

namespace libpi {
  namespace thread {

Link::Link(int actors) // {{{
{ if (actors<2)
    throw string("libpi::thread::Link constructor: Numbor of actors is too small");
  for (int i=1; i<actors; ++i)
    myChannels.push_back(shared_ptr<Channel>(new Channel()));
} // }}}

Link::~Link() // {{{
{
} // }}}

string Link::GetType() const // {{{
{ throw "libpi::thread::Link is not serializable";
} // }}}

string Link::ToString() const // {{{
{ throw "libpi::thread::Link is not serializable";
} // }}}

bool Link::operator==(const Value &rhs) const // {{{
{ return (&rhs)==this;
} // }}}

shared_ptr<Session> Link::Connect(int pid, int actors) // {{{
{ if (actors!=myChannels.size()+1)
    throw string("libpi::thread::Link::Connect: Wrong number of channels provided");
  if (pid<0 || actors<=pid)
    throw string("libpi::thread::Link::Connect: pid must be between 0 and actors-1.");

  // Create vectors for session channels
  vector<shared_ptr<libpi::Channel> > inChannels;
  vector<shared_ptr<libpi::Channel> > outChannels;

  // Create receiving session-channels
  for (int i=0; i<actors; ++i)
  { inChannels.push_back(shared_ptr<libpi::Channel>(new Channel()));
  }

  if (pid==0) // Orchestrate session initiation
  { outChannels.push_back(inChannels[pid]);
    for (int actor=1; actor<actors; ++actor) // Receive channels from all actors
    { shared_ptr<Value> val=myChannels[actor-1]->SingleReceive();
      shared_ptr<libpi::Channel> ch=dynamic_pointer_cast<libpi::Channel>(val);
      if (!ch)
        throw string("libpi::thread::Link Received non-channel during connecting");
      outChannels.push_back(ch);
    }
    for (int actor=1; actor<actors; ++actor) // Send own reception-channels
    { outChannels[actor]->SingleSend(inChannels[actor]);
    }
    for (int actor=1; actor<actors; ++actor) // Receive channels from all actors
    { for (int actor2=1; actor2<actors; ++actor2)
      { if (actor==actor2)
          continue; // Skip distribution when sender and receiver is the same
        shared_ptr<Value> val=inChannels[actor]->SingleReceive();
        outChannels[actor2]->SingleSend(val);
      }
    }
    // Session channels has been established!
  }
  else
  { myChannels[pid-1]->SingleSend(inChannels.front());
    shared_ptr<Value> val=inChannels.front()->SingleReceive();
    shared_ptr<libpi::Channel> ch=dynamic_pointer_cast<libpi::Channel>(val);
    if (!ch)
      throw string("libpi::thread::Link Received non-channel during connecting");
    outChannels.push_back(ch);
    for (int actor=1; actor<actors; ++actor) // Send remaining reception-channels
    { if (pid==actor)
        continue; // Skip distribution of own channel
      outChannels.front()->SingleSend(inChannels[actor]);
    }
    for (int actor=1; actor<actors; ++actor) // Receive remaining transmission-channels
    { if (pid==actor)
      { outChannels.push_back(inChannels[pid]);
        continue; // Skip distribution of own channel
      }
      shared_ptr<Value> val=inChannels.front()->SingleReceive();
      shared_ptr<libpi::Channel> ch=dynamic_pointer_cast<libpi::Channel>(val);
      if (!ch)
        throw string("libpi::thread::Link Received non-channel during connecting");
      outChannels.push_back(ch);
    }
    // Session channels has been established!
  }
  return shared_ptr<Session>(new Session(pid,actors,inChannels,outChannels));
} // }}}
  }
}
