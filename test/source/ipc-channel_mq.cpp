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
      { Channel_MQ snd("c2p");
        Channel_MQ rcv("p2c");
        Message msg;
        msg.AddData("Ping",5);
        cout << "Child: Sending message: " << msg.GetData() << endl;
        snd.Send(msg);
        msg.Clear();
        rcv.Receive(msg);
        cout << "Child: Received message: " << msg.GetData() << endl;
        msg.Clear();
        rcv.Receive(msg); // Receive channel address
        cout << "Child: Received address: " << msg.GetData() << endl;
        Channel_MQ del(msg.GetData());
        msg.Clear();
        string s="Delegation succeeded!";
        msg.AddData(s.c_str(),s.size()+1);
        cout << "Child: Sending message on received channel: " << msg.GetData() << endl;
        snd.Send(msg);
        del.Send(msg);
        rcv.Unlink();
      } catch (string s)
      { cerr << "Error in child process." << endl
             << "Message was: " << s << endl;
        return 1;
      }
      break;
    default: // parent
      try
      { Channel_MQ snd("p2c");
        Channel_MQ rcv("c2p");
        Channel_MQ del;
        Message msg;
        msg.AddData("Pong",5);
        cout << "Parent: Sending message: " << msg.GetData() << endl;
        snd.Send(msg);
        msg.Clear();
        rcv.Receive(msg);
        cout << "Parent: Received message: " << msg.GetData() << endl;
        msg.Clear();
        msg.AddData(del.GetAddress().c_str(),del.GetAddress().size()+1);
        cout << "Parent: Sending address: " << msg.GetData() << endl;
        snd.Send(msg);
        msg.Clear();
        del.Receive(msg);
        cout << "Parent: Received message on transmitted channel: " << msg.GetData() << endl;
        rcv.Unlink();
        del.Unlink();
      } catch (string s)
      { cerr << "Error in parent process." << endl
             << "Message was: " << s << endl;
        return 1;
      }
      break;
  }
  return 0;
}
