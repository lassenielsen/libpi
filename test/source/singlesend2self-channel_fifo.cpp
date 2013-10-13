#include <libpi/channel_fifo.hpp>
#include <iostream>
#include <sstream>

using namespace libpi;
using namespace std;

int main(int argc, char **argv)
{ // Testing standard message passing with short message {{{
  { string shortmsg="Hello World";
    Channel_FIFO *c1;
    try
    { c1=new Channel_FIFO();
    } catch (string s)
    { cerr << "Error in channel creation." << endl
           << "Message was: " << s << endl;
      return 1;
    }
    try // {{{
    { Message msg;
      msg.AddData(shortmsg.c_str(),shortmsg.size()+1);
      c1->SingleSend(msg);
      cout << "Short message sent." << endl;
    } catch (string s)
    { cerr << "Error sending message." << endl
           << "Message was: " << s << endl;
      return 1;
    } // }}}
    try // {{{
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
    } // }}}
    try // {{{
    { Message msg;
      msg.AddData(shortmsg.c_str(),shortmsg.size()+1);
      c1->SingleSend(msg);
      cout << "Short message sent." << endl;
    } catch (string s)
    { cerr << "Error sending message." << endl
           << "Message was: " << s << endl;
      return 1;
    } // }}}
    try // {{{
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
    } // }}}
    c1->Unlink();
    delete c1;
  }
  // }}}
  return 0;
}
