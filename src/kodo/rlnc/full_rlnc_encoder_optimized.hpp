#include <cstdint>

#include <kodo/final_layer.hpp>
#include <kodo/zero_symbol_encoder.hpp>
#include <kodo/default_on_systematic_encoder.hpp>
#include <kodo/payload_encoder.hpp>
#include <kodo/symbol_id_encoder.hpp>
#include <kodo/coefficient_storage.hpp>
#include <kodo/coefficient_info.hpp>
#include <kodo/plain_symbol_id_writer.hpp>
#include <kodo/new_uniform_generator.hpp>
#include <kodo/storage_aware_encoder.hpp>
#include <kodo/encode_symbol_tracker.hpp>
#include <kodo/linear_block_encoder.hpp>
#include <kodo/coefficient_value_access.hpp>
#include <kodo/deep_storage_layers.hpp>
#include <kodo/finite_field_layers.hpp>

#include <boost/random/mersenne_twister.hpp>           //mt11213b and mt19937
#include <boost/random/shuffle_order.hpp>              //knuth_b and kreutzer1986
#include <boost/random/taus88.hpp>                     //taus88

namespace kodo
{
    /// @ingroup fec_stacks
    /// @brief Complete stack implementing a RLNC encoder.
    ///
    /// The key features of this configuration is the following:
    /// - Systematic encoding (uncoded symbols produced before switching
    ///   to coding)
    /// - Full encoding vectors, this stack uses the plain_symbol_id_writer
    ///   which sends the full encoding vector with every encoded symbol.
    ///   Encoding vectors are generated using a random uniform generator.
    /// - Deep symbol storage which makes the encoder allocate its own
    ///   internal memory.

    //////////////////////////////////////////////////////////////////
    // taus88 implemention with 4 stacks.
    //////////////////////////////////////////////////////////////////
    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_taus88_uint8 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::taus88,
        uint8_t,
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_taus88_uint8>;
    };

    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_taus88_uint16 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::taus88,
        uint16_t,
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_taus88_uint16>;
    };

    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_taus88_uint32 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::taus88,
        uint32_t,
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_taus88_uint32>;
    };

    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_taus88_uint64 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::taus88,
        uint64_t,
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_taus88_uint64>;
    };

    //////////////////////////////////////////////////////////////////
    // mt19937 implementaion in 4 stack.
    //////////////////////////////////////////////////////////////////
    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_mt19937_uint8 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::mt19937,
        uint8_t,
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_mt19937_uint8>;
    };

    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_mt19937_uint16 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::mt19937,
        uint16_t,
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_mt19937_uint16>;
    };


    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_mt19937_uint32 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::mt19937,
        uint32_t,
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_mt19937_uint32>;
    };

    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_mt19937_uint64 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::mt19937,
        uint64_t,
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_mt19937_uint64>;
    };

    //////////////////////////////////////////////////////////////////
    // mt11213b implementaion in 4 stack.
    //////////////////////////////////////////////////////////////////
    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_mt11213b_uint8 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::mt11213b,
        uint8_t,
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_mt11213b_uint8>;
    };

    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_mt11213b_uint16 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::mt11213b,
        uint16_t,
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_mt11213b_uint16>;
    };

    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_mt11213b_uint32 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::mt11213b,
        uint32_t,
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_mt11213b_uint32>;
    };

    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_mt11213b_uint64 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::mt11213b,
        uint64_t,
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_mt11213b_uint64>;
    };

    //////////////////////////////////////////////////////////////////
    // knuth_b implementation in four stacks
    //////////////////////////////////////////////////////////////////
    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_knuth_b_uint8 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::knuth_b,
        uint8_t,
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_knuth_b_uint8>;
    };

    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_knuth_b_uint16 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::knuth_b,
        uint16_t,
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_knuth_b_uint16>;
    };

    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_knuth_b_uint32 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::knuth_b,
        uint32_t,
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_knuth_b_uint32>;
    };

    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_knuth_b_uint64 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::knuth_b,
        uint64_t,
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_knuth_b_uint64>;
    };

    //////////////////////////////////////////////////////////////////
    // kreutzer1986 implementation in four stacks.
    //////////////////////////////////////////////////////////////////
    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_kreutzer1986_uint8 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::kreutzer1986,
        uint8_t,
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_kreutzer1986_uint8>;
    };

    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_kreutzer1986_uint16 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::kreutzer1986,
        uint16_t,
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_kreutzer1986_uint16>;
    };

    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_kreutzer1986_uint32 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::kreutzer1986,
        uint32_t,
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_kreutzer1986_uint32>;
    };

    template<class Field, class TraceTag = kodo::disable_trace>
    class full_rlnc_encoder_kreutzer1986_uint64 : public
        // Payload Codec API
        payload_encoder<
        // Codec Header API
        default_on_systematic_encoder<
        symbol_id_encoder<
        // Symbol ID API
        plain_symbol_id_writer<
        // Coefficient Generator API
        new_uniform_generator<
        boost::random::kreutzer1986,
        uint64_t,
        // Codec API
        encode_symbol_tracker<
        zero_symbol_encoder<
        linear_block_encoder<
        storage_aware_encoder<
        // Coefficient Storage API
        coefficient_value_access<
        coefficient_info<
        // Symbol Storage API
        deep_storage_layers<TraceTag,
        // Finite Field API
        finite_field_layers<Field,
        // Final Layer
        final_layer
        > > > > > > > > > > > > >
    {
    public:
        using factory = pool_factory<full_rlnc_encoder_kreutzer1986_uint64>;
    };
}
