#include <libpi/task/link.hpp>
#include <libpi/bool.hpp>

using namespace std;

namespace libpi {
  namespace task {

Link::Link(int actors, libpi::gc::GCRegistrant *registrant) // {{{
: libpi::Link(registrant)
{ if (actors<2)
    throw string("libpi::task::Link constructor: Numbor of actors is too small");
  for (int i=1; i<actors; ++i)
    myChannels.push_back(new Channel(registrant));
} // }}}

Link::~Link() // {{{
{
} // }}}

string Link::GetType() const // {{{
{ throw "libpi::task::Link is not serializable";
} // }}}

void Link::ToStream(ostream &dest) const // {{{
{ throw "libpi::task::Link is not serializable";
} // }}}

Bool *Link::operator==(const Value &rhs) const // {{{
{ const Link *rhsptr=dynamic_cast<const Link*>(&rhs);
  if (rhsptr==NULL)
    return Bool::GetInstance(false);
  if (rhsptr->GetChannels().size()!=GetChannels().size())
    return Bool::GetInstance(false);
  for (int i=0; i<GetChannels().size(); ++i)
  { Bool *eltResult=(*GetChannels()[i])==(*rhsptr->GetChannels()[i]);
    if (!(eltResult->GetValue()))
      return Bool::GetInstance(false);
  }
  return Bool::GetInstance(true);
} // }}}
Bool *Link::operator<=(const Value &rhs) const // {{{
{ return (*this)==rhs;
} // }}}
Bool *Link::operator<(const Value &rhs) const // {{{
{ return Bool::GetInstance(false);
} // }}}
Bool *Link::operator>=(const Value &rhs) const // {{{
{ return (*this)==rhs;
} // }}}
Bool *Link::operator>(const Value &rhs) const // {{{
{ return Bool::GetInstance(false);
} // }}}

Session *Link::Connect(int pid, int actors, libpi::gc::GCRegistrant *registrant) // {{{
{ if (actors!=myChannels.size()+1)
    throw string("libpi::task::Link::Connect: Wrong number of channels provided");
  if (pid<0 || actors<=pid)
    throw string("libpi::task::Link::Connect: pid must be between 0 and actors-1.");

  // Create vectors for session channels
  vector<libpi::Channel* > inChannels;
  vector<libpi::Channel* > outChannels;

  // Create receiving session-channels
  for (int i=0; i<actors; ++i)
  { inChannels.push_back(new Channel(registrant));
  }

  if (pid==0) // Orchestrate session initiation
  { outChannels.push_back(inChannels[pid]);
    for (int actor=1; actor<actors; ++actor) // Receive channels from all actors
    { Value *val=myChannels[actor-1]->SingleReceive(registrant);
      libpi::Channel *ch=dynamic_cast<libpi::Channel*>(val);
      if (!ch)
        throw string("libpi::task::Link Received non-channel during connecting");
      outChannels.push_back(ch);
    }
    for (int actor=1; actor<actors; ++actor) // Send own reception-channels
    { outChannels[actor]->SingleSend(inChannels[actor]);
    }
    for (int actor=1; actor<actors; ++actor) // Receive channels from all actors
    { for (int actor2=1; actor2<actors; ++actor2)
      { if (actor==actor2)
          continue; // Skip distribution when sender and receiver is the same
        Value *val=inChannels[actor]->SingleReceive(registrant);
        outChannels[actor2]->SingleSend(val);
      }
    }
    // Session channels has been established!
  }
  else
  { myChannels[pid-1]->SingleSend(inChannels.front());
    Value *val=inChannels.front()->SingleReceive(registrant);
    libpi::Channel *ch=dynamic_cast<libpi::Channel*>(val);
    if (!ch)
      throw string("libpi::task::Link Received non-channel during connecting");
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
      Value *val=inChannels.front()->SingleReceive(registrant);
      libpi::Channel *ch=dynamic_cast<libpi::Channel*>(val);
      if (!ch)
        throw string("libpi::task::Link Received non-channel during connecting");
      outChannels.push_back(ch);
    }
    // Session channels has been established!
  }
  return new Session(pid,actors,inChannels,outChannels, registrant);
} // }}}

    void Link::Mark(unordered_set<Value*> &marks) // {{{
    { if (marks.count(this)>0)
        return;

      for (auto it=myChannels.begin(); it!=myChannels.end(); ++it)
        (*it)->Mark(marks);
    } // }}}
  }
}
