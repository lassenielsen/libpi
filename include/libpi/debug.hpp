#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <map>
#include <pthread.h>
#include <string>
#include <time.h>
#include <sstream>
#include <iostream>

namespace debug {

#ifdef CWDEBUG
std::map<int,int> _indent;
inline std::string indent(int delta=0) // {{{
{
  _indent[pthread_self()]+=delta;
  return delta>=0?std::string(_indent[pthread_self()]-delta,' '):std::string(_indent[pthread_self()],' ');
} // }}}

inline std::string timestamp() // {{{
{
  time_t now;
  time(&now);
  struct tm *t=gmtime(&now);
  std::stringstream result;
  result << t->tm_year << "-" << t->tm_mon << "-" << t->tm_mday << "-" << t->tm_hour << "-" << t->tm_min << "-" << t->tm_sec;
  return result.str();
} // }}}

#define _DEBUG_HEADER "[AT " << debug::timestamp() << "](PROCESS " << getpid() << ", THREAD " << pthread_self() << ")"
#define INFOMSG(msg) std::cerr << _DEBUG_HEADER << debug::indent() << msg << std::endl
#define WARNMSG(msg) INFOMSG(msg)
#define ERRORMSG(msg) INFOMSG(msg)
#define ENTER(msg) std::cerr << _DEBUG_HEADER << debug::indent(2) << "Entering " << msg << std::endl
#define LEAVE(msg) std::cerr << _DEBUG_HEADER << debug::indent(-2) << "Leaving " << msg << std::endl
#define DISPLAY(var) INFOMSG(std::string("VALUE OF ") + #var + " IS " + string(var))

class Scope // {{{
{ public:
    Scope(const std::string &name) // {{{
    : myName(name)
    { ENTER(myName);
    } // }}}
    ~Scope() // {{{
    { LEAVE(myName);
    } // }}}

  private:
    std::string myName;
}; // }}}

#define SCOPE(name) debug::Scope _scope(name);
#else
#define INFOMSG(msg)
#define WARNMSG(msg)
#define ERRORMSG(msg)
#define ENTER(msg)
#define LEAVE(msg)
#define DISPLAY(var)
#define SCOPE(name)
#endif
}

#endif
