// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

/// @file test_coefficient_generator.hpp Unit tests for the coefficient
///       generators

#include <cstdint>

#include <gtest/gtest.h>

#include <fifi/fifi_utils.hpp>

#include <kodo/final_layer.hpp>
#include <kodo/final_layer.hpp>
#include <kodo/finite_field_info.hpp>
#include <kodo/coefficient_info.hpp>
#include <kodo/storage_block_info.hpp>
#include <kodo/uniform_generator.hpp>
#include <kodo/sparse_uniform_generator.hpp>
#include <kodo/fake_symbol_storage.hpp>

#include "basic_api_test_helper.hpp"

/// Defines a number test stacks which contains the layers we wish to
/// test.
/// The stacks we define below contain the expect layers used in a
/// typical coefficient storage stack. In addition to this we test both
/// with and without adding the factory pool layer. This layer will
/// recycle the allocated objects.
namespace kodo
{

    /// Fake codec layer is a helper call for unit-testing code
    /// for the coefficient generator layers. The fake layer will
    /// generate a certain number pivot
    template<class SuperCoder>
    class fake_codec_layer : public SuperCoder
    {
    public:

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_pivots = 0;
            m_pivot.resize(the_factory.symbols());

            for(uint32_t i = 0; i < m_pivot.size(); ++i)
            {
                m_pivot[i] = (rand() % 2) != 0;
                if(m_pivot[i])
                    ++m_pivots;
            }
        }

        /// @copydoc layer::rank() const
        uint32_t rank() const
        {
            return m_pivots;
        }

        /// @copydoc layer::is_symbol_pivot(uint32_t) const
        bool is_symbol_pivot(uint32_t index) const
        {
            assert(index < SuperCoder::symbols());
            return m_pivot[index];
        }

    private:

        /// Track dummy pivot
        std::vector<bool> m_pivot;

        /// The number of pivots
        uint32_t m_pivots;

    };

}

/// Tests:
///   - layer::generate(uint8_t*)
///   - layer::generate_partial(uint8_t*)
///   - layer::seed(seed_type)
template<class Coder>
struct api_generate
{

    typedef typename Coder::factory factory_type;
    typedef typename Coder::field_type field_type;
    typedef typename Coder::value_type value_type;

    api_generate(uint32_t max_symbols,
                 uint32_t max_symbol_size)
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

        run_partial(symbols, symbol_size);
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

        coder->seed(0);
        coder->generate(&vector_a[0]);

        coder->seed(0);
        coder->generate(&vector_b[0]);

        auto storage_a = sak::storage(vector_a);
        auto storage_b = sak::storage(vector_b);


        EXPECT_TRUE(sak::is_equal(storage_a,storage_b));
    }


    void run_partial(uint32_t symbols, uint32_t symbol_size)
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

        coder->seed(0);
        coder->generate_partial(&vector_a[0]);
        coder->generate_partial(&vector_b[0]);

        coder->seed(0);
        coder->generate_partial(&vector_c[0]);

        coder->seed(1);
        coder->generate_partial(&vector_d[0]);

        auto storage_a = sak::storage(vector_a);
        auto storage_c = sak::storage(vector_c);

        EXPECT_TRUE(sak::is_equal(storage_a,storage_c));

        uint32_t count_a = 0;
        uint32_t count_b = 0;
        uint32_t count_c = 0;
        uint32_t count_d = 0;

        for(uint32_t i = 0; i < symbols; ++i)
        {
            value_type v_a = fifi::get_value<field_type>(
                (value_type*)&vector_a[0], i);

            value_type v_b = fifi::get_value<field_type>(
                (value_type*)&vector_b[0], i);

            value_type v_c = fifi::get_value<field_type>(
                (value_type*)&vector_c[0], i);

            value_type v_d = fifi::get_value<field_type>(
                (value_type*)&vector_d[0], i);

            if(!coder->is_symbol_pivot(i))
            {
                ASSERT_EQ(v_a, 0U);
                ASSERT_EQ(v_b, 0U);
                ASSERT_EQ(v_c, 0U);
                ASSERT_EQ(v_d, 0U);
            }

            count_a += (v_a != 0);
            count_b += (v_b != 0);
            count_c += (v_c != 0);
            count_d += (v_d != 0);

        }

        EXPECT_TRUE(count_a <= coder->rank());
        EXPECT_TRUE(count_b <= coder->rank());
        EXPECT_TRUE(count_c <= coder->rank());
        EXPECT_TRUE(count_d <= coder->rank());
    }

private:

    // The factory
    factory_type m_factory;

};
