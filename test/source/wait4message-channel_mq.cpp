#include <libpi/channel_mq.hpp>
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
      { Channel_MQ snd("test");
        Message msg;
        msg.Clear();
        msg.AddData("Ping",5);
        cout << "Child: Sleeping..." << endl;
        usleep(5000000);
        cout << "Child: Sending message: " << msg.GetData() << endl;
        snd.Send(msg);
        msg.Clear();
        msg.AddData("Pong",5);
        cout << "Child: Sleeping..." << endl;
        usleep(5000000);
        cout << "Child: Sending (single) message: " << msg.GetData() << endl;
        snd.SingleSend(msg);
      } catch (string s)
      { cerr << "Error in child process." << endl
             << "Message was: " << s << endl;
        return 1;
      }
      break;
    default: // parent
      try
      { Channel_MQ rcv("test");
        Message msg;
        msg.Clear();
        cout << "Parent: Receiving message..." << endl;
        rcv.Receive(msg);
        cout << "Parent: Received message: " << msg.GetData() << endl;
        msg.Clear();
        cout << "Parent: Receiving (single) message..." << endl;
        rcv.SingleReceive(msg);
        cout << "Parent: Received message: " << msg.GetData() << endl;
        rcv.Unlink();
      } catch (string s)
      { cerr << "Error in parent process." << endl
             << "Message was: " << s << endl;
        return 1;
      }
      break;
  }
  return 0;
}
