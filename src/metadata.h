// -*- C++ -*-

#ifndef METADATA_H_
#define METADATA_H_

#include <string.h>
#include "basic.h"

struct Metadata
{
    uint water, flood, waterproof, growth, razors;
    std::string trampolines;

    Metadata() : water(0), flood(0), waterproof(10), growth(25), razors(0),
                 trampolines() {}
};

#endif // METADATA_H_
