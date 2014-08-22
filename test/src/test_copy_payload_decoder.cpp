// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_copy_payload_decoder Unit test for copy_payload_decoder layer

#include <gtest/gtest.h>
#include <stub/call.hpp>

#include <kodo/copy_payload_decoder.hpp>
#include <kodo/basic_factory.hpp>

/// Here we define the stacks which should be tested.
namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        // Layer to make the m_payload_copy buffer available
        template<class SuperCoder>
        class access_payload_copy : public SuperCoder
        {
        public:

            std::vector<uint8_t>& payload_copy()
            {
                return SuperCoder::m_payload_copy;
            }

            const std::vector<uint8_t>& payload_copy() const
            {
                return SuperCoder::m_payload_copy;
            }
        };

        // Dummy layer satisfying the dependencies of copy_payload_decoder
        class dummy_layer
        {
        public:

            class factory_base
            {
            public:

                factory_base(uint32_t,uint32_t)
                { }

                uint32_t max_payload_size() const
                {
                    return m_max_payload_size();
                }

                stub::call<uint32_t()> m_max_payload_size;
            };

        public:

            template<class Factory>
            void construct(Factory& the_factory)
            {
                (void) the_factory;
            }

            template<class Factory>
            void initialize(Factory& the_factory)
            {
                (void) the_factory;
            }

            uint32_t payload_size() const
            {
                return m_payload_size();
            }

            void decode(uint8_t* payload)
            {
                m_decode(payload);

                // Clear payload
                std::fill_n(payload, payload_size(), 0);
            }

            stub::call<uint32_t()> m_payload_size;
            stub::call<void(uint8_t*)> m_decode;
        };

        class dummy_stack : public
            access_payload_copy<copy_payload_decoder<dummy_layer>>
        {
        public:
            using factory = basic_factory<dummy_stack>;
        };
    }
}

/// Run the tests
TEST(TestCopyPayloadDecoder, api)
{
    // Create and initialize coder
    kodo::dummy_stack::factory f(0,0);
    f.m_max_payload_size.set_return(10);

    auto c = f.build();
    EXPECT_TRUE((bool)f.m_max_payload_size.expect_calls().with());

    c->m_payload_size.set_return(5);

    std::vector<uint8_t> data(10, 'a');
    std::vector<uint8_t> data_copy = data;

    c->decode(data.data());

    // The data did not change
    EXPECT_TRUE(data == data_copy);

    // The internal buffer was used
    EXPECT_TRUE((bool)c->m_decode.expect_calls()
                    .with(c->payload_copy().data()));
}
