// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_sparse_uniform_generator.hpp Unit tests for the sparse uniform
///       coefficient generators

#include <fifi/is_binary.hpp>
#include <kodo/basic_factory.hpp>
#include <kodo/pool_factory.hpp>

#include "kodo_unit_test/coefficient_generator_helper.hpp"

namespace kodo
{
    // Put dummy layers and tests classes in an anonymous namespace
    // to avoid violations of ODF (one-definition-rule) in other
    // translation units
    namespace
    {
        // Sparse uniform generator
        template<class Field>
        class sparse_uniform_generator_stack : public
            sparse_uniform_generator<
            fake_codec_layer<
            coefficient_info<
            fake_symbol_storage<
            storage_block_length<
            storage_block_size<
            finite_field_info<Field,
            final_layer
            > > > > > > >
        {
        public:
            using factory = basic_factory<sparse_uniform_generator_stack>;
        };

        template<class Field>
        class sparse_uniform_generator_stack_pool : public
            sparse_uniform_generator<
            fake_codec_layer<
            coefficient_info<
            fake_symbol_storage<
            storage_block_length<
            storage_block_size<
            finite_field_info<Field,
            final_layer
            > > > > > > >
        {
        public:
            using factory = pool_factory<sparse_uniform_generator_stack_pool>;
        };
    }
}

// Put dummy layers and tests classes in an anonymous namespace
// to avoid violations of ODF (one-definition-rule) in other
// translation units
namespace
{

    /// Tests:
    ///   - layer::set_density(double)
    ///   - layer::set_average_nonzero_symbols(double)
    ///   - layer::density()
    template<class Coder>
    struct api_density
    {

        typedef typename Coder::factory factory_type;
        typedef typename Coder::field_type field_type;
        typedef typename Coder::value_type value_type;

        api_density(uint32_t max_symbols,
                    uint32_t max_symbol_size)
            : m_factory(max_symbols, max_symbol_size)
        { }

        void run()
        {
            // We invoke the test multiple times to ensure that if the
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

            // Test lower bounds
            run_once(1, symbol_size);
            run_once(symbols, 4);
            run_once(1, 4);
        }

        void run_once(uint32_t symbols, uint32_t symbol_size)
        {
            m_factory.set_symbols(symbols);
            m_factory.set_symbol_size(symbol_size);

            auto coder = m_factory.build();

            std::vector<uint8_t> vector_a =
                random_vector(coder->coefficient_vector_size());

            std::vector<uint8_t> vector_b =
                random_vector(coder->coefficient_vector_size());

            std::vector<uint8_t> vector_c =
                random_vector(coder->coefficient_vector_size());

            std::vector<uint8_t> vector_d =
                random_vector(coder->coefficient_vector_size());

            if (fifi::is_binary<field_type>::value)
            {
                double average_nonzero_symbols = symbols/2.0;
                coder->set_average_nonzero_symbols(average_nonzero_symbols);
                EXPECT_EQ(average_nonzero_symbols/symbols, coder->density());
            }
            else
            {
                double average_nonzero_symbols = std::ceil(symbols/2.0);
                coder->set_average_nonzero_symbols(average_nonzero_symbols);
                EXPECT_EQ(average_nonzero_symbols/symbols, coder->density());

                coder->set_average_nonzero_symbols(symbols);
                EXPECT_EQ(1.0, coder->density());

                coder->set_density(1.0);
                EXPECT_EQ(1.0, coder->density());
            }

            coder->set_density(0.01);
            EXPECT_EQ(0.01, coder->density());

            coder->seed(0);
            coder->generate(&vector_a[0]);

            coder->seed(0);
            coder->generate(&vector_b[0]);

            auto storage_a = sak::storage(vector_a);
            auto storage_b = sak::storage(vector_b);

            EXPECT_TRUE(sak::is_equal(storage_a,storage_b));

            coder->seed(0);

            coder->generate(&vector_c[0]);
            coder->generate(&vector_d[0]);
        }

    private:

        // The factory
        factory_type m_factory;

    };

}

/// Run the tests typical coefficients stack
TEST(TestCoefficientGenerator, sparse_uniform_generator_stack)
{
    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    // API tests:
    run_test<
        kodo::sparse_uniform_generator_stack,
        api_generate>(symbols, symbol_size);

    run_test<
        kodo::sparse_uniform_generator_stack_pool,
        api_generate>(symbols, symbol_size);
}


/// Run the tests typical coefficients stack
TEST(TestCoefficientGenerator, density_sparse_uniform_generator_stack)
{
    uint32_t symbols = rand_symbols();
    uint32_t symbol_size = rand_symbol_size();

    // API tests:
    run_test<
        kodo::sparse_uniform_generator_stack,
        api_density>(symbols, symbol_size);

    run_test<
        kodo::sparse_uniform_generator_stack_pool,
        api_density>(symbols, symbol_size);
}
