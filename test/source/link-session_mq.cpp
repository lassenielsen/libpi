#include <libpi/session_mq.hpp>
#include <iostream>
#include <sstream>
#include <unistd.h>

using namespace libpi;
using namespace std;

int main(int argc, char **argv)
{ pid_t pid=fork();
  switch (pid)
  { case -1: // Error
      cerr << "Error during fork!" << endl;
      return 1;
    case 0: // child process
      try
      { vector<Channel_MQ> link;
        link.push_back(Channel_MQ(10001));
        link.push_back(Channel_MQ(10002));
        Session_MQ s(link,0,3);
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
      break;
    default: // parent
      pid=fork();
      switch (pid)
      { case -1: // Error
          cerr << "Error during fork!" << endl;
          return 1;
        case 0: // child process
          try
          { vector<Channel_MQ> link;
            link.push_back(Channel_MQ(10001));
            link.push_back(Channel_MQ(10002));
            cout << "PGM: Linking as participant 1" << endl;
            Session_MQ s(link,1,3);
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
          break;
        default: // parent
          try
          { vector<Channel_MQ> link;
            link.push_back(Channel_MQ(10001));
            link.push_back(Channel_MQ(10002));
            link[0].Unlink();
            link[1].Unlink();
            link[2].Unlink();
            Session_MQ s(link,2,3);
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
          break;
      }
      break;
  }
  return 0;
}
