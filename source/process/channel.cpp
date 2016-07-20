#include <libpi/debug.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <libpi/process/channel.hpp>
#include "common.cpp"

#include <iostream>
#include <string.h>
#include <sys/resource.h>
#include <stdio.h>

using namespace std;

namespace libpi {
  namespace process {

const int Channel::ourMessageLength=1024*8;

Channel::Channel(const string &path) // {{{
: myUnlink(false)
{
  SCOPE(string("Channel::Channel"));
  DISPLAY(path);
  if (path=="") // Need to create path and fifo file {{{
  {
    INFOMSG("Creating FIFO");
    myPath=tempnam(NULL, "fifo");
    DISPLAY(myPath);
    int success=mknod(myPath.c_str(), S_IFIFO | O_EXCL | 0666 , 0); // FIXME: Consider security
    if (success!=0)
    {
      ERRORMSG(string("Channel::Channel Error - Unable to create fifo channel: ") + myPath + "\n"
             + "Error message: " + strerror(errno));
      throw (string)"Unable to create fifo channek: " + myPath + "\n"
                  + "Error was: " + strerror(errno);
    }
  } // }}}
  else // Simply use provided path {{{
  {
    myPath=path;
  } // }}}
  myStream.open(myPath.c_str());
  if (!myStream.is_open()) // Unsuccessful {{{
  {
    ERRORMSG(string("Unable open path: ") + myPath);
    throw string("Channel::Channel Error - Unable open path: ") + myPath;
  } // }}}
} // }}}

Channel::Channel(const Channel &init) // {{{
: myUnlink(false)
, myPath(init.GetPath())
{
  SCOPE(string("Channel::Channel"));
  DISPLAY(init.GetAddress());
  myStream.open(myPath.c_str());
} // }}}

Channel &Channel::operator=(const Channel &rhs) // {{{
{
  SCOPE(string("Channel::operator="));
  DISPLAY(rhs.GetAddress());
  myStream.close();
  myPath=rhs.GetPath();
  myStream.open(myPath.c_str());
} // }}}

Channel *Channel::Copy() const // {{{
{ return new Channel(*this);
} // }}}

Channel::~Channel() // {{{
{
  SCOPE(string("Channel::~Channel(") + myPath + ")");
  myStream.close();
  if (myUnlink)
    remove(myPath.c_str());
} // }}}

void Channel::Unlink() // {{{
{
  SCOPE(string("Channel::Unlink"));
  DISPLAY(myPath);
  myUnlink=true;
} // }}}

void Channel::Send(Message &msg) // {{{
{
  myStream.close();
  myStream.open(myPath.c_str()); // FIXME: otherwise chanse of segfault
  SCOPE(string("Channel::Send"));
  DISPLAY(myPath);
  DISPLAY(msg.GetData());
  myStream << msg.GetSize() << endl;
  myStream.write(msg.GetData(),msg.GetSize());
  myStream << endl;
} // }}}

void Channel::Receive(Message &msg) // {{{
{
  SCOPE(string("Channel::Receive"));
  DISPLAY(myPath);
  long size;
  INFOMSG("RECEIVE SIZE");
  myStream>>size;
  DISPLAY(int2str(size));
  INFOMSG("READ NEWLINE");
  char newline;
  myStream.read(&newline,1); // read newline
  INFOMSG("DONE");
  char *data=new char[size];
  INFOMSG("READ DATA CHUNK");
  myStream.read(data,size);
  INFOMSG("DONE");
  msg.AddData(data,size);
  INFOMSG("READ NEWLINE");
  myStream.read(&newline,1); // read newline
  INFOMSG("DONE");
  delete [] data;
} // }}}

void Channel::SingleSend(Message &msg) // {{{
{
  SCOPE(string("Channel::SingleSend"));
  DISPLAY(msg.GetData());
  DISPLAY(myPath);
  string msgSize=int2str(ourMessageLength-6); // FIXME: hardcoded ceil(ourMessageLength)+2
  DISPLAY(msgSize);
  string dataSize=int2str(msg.GetSize());
  DISPLAY(dataSize);
  char *buffer=new char[ourMessageLength];
  memcpy(buffer,msgSize.c_str(),msgSize.size()); // No \0 !
  buffer[msgSize.size()]='\n';
  memcpy(buffer+msgSize.size()+1,dataSize.c_str(),dataSize.size()); // No \0 !
  buffer[msgSize.size()+1+dataSize.size()]='\n';
  buffer[ourMessageLength-1]='\n';
  memcpy(buffer+msgSize.size()+1+dataSize.size()+1,msg.GetData(),msg.GetSize());
  DISPLAY(buffer);

  myStream.close();
  myStream.open(myPath.c_str()); // FIXME: otherwise risk of segfault
  myStream.write(buffer,ourMessageLength);

  INFOMSG(string("Sent buffer: ") + buffer);
  delete [] buffer;
} // }}}

void Channel::SingleReceive(Message &msg) // {{{
{
  SCOPE(string("Channel::SingleReceive"));
  DISPLAY(myPath);
  char *buffer = new char[ourMessageLength];
  myStream.read(buffer,ourMessageLength);
  INFOMSG(string("Received buffer: ") + buffer);
  // Parse message
  int pos=0;
  while (buffer[pos]!='\n' && pos<ourMessageLength)
    ++pos;
  if (buffer[pos]!='\n')
  {
    ERRORMSG("ILL FORMATED BUFFER");
    throw string("Channel::SingleReceive: Ill formatted buffer");
  }
  buffer[pos]='\0';
  string msgLength(buffer);
  int data_start=++pos;
  while (buffer[pos]!='\n' && pos<ourMessageLength)
    ++pos;
  if (buffer[pos]!='\n')
  {
    ERRORMSG("ILL FORMATED BUFFER");
    throw string("Channel::SingleReceive: Ill formatted buffer");
  }
  buffer[pos]='\0';
  string dataSize(buffer+data_start);
  msg.AddData(buffer+pos+1,str2int(dataSize));
  delete [] buffer;
  DISPLAY(msg.GetData());
} // }}}

string Channel::GetAddress() const // {{{
{ return myPath;
} // }}}

string Channel::GetPath() const // {{{
{ return myPath;
} // }}}

  }
}
