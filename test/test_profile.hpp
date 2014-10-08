// Copyright (c) 2011-2014 Steinwurf ApS
// All Rights Reserved
//
// THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF STEINWURF
// The copyright notice above does not evidence any
// actual or intended publication of such source code.

#pragma once

enum class test_profile
{
    desktop,
    embedded
};

test_profile current_test_profile();

void parse_test_arguments(int argc, char** argv);
