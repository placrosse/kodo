// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include "systematic_encoder.hpp"

namespace kodo
{

    template<class SuperCoder>
    class default_on_systematic_encoder :
        public base_systematic_encoder<true, SuperCoder>
    { };

}


