/** This file denines value structure (class) used by the the cpi
 ** (pi-calculus operations for C++) operations.
 */
namespace libpi
{
  class Value
  { public:
      Value(int size, const char *date);
      virtual ~Value();
  
      int GetSize();
      const char *GetDate();
  
    private:
      int mySize;
      char *myDate;
  };
}
