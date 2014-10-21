// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

enum class test_profile
{
    desktop,
    embedded
};

test_profile current_test_profile();

void parse_test_arguments(int argc, char** argv);
