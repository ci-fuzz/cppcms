#include "/home/user/workspace/fuzzing/cppcms/cppcms/json.h"
#include <stdint.h>

class cppcms::json::value &cppcms::json::value::operator[](size_t n);

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
  // Add Setup code here.
  cppcms::json::value value;
  value.operator[](/* add arguments here */);
  // Add Teardown code here.

  return 0;
}
