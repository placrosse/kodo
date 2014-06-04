// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <gtest/gtest.h>

template<class RankWriter>
inline void test_rank_writer()
{

    RankWriter stack;

    typedef typename RankWriter::rank_type rank_type;

    std::vector<uint8_t> buffer(sizeof(rank_type));

    stack.m_rank = 5;
    stack.write_rank(&buffer[0]);
    EXPECT_EQ(sak::big_endian::get<rank_type>(&buffer[0]), 5U);

    stack.m_rank = 2345;
    stack.write_rank(&buffer[0]);
    EXPECT_EQ(sak::big_endian::get<rank_type>(&buffer[0]), 2345U);

    stack.m_rank = 11;
    stack.write_rank(&buffer[0]);
    EXPECT_EQ(sak::big_endian::get<rank_type>(&buffer[0]), 11U);

}





