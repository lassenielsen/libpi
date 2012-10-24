#include <libpi/channel_mq.hpp>
#include <iostream>

using namespace libpi;
using namespace std;

int main(int argc, char **argv)
{ Channel_MQ c1;
  string msg_str="Hello World";
  Message msg;
  msg.AddData(msg_str.c_str(),msg_str.size()+1);
  c1.Send(msg);
  Message msg2;
  c1.Receive(msg2);
  cout << "Received message: " << msg2.GetData() << endl;
  return 0;
}
