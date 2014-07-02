// Copyright Steinwurf ApS 2011-2013.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_symbol_id.cpp Unit tests for the Symbol ID API

#include <cstdint>

#include <gtest/gtest.h>

#include <fifi/default_field.hpp>

#include <kodo/final_layer.hpp>
#include <kodo/final_layer.hpp>
#include <kodo/plain_symbol_id_reader.hpp>
#include <kodo/plain_symbol_id_writer.hpp>
#include <kodo/uniform_generator.hpp>
#include <kodo/coefficient_storage.hpp>
#include <kodo/coefficient_info.hpp>
#include <kodo/storage_block_length.hpp>
#include <kodo/storage_block_size.hpp>
#include <kodo/encode_symbol_tracker.hpp>
#include <kodo/finite_field_math.hpp>
#include <kodo/finite_field_info.hpp>
#include <kodo/reed_solomon/reed_solomon_symbol_id_writer.hpp>
#include <kodo/reed_solomon/reed_solomon_symbol_id_reader.hpp>
#include <kodo/reed_solomon/vandermonde_matrix.hpp>
#include <kodo/basic_factory.hpp>

#include "kodo_unit_test/basic_api_test_helper.hpp"

/// Here we define the stacks which should be tested.
namespace kodo
{

    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {

        template<class SuperCoder>
        struct dummy_layer : public SuperCoder
        {
            uint32_t rank() const
            {
                return SuperCoder::symbols();
            }
        };


        template<class Field>
        class plain_uniform_stack : public
            plain_symbol_id_reader<
            plain_symbol_id_writer<
            uniform_generator<
            coefficient_info<
            dummy_layer<
            storage_block_length<
            storage_block_size<
            finite_field_info<Field,
            final_layer
            > > > > > > > >
        {
        public:
            using factory = basic_factory<plain_uniform_stack>;
        };

        template<class Field>
        class rs_vandermond_nonsystematic_stack : public
            reed_solomon_symbol_id_reader<
            reed_solomon_symbol_id_writer<
            vandermonde_matrix<
            coefficient_info<
            dummy_layer<
            storage_block_length<
            storage_block_size<
            encode_symbol_tracker<
            finite_field_math<typename fifi::default_field<Field>::type,
            finite_field_info<Field,
            final_layer
            > > > > > > > > > >
        {
        public:
            using factory = basic_factory<rs_vandermond_nonsystematic_stack>;
        };
    }
}

/// Tests:
///   - layer::factory_base::max_id_size() const
///   - layer::read_id(uint8_t*,uint8_t**)
///   - layer::write_id(uint8_t*,uint8_t**)
///   - layer::id_size()
template<class Coder>
struct api_symbol_id
{

    typedef typename Coder::factory factory_type;
    typedef typename Coder::field_type field_type;
    typedef typename Coder::value_type value_type;

    api_symbol_id(uint32_t max_symbols, uint32_t max_symbol_size)
        : m_factory(max_symbols, max_symbol_size)
        { }

    void run()
        {
            // We invoke the test three times to ensure that if the
            // factory recycles the objects they are safe to reuse
            run_once(m_factory.max_symbols(),
                     m_factory.max_symbol_size());

            run_once(m_factory.max_symbols(),
                     m_factory.max_symbol_size());

            // Build with different from max values
            uint32_t symbols =
                rand_symbols(m_factory.max_symbols());

            uint32_t symbol_size =
                rand_symbol_size(m_factory.max_symbol_size());

            run_once(symbols, symbol_size);
        }

    void run_once(uint32_t symbols, uint32_t symbol_size)
        {
            m_factory.set_symbols(symbols);
            m_factory.set_symbol_size(symbol_size);

            auto coder = m_factory.build();

            EXPECT_TRUE(m_factory.max_id_size() > 0);
            EXPECT_TRUE(coder->id_size() > 0);

            EXPECT_TRUE(m_factory.max_id_size() >= coder->id_size());

            std::vector<uint8_t> id(coder->id_size());

            uint8_t *coefficients_out = 0;
            uint8_t *coefficients_in = 0;

            // Write the symbol id and get the corresponding coefficients
            uint32_t bytes_used =
                coder->write_id(&id[0], &coefficients_out);

            EXPECT_TRUE(bytes_used > 0);
            EXPECT_TRUE(coefficients_out != 0);

            // Read the symbol id and get the corresponding coefficients
            coder->read_id(&id[0], &coefficients_in);

            EXPECT_TRUE(coefficients_in != 0);

            auto storage_out =
                sak::storage(coefficients_out, coder->coefficient_vector_size());

            auto storage_in =
                sak::storage(coefficients_in, coder->coefficient_vector_size());

            EXPECT_TRUE(sak::equal(storage_out, storage_in));
        }

private:

    // The factory
    factory_type m_factory;

};


/// Run the tests typical coefficients stack
TEST(TestSymbolId, test_plain_stack)
{
    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    // API tests:
    run_test<kodo::plain_uniform_stack, api_symbol_id>(
        symbols, symbol_size);

}


/// Run the tests typical coefficients stack
TEST(TestSymbolId, test_rs_stack)
{
    uint32_t symbols = rand_symbols(255);
    uint32_t symbol_size = rand_symbol_size();

    // API tests:
    api_symbol_id
    <
        kodo::rs_vandermond_nonsystematic_stack<fifi::binary8>
    > test(symbols, symbol_size);

    test.run();

}
