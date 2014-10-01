// Copyright Steinwurf ApS 2011.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#include <cstring>
#include <utility>

#include <kodo/object/storage_decoder.hpp>
#include <kodo/object/storage_encoder.hpp>

#include <kodo/rlnc/full_rlnc_codes.hpp>

/// @example customize_partitioning_scheme.cpp
///
/// Often we want to encode / decode data that exceed a single
/// encoding/decoding block. In this case we need to "chop" up the
/// data into manageable chunks and then encode and decode each chuck
/// separately. The way we "chop" the data is called the block
/// partitioning scheme. In this example we show how to create a
/// custom block partitioning scheme.
///

namespace kodo
{
    //! [0]
    /// In this case we will implement a partitioning scheme which
    /// creates blocks with exactly the same number of symbols in
    /// each.
    ///
    /// In Kodo a block partitioning scheme must implement the
    /// following functions:
    class fixed_partitioning_scheme
    {
    public:

        /// Default constructor
        fixed_partitioning_scheme()
            : m_max_symbols(0),
              m_max_symbol_size(0),
              m_object_size(0),
              m_total_symbols(0),
              m_total_blocks(0)
        { }

        /// Constructor
        /// @param max_symbols the maximum number of symbols in a block
        /// @param max_symbol_size the size in bytes of a symbol
        /// @param object_size the size in bytes of the whole object
        fixed_partitioning_scheme(uint32_t max_symbols,
                                  uint32_t max_symbol_size,
                                  uint32_t object_size)
            : m_max_symbols(max_symbols),
              m_max_symbol_size(max_symbol_size),
              m_object_size(object_size)
        {
            assert(m_max_symbols > 0);
            assert(m_max_symbol_size > 0);
            assert(m_object_size > 0);

            // Lets calculate how many symbols we can make for an
            // object with this size
            //
            // Note the following: ceil(x/y) = ((x - 1) / y) + 1

            // How many symbols can we make?
            m_total_symbols = ((m_object_size - 1) / m_max_symbol_size) + 1;

            // Since we want the same number of symbols in each block
            // we find the greatest common divisor between the total
            // amount of symbols and the max_symbols per block.
            m_symbols_per_block = gcd(m_total_symbols, m_max_symbols);

            assert(m_symbols_per_block > 0);

            m_total_blocks = m_total_symbols / m_symbols_per_block;
            assert(m_total_blocks > 0);
        }

        /// @copydoc block_partitioning::symbols(uint32_t) const
        uint32_t symbols(uint32_t block_id) const
        {
            assert(m_total_blocks > block_id);
            assert(m_symbols_per_block > 0);

            return m_symbols_per_block;
        }

        /// @copydoc block_partitioning::symbol_size(uint32_t) const
        uint32_t symbol_size(uint32_t block_id) const
        {
            (void) block_id;
            assert(m_max_symbol_size > 0);
            return m_max_symbol_size;
        }

        /// @copydoc block_partitioning::block_size(uint32_t) const
        uint32_t block_size(uint32_t block_id) const
        {
            assert(m_total_blocks > block_id);
            return symbols(block_id) * symbol_size(block_id);
        }

        /// @copydoc block_partitioning::bytes_offset(uint32_t) const
        uint32_t byte_offset(uint32_t block_id) const
        {
            assert(m_total_blocks > block_id);

            // We have a constant block size and since block_id starts
            // from zero we can simply multiply the block size with
            // the block_id to get the offset
            return block_size(block_id) * block_id;
        }

        /// @copydoc block_partitioning::bytes_used(uint32_t) const
        uint32_t bytes_used(uint32_t block_id) const
        {
            assert(block_id < m_total_blocks);

            uint32_t offset = byte_offset(block_id);

            assert(offset < m_object_size);
            uint32_t remaining =  m_object_size - offset;
            uint32_t the_block_size = block_size(block_id);

            return std::min(remaining, the_block_size);
        }

        /// @copydoc block_partitioning::blocks() const
        uint32_t blocks() const
        {
            assert(m_total_blocks > 0);
            return m_total_blocks;
        }

        /// @copydoc block_partitioning::object_size() const
        uint32_t object_size() const
        {
            assert(m_object_size > 0);
            return m_object_size;
        }

        /// @copydoc block_partitioning::total_symbols() const
        uint32_t total_symbols() const
        {
            assert(m_total_symbols > 0);
            return m_total_symbols;
        }

        /// @copydoc block_partitioning::total_block_size() const
        uint32_t total_block_size() const
        {
            assert(m_total_symbols > 0);
            assert(m_max_symbol_size > 0);
            return m_total_symbols * m_max_symbol_size;
        }

    private:

        /// Calculate the greatest common divisor of two numbers
        uint32_t gcd(uint32_t u, uint32_t v)
        {
            assert(u > 0);
            assert(v > 0);

            while (v != 0)
            {
                uint32_t r = u % v;
                u = v;
                v = r;
            }

            return u;
        }

    private:

        /// The maximum number of symbols per block
        uint32_t m_max_symbols;

        /// The maximum size of a symbol in bytes
        uint32_t m_max_symbol_size;

        /// The size of the object to transfer in bytes
        uint32_t m_object_size;

        /// The total number of symbols in the object
        uint32_t m_total_symbols;

        /// The number of symbols per block
        uint32_t m_symbols_per_block;

        /// The total number of blocks in the object
        uint32_t m_total_blocks;

    };
    //! [1]
}

int main()
{
    //! [2]
    // Set the number of symbols (i.e. the generation size in RLNC
    // terminology) and the size of a symbol in bytes
    uint32_t max_symbols = 40;
    uint32_t max_symbol_size = 64;

    uint32_t object_size = 6400;
    //! [3]

    //! [4]
    using storage_encoder = kodo::object::storage_encoder<
        kodo::shallow_full_rlnc_encoder<fifi::binary>,
        kodo::fixed_partitioning_scheme>;

    using storage_decoder = kodo::object::storage_decoder<
        kodo::shallow_full_rlnc_decoder<fifi::binary>,
        kodo::fixed_partitioning_scheme>;
    //! [5]

    storage_encoder::factory encoder_factory(max_symbols, max_symbol_size);
    storage_decoder::factory decoder_factory(max_symbols, max_symbol_size);

    std::vector<uint8_t> data_out(object_size, '\0');
    std::vector<uint8_t> data_in(object_size, 'x');

    encoder_factory.set_storage(sak::storage(data_in));
    decoder_factory.set_storage(sak::storage(data_out));

    auto object_encoder = encoder_factory.build();
    auto object_decoder = decoder_factory.build();

    std::cout << "Object size = " << object_size << std::endl;
    std::cout << "Encoder blocks = " << object_encoder->blocks() << std::endl;
    std::cout << "Decoder blocks = " << object_decoder->blocks() << std::endl;

    //! [6]
    for (uint32_t i = 0; i < object_encoder->blocks(); ++i)
    {
        std::cout << "Block = " << i << " symbols = "
                  << object_encoder->symbols(i) << " symbol size = "
                  << object_encoder->symbol_size(i) << std::endl;
    }
    //! [7]

    for (uint32_t i = 0; i < object_encoder->blocks(); ++i)
    {
        auto e = object_encoder->build(i);
        auto d = object_decoder->build(i);

        std::vector<uint8_t> payload(e->payload_size());

        while (!d->is_complete())
        {
            e->encode( payload.data() );

            // Here we would send and receive the payload over a
            // network. Lets throw away some packet to simulate.
            if (rand() % 2)
            {
                continue;
            }

            d->decode( payload.data() );
        }
    }

    // Check we properly decoded the data
    if (data_in == data_out)
    {
        std::cout << "Data decoded correctly" << std::endl;
    }
    else
    {
        std::cout << "Unexpected failure to decode "
                  << "please file a bug report :)" << std::endl;
    }
}
