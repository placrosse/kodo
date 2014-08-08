// Copyright Steinwurf ApS 2011-2014.
// Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
// See accompanying file LICENSE.rst or
// http://www.steinwurf.com/licensing

#pragma once

namespace kodo
{
    /// @ingroup payload_codec_layers
    ///
    /// @brief This layer can be added to a decoder stack to make sure
    ///        that a "partial" symbol will be restored once decoding
    ///        is complete.
    ///
    /// When we use shallow buffers with a decoder we want to directly
    /// decode into the user provided buffer. However this might now
    /// always be so easy, for example if the user provided buffer is
    /// not a multiple of the symbol size and the number of symbols.
    ///
    /// To deal with this we will typically use the
    /// partial_mutable_shallow_symbol_storage layer in shallow
    /// decoders. This layer will, if needed, provide an internal
    /// buffer for the last partial symbol. However, once decoding is
    /// complete we want to move the decoded data stored in the
    /// internal buffer to the user provided buffer. To do this the
    /// partial_mutaable_shallow_symbol_storage layer provides a
    /// restore function. This layer makes sure that the restore
    /// function is called once decoding is complete. We do this by
    /// "intercepting" calls to the decode functions and check if
    /// decoding is complete. If it is we invoke restore.
    ///
    /// There are currently three functions which allows a user to add
    /// data to the decoder we "intercept" them all here.
    template<class SuperCoder>
    class restore_partial_symbol_decoder : public SuperCoder
    {
    public:

        /// @copydoc layer::initialize(Factory&)
        template<class Factory>
        void initialize(Factory& the_factory)
        {
            SuperCoder::initialize(the_factory);
            m_restored = false;
        }

        /// @copydoc layer::decode(uint8_t*)
        void decode(uint8_t *payload)
        {
            SuperCoder::decode(payload);
            restore_on_decode();
        }

        /// @copydoc layer::decode_symbol(uint8_t*,uint8_t*)
        void decode_symbol(uint8_t *symbol_data,
                           uint8_t *symbol_coefficients)
        {
            SuperCoder::decode_symbol(symbol_data, symbol_coefficients);
            restore_on_decode();
        }

        /// @copydoc layer::decode_symbol(uint8_t*, uint32_t)
        void decode_symbol(uint8_t *symbol_data, uint32_t symbol_index)
        {
            SuperCoder::decode_symbol(symbol_data, symbol_index);
            restore_on_decode();
        }

    private:

        /// This function will restore the partial symbol if needed
        void restore_on_decode()
        {
            if (!SuperCoder::is_complete())
                return;

            if (!SuperCoder::has_partial_symbol())
                return;

            if (!m_restored)
            {
                m_restored = true;
                SuperCoder::restore_partial_symbol();
            }
        }

    private:

        /// Keeps track of whether we already called the restore function
        /// so that we only do it once
        bool m_restored;
    };
}
