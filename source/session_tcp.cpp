Session::Session(Channel &channel, int pid, int maxpid)
{ if (pid<0 || maxpid<=pid)
    throw "Session Constructor: pid mus be between 0 and maxpid-1";
  else
  { Channel::Create// connect to channel
    TCPsocket socket=connect(channel);
    vector<Channel> inAddresses;
    vector<Channel> outAddresses;
    vector<Channel> cmdAddresses;
    for (int i=0; i<maxpid-1;++i)
    { Channel inAddr;
      string address=inAddr.str();
      send_msg(socket, address.c_str(), address.size()+1);
      inAddresses.push_back(inAddr);
    }
    for (int i=0; i<pid-1;++i)
    { Channel cmdAddr;
      cmdAddresses.push_back(cmdAddr);
    }
    
    // send own channels
    // receive channels for other participants
  }
}
