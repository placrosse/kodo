// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <sstream>
#include <gtest/gtest.h>

#include <kodo/hexdump.hpp>

TEST(TestHexdump, api)
{
    {
        std::string test = "hellohellohelloo";

        kodo::hexdump hex(sak::storage(test));

        std::stringstream ss;

        ss << hex;

        EXPECT_EQ("0000  68 65 6c 6c 6f 68 65 6c 6c 6f 68 65 6c 6c 6f 6f  "
                  "hellohellohelloo\n", ss.str());
    }

    {
        std::vector<uint8_t> data =
            { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
              0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };

        kodo::hexdump hex(sak::storage(data));
        std::stringstream ss;

        ss << hex;

        EXPECT_EQ("0000  01 02 03 04 05 06 07 08 01 02 03 04 05 06 07 08  "
                  "................\n", ss.str());
    }

    {
        std::vector<uint8_t> data =
            { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
              0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
              0x05, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
              0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08 };

        kodo::hexdump hex(sak::storage(data));
        std::stringstream ss;

        ss << hex;

        EXPECT_EQ("0000  01 02 03 04 05 06 07 08 01 02 03 04 05 06 07 08  "
                  "................\n"
                  "0010  05 02 03 04 05 06 07 08 01 02 03 04 05 06 07 08  "
                  "................\n",
                  ss.str());
    }

    {
        // 48 bytes
        std::vector<uint8_t> data =
            { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
              0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
              0x05, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
              0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
              0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
              0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

        kodo::hexdump hex(sak::storage(data));
        hex.set_max_size(5);

        std::stringstream ss;
        ss << hex;

        EXPECT_EQ("0000  01 02 03 04 05                                   "
                  ".....\n"
                  "....\n"
                  "0030\n",
                  ss.str());
    }

    {
        // 48 bytes
        std::vector<uint8_t> data = { 0x01, 0x02, 0x03, 0x04, 0x05 };

        kodo::hexdump hex(sak::storage(data));
        hex.set_max_size(3);

        std::stringstream ss;
        ss << hex;

        EXPECT_EQ("0000  01 02 03                                         "
                  "...\n",
                  ss.str());
    }
}
