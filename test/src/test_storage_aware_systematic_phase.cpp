// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_storage_aware_systematic_phase.cpp Unit tests for the
///       kodo::storage_aware_systematic_phase layer

#include <cstdint>
#include <gtest/gtest.h>

#include <kodo/storage_aware_systematic_phase.hpp>

namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        // Small helper struct which provides the API needed by the
        // storage_aware_systematic_phase layer.
        struct dummy_final
        {
            typedef uint32_t rank_type;

            struct factory
            {
                /// @copydoc layer::factory::factory(uint32_t,uint32_t)
                factory(uint32_t max_symbols, uint32_t max_symbol_size)
                    : m_max_payload_size(0)
                {
                    (void) max_symbols;
                    (void) max_symbol_size;
                }

                uint32_t max_payload_size() const
                {
                    return m_max_payload_size;
                }

                uint32_t m_max_payload_size;
            };


            template<class Factory>
            void initialize(Factory& the_factory)
            {
                (void) the_factory;
            }


            void decode(uint8_t *payload)
            {
                m_payload = payload;
            }

            uint32_t payload_size() const
            {
                return m_payload_size;
            }

            uint8_t *m_payload;
            uint32_t m_payload_size;
        };

        // Instantiate a stack containing the storage_aware_systematic_phase
        class rank_decoder_stack
            : public // Payload API
              storage_aware_systematic_phase<
              // Codec Header API
              // Symbol ID API
              // Codec API
              // Coefficient Storage API
              // Storage API
              // Finite Field API
              // Factory API
              // Final type
              dummy_final>
          { };
    }
}

/// Run the tests typical coefficients stack
TEST(TestPayloadRankDecoder, test_storage_aware_systematic_phase)
{

    kodo::rank_decoder_stack::factory factory(0U, 0U);


}


