#include <libpi/channel_mq.hpp>
#include <iostream>
#include <sstream>

using namespace libpi;
using namespace std;

int main(int argc, char **argv)
{ // Testing standard message passing with short message {{{
  { string shortmsg="Hello World";
    Channel_MQ *c1;
    try
    { c1=new Channel_MQ();
    } catch (string s)
    { cerr << "Error in channel creation." << endl
           << "Message was: " << s << endl;
      return 1;
    }
    try
    { Message msg;
      msg.AddData(shortmsg.c_str(),shortmsg.size()+1);
      c1->Send(msg);
      cout << "Short message sent." << endl;
    } catch (string s)
    { cerr << "Error sending message." << endl
           << "Message was: " << s << endl;
      return 1;
    }
    try
    { Message msg;
      c1->Receive(msg);
      string msgstr=msg.GetData();
      if (msgstr==shortmsg)
        cout << "Received short message correctly." << endl;
      else
        throw (string)"Received wrong message:\n"
                    + "Original Message: " + shortmsg + "\n"
                    + "Received Message: " + msgstr;
    } catch (string s)
    { cerr << "Error receiving message." << endl
           << "Message was: " << s << endl;
      return 1;
    }
    c1->Unlink();
    delete c1;
  }
  // }}}
  // Testing standard message passing withlong message {{{
  { stringstream longmsg;
    for (int i=0; i<1024*32; ++i)
      longmsg<<(char)('a'+(i%20));
    Channel_MQ *c1;
    try
    { c1=new Channel_MQ();
    } catch (string s)
    { cerr << "Error in channel creation." << endl
           << "Message was: " << s << endl;
      return 1;
    }
    try
    { Message msg;
      msg.AddData(longmsg.str().c_str(),longmsg.str().size()+1);
      c1->Send(msg);
      cout << "Long message sent." << endl;
    } catch (string s)
    { cerr << "Error sending message." << endl
           << "Message was: " << s << endl;
      return 1;
    }
    try
    { Message msg;
      c1->Receive(msg);
      string msgstr=msg.GetData();
      if (msgstr==longmsg.str())
        cout << "Received long message correctly." << endl;
      else
        throw (string)"Received wrong message:\n"
                    + "Original Message: " + longmsg.str() + "\n"
                    + "Received Message: " + msgstr;
    } catch (string s)
    { cerr << "Error receiving message." << endl
           << "Message was: " << s << endl;
      return 1;
    }
    c1->Unlink();
    delete c1;
  }
  // }}}
  // Testing single-message passing with short message {{{
  { string shortmsg="Hello World";
    Channel_MQ *c1;
    try
    { c1=new Channel_MQ();
    } catch (string s)
    { cerr << "Error in channel creation." << endl
           << "Message was: " << s << endl;
      return 1;
    }
    try
    { Message msg;
      msg.AddData(shortmsg.c_str(),shortmsg.size()+1);
      c1->SingleSend(msg);
      cout << "Short message sent." << endl;
    } catch (string s)
    { cerr << "Error sending message." << endl
           << "Message was: " << s << endl;
      return 1;
    }
    try
    { Message msg;
      c1->SingleReceive(msg);
      string msgstr=msg.GetData();
      if (msgstr==shortmsg)
        cout << "Received short message correctly." << endl;
      else
        throw (string)"Received wrong message:\n"
                    + "Original Message: " + shortmsg + "\n"
                    + "Received Message: " + msgstr;
    } catch (string s)
    { cerr << "Error receiving message." << endl
           << "Message was: " << s << endl;
      return 1;
    }
    c1->Unlink();
    delete c1;
  }
  // }}}
  // Testing single-message passing with long message {{{
  { stringstream longmsg;
    for (int i=0; i<1024*32; ++i)
      longmsg<<(char)('a'+(i%20));
    Channel_MQ *c1;
    try
    { c1=new Channel_MQ();
    } catch (string s)
    { cerr << "Error in channel creation." << endl
           << "Message was: " << s << endl;
      return 1;
    }
    try
    { Message msg;
      msg.AddData(longmsg.str().c_str(),longmsg.str().size()+1);
      c1->SingleSend(msg);
      cerr << "Error: Long message was sent without warning!" << endl;
      return 1;
    } catch (string s)
    { cout << "Message size violation detected." << endl;
    }
    c1->Unlink();
    delete c1;
  }
  // }}}
  return 0;
}
