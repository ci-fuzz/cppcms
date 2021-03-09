// Copyright (c) 2019 Code Intelligence. All rights reserved.

#include <sstream>
#include <iostream>
#include <stdint.h>

#include <cppcms/json.h>
#include <mutator.h>

DEFINE_GRAMMAR_FUZZER(const uint8_t *data, size_t size) {
    cppcms::json::value value;
    const char* c_data = (char*) data;
    if (value.load(c_data, c_data + size, true)){
        value.save(cppcms::json::readable);
    }
}
