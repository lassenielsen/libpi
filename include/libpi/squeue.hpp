#pragma once
#include <string>

typedef unsigned long int size_t;

template<typename _Tp>
class squeue
{ public:
    squeue()
    : myValues(nullptr)
    , myFront(0)
    , mySize(0)
    , myCapacity(0)
    {}
    virtual ~squeue()
    { if (myValues)
        delete[] myValues;
    }

    void push(const _Tp &val)
    { if (mySize==myCapacity)
        resize((mySize+1)*2);
      size_t next=(myFront+mySize)%myCapacity;
      myValues[next]=val;
      ++mySize;
    }

    _Tp pop()
    { if (mySize>0)
      { _Tp res=myValues[myFront];
        myFront=(myFront+1)%myCapacity;
        --mySize;
        if (mySize+1<myCapacity/4)
          resize(mySize*2);
        return res;
      }
      else
        throw std::string("No values in squeue::pop");
    }

    void resize(size_t newCapacity)
    { _Tp *newValues=new _Tp[newCapacity];
      for (size_t i=0; i<mySize; ++i)
      { size_t idx=(myFront+i)%myCapacity;
        newValues[i]=myValues[idx];
      }
      if (myValues)
        delete [] myValues;
      myValues=newValues;
      myCapacity=newCapacity;
      myFront=0;
    }

    bool empty()
    { return mySize==0;
    }

    size_t capacity()
    { return myCapacity;
    }

    size_t calcCapacity()
    { if (myValues)
        return sizeof(*myValues)/sizeof(_Tp);
      return 0;
    }

  private:
    _Tp *myValues;
    size_t myFront;
    size_t mySize;
    size_t myCapacity;
};
