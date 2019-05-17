#define HASASSERT
#include "opentx.h"

#if 0
struct TestStr {
  CUSTOM_SCREENS_DATA
};

#define DISPLAY_VALUE2(x) #x
#define DISPLAY_VALUE(x) DISPLAY_VALUE2(x)
#pragma message( "My Value  = " DISPLAY_VALUE(CUSTOM_SCREENS_DATA) )

void foo(){
  char checker(int);
  char checkSizeOf[sizeof(ModuleData)*8]={checker(&checkSizeOf)};
}
#endif