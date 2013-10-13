#include <libpi/channel.hpp>
#include <unistd.h>

using namespace libpi;
using namespace std;

map<string,Channel::channel_creator> Channel::ourChannelCreators;

Channel *Channel::Create(const string &address) // {{{
{ // Split address into its components
  int pos=address.find("://");
  if (pos<0) throw string("Channel::Create: address is not formatted correctly, missing ://");
  string protocol=address.substr(0,pos-1);
  string addr=address.substr(pos+3);
  channel_creator create=ourChannelCreators[protocol];
  if (create==NULL) throw (string)"Channel::Create: Unknown protocol: " + protocol;
  return create(address);
} // }}}
