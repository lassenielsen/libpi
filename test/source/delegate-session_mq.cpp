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
      { vector<Channel_MQ> link_1;
        link_1.push_back(Channel_MQ(10001));
        link_1[0].Unlink();
        cout << "Linking on 10001 as participant 0" << endl;
        Session_MQ s1(link_1,0,2);
        cout << "Linking on 10001 as participant 0 complete (s1)" << endl;
        vector<Channel_MQ> link_2;
        link_2.push_back(Channel_MQ(20001));
        link_2[0].Unlink();
        cout << "Linking on 20001 as participant 0" << endl;
        Session_MQ s2(link_2,0,2);
        cout << "Linking on 20001 as participant 0 complete (s2)" << endl;
        cout << "Delegating s2@(1of2) over s1@(1of2) to participant 1(2of2)" << endl;
        s1.Delegate(1,s2);
        cout << "Delegating s2@(1of2) over s1@(1of2) to participant 1(2of2) complete" << endl;
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
          { vector<Channel_MQ> link_1;
            link_1.push_back(Channel_MQ(10001));
            cout << "Linking on 10001 as participant 1" << endl;
            Session_MQ s1(link_1,1,2);
            cout << "Linking on 10001 as participant 1 complete (s1)" << endl;
            cout << "Receiving session over s1" << endl;
            Session *s2=s1.ReceiveSession(0);
            cout << "Receiving session over s1 complete" << endl;
            Message msg;
            string strmsg0="session received";
            msg.AddData(strmsg0.c_str(),strmsg0.size()+1);
            s2->Send(1,msg);
            cout << "Sent. " << strmsg0 << " to participant 1 on received session" << endl;
            msg.Clear();
            delete s2;
          } catch (string s)
          { cerr << "Error in child (1) process." << endl
                 << "Message was: " << s << endl;
            return 1;
          }
          break;
        default: // parent
          try
          { vector<Channel_MQ> link_2;
            link_2.push_back(Channel_MQ(20001));
            cout << "Linking on 20001 as participant 1" << endl;
            Session_MQ s2(link_2,1,2);
            cout << "Linking on 20001 as participant 1 complete (s2)" << endl;
            Message msg;
            s2.Receive(0,msg);
            cout << "Participant 1 received message: " << msg.GetData() << endl;
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
