#include <libpi/session_fifo.hpp>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>

using namespace libpi;
using namespace std;

int main(int argc, char **argv)
{ system("mkfifo ./ch1");
  system("mkfifo ./ch2");
  pid_t pid=fork();
  switch (pid)
  { case -1: // Error {{{
      cerr << "Error during fork!" << endl;
      return 1; // }}}
    case 0: // child process {{{
      try
      { vector<Channel_FIFO> link;
        link.push_back(Channel_FIFO("./ch1"));
        link.push_back(Channel_FIFO("./ch2"));
        Session_FIFO s(link,0,3);
        Message msg;
        string strmsg1="P0->P1";
        msg.AddData(strmsg1.c_str(),strmsg1.size()+1);
        s.Send(1,msg);
        cout << "Sent: " << strmsg1 << " to participant 1" << endl;
        msg.Clear();
        string strmsg2="P0->P2";
        msg.AddData(strmsg2.c_str(),strmsg2.size()+1);
        s.Send(2,msg);
        cout << "Sent: " << strmsg2 << " to participant 2" << endl;
        msg.Clear();
        s.Receive(1,msg);
        cout << "Participant 0 received message: " << msg.GetData() << endl;
        msg.Clear();
        s.Receive(2,msg);
        cout << "Participant 0 received message: " << msg.GetData() << endl;
      } catch (string s)
      { cerr << "Error in child (0) process." << endl
             << "Message was: " << s << endl;
        return 1;
      }
      break; // }}}
    default: // parent {{{
      pid=fork();
      switch (pid)
      { case -1: // Error {{{
          cerr << "Error during fork!" << endl;
          return 1; // }}}
        case 0: // child process // {{{
          try
          { vector<Channel_FIFO> link;
            link.push_back(Channel_FIFO("./ch1"));
            link.push_back(Channel_FIFO("./ch2"));
            cout << "PGM: Linking as participant 1" << endl;
            Session_FIFO s(link,1,3);
            Message msg;
            string strmsg0="P1->P0";
            msg.AddData(strmsg0.c_str(),strmsg0.size()+1);
            s.Send(0,msg);
            cout << "Sent. " << strmsg0 << " to participant 0" << endl;
            msg.Clear();
            string strmsg2="P1->P2";
            msg.AddData(strmsg2.c_str(),strmsg2.size()+1);
            s.Send(2,msg);
            cout << "Sent. " << strmsg2 << " to participant 2" << endl;
            msg.Clear();
            s.Receive(0,msg);
            cout << "Participant 1 received message: " << msg.GetData() << endl;
            msg.Clear();
            s.Receive(2,msg);
            cout << "Participant 1 received message: " << msg.GetData() << endl;
          } catch (string s)
          { cerr << "Error in child (1) process." << endl
                 << "Message was: " << s << endl;
            return 1;
          }
          break; // }}}
        default: // parent {{{
          try
          { vector<Channel_FIFO> link;
            link.push_back(Channel_FIFO("./ch1"));
            link.push_back(Channel_FIFO("./ch2"));
            link[0].Unlink();
            link[1].Unlink();
            Session_FIFO s(link,2,3);
            Message msg;
            string strmsg0="P2->P0";
            msg.AddData(strmsg0.c_str(),strmsg0.size()+1);
            s.Send(0,msg);
            cout << "Sent. " << strmsg0 << " to participant 0" << endl;
            msg.Clear();
            string strmsg1="P2->P1";
            msg.AddData(strmsg1.c_str(),strmsg1.size()+1);
            s.Send(1,msg);
            cout << "Sent. " << strmsg1 << " to participant 2" << endl;
            msg.Clear();
            s.Receive(0,msg);
            cout << "Participant 2 received message: " << msg.GetData() << endl;
            msg.Clear();
            s.Receive(1,msg);
            cout << "Participant 2 received message: " << msg.GetData() << endl;
          } catch (string s)
          { cerr << "Error in parrent (2) process." << endl
                 << "Message was: " << s << endl;
            return 1;
          }
          break; // }}}
      }
      break; // }}}
  }
  return 0;
}
