// Copyright (c) 2019 Code Intelligence. All rights reserved.

#include <sstream>
#include <stdint.h>

#include <cppcms/json.h>

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
    cppcms::json::value value;
    const char* c_data = (char*) data;
    if (value.load(c_data, c_data + size, true)){
        value.save(cppcms::json::readable);
    }
    return 0;
}