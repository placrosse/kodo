// Copyright Steinwurf ApS 2011-2012.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

#include <cstdint>

#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/make_shared.hpp>
#include <boost/optional.hpp>

#include <sak/storage.hpp>

#include <fifi/is_binary.hpp>
#include <fifi/fifi_utils.hpp>

#include <kodo/has_linear_block_decoder_delayed.hpp>

namespace kodo
{

    /// @ingroup payload_codec
    /// @brief This layer can be added to a decoding stack to track
    ///        when partial decoding is possible i.e. when symbols in
    ///        decoder are fully decoded even though the full data
    ///        block has not been sent.
    ///
    /// Detecting whether data has been partially decoded can be done by
    /// tracking whether encoder and decoder has the same rank. This of course
    /// means that we depend on the decoder performs full Gaussian elimination
    /// on the incoming symbols.
    ///
    /// To figure out which symbols have been partially decoded the decoder's
    /// layer::is_symbol_pivot(uint32_t) const function can be used.
    template<class SuperCoder>
    class partial_decoding_tracker : public SuperCoder
    {
    public:

        /// @copydoc layer::rank_type
        typedef typename SuperCoder::rank_type rank_type;

    public:

        static_assert(!has_linear_block_decoder_delayed<SuperCoder>::value,
                      "In order for the ... split this into two layers"
                      "one which just check the symbols_decoded() and"
                      "one which sets the symbol_decoded()");

    public:

        /// Constructor
        partial_decoding_tracker()
            : m_partial_complete(false)
        { }

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void inititalize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);

            m_partial_complete = false;
        }

        /// @copydoc layer::decode(uint8_t*)
        void decode(uint8_t *payload)
        {
            assert(payload != 0);

            uint32_t symbols_decoded = SuperCoder::symbols_decoded();

            SuperCoder::decode(payload);

            rank_type decoder_rank = SuperCoder::rank();
            rank_type seen_encoder_rank = SuperCoder::seen_encoder_rank();

            // If the decoder and encoder rank matches we know that when
            // using Gaussian Elimination with backward substitution that
            //
            if(decoder_rank == seen_encoder_rank)
            {
                for(uint32_t i = 0; i < SuperCoder::symbols(); ++i)
                {
                    if(SuperCoder::is_symbol_seen(i))
                    {
                        SuperCoder::set_symbol_decoded(i);
                    }
                }
            }

            // We cannot have less decoded symbols after calling decode
            assert(SuperCoder::symbols_decoded() >= symbols_decoded);

            m_partial_complete =
                SuperCoder::symbols_decoded() > symbols_decoded;
        }


        /// @return True if the decoding matrix should be partially decoded.
        bool is_partial_complete() const
        {
            return m_partial_complete;
        }

    private:

        /// Boolean tracking whether the decoder had any new decoded
        /// symbols since last time the layer::decode(uint8_t*)
        /// function was invoked
        bool m_partial_complete;

    };

}

