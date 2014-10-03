.. _encoding_and_decoding_large_objects:

Encoding and decoding large objects
-----------------------------------

In practice we often have to encode/decode data which does not fit
into a single encoder or decoder. To support this use-case Kodo
provides the ``kodo::object::storage_encoder`` and
``kodo::object::storage_decoder`` classes.

It is recommended that you first familiarize yourself with using a
single encoder/decoder pair. You will notice that extending
to several encoders and decoders requires only a few changes to the
code. We will not explain all parameters in detail in this example
only those relevant to using the ``kodo::object::storage_encoder`` and
``kodo::object::storage_decoder`` classes. If you find some
information missing, please check the :ref:`the_basics` example as it is likely
you find it there.

.. contents:: Table of Contents
   :local:

The complete example
~~~~~~~~~~~~~~~~~~~~

.. literalinclude:: ../../../examples/encode_decode_storage/encode_decode_storage.cpp
    :language: c++
    :linenos:

Adding the includes
~~~~~~~~~~~~~~~~~~~

First we have to provide the appropriate includes which defines the
codec that we want to use and the ``kodo::object::storage_encoder``
and ``kodo::object::storage_decoder`` classes.

.. literalinclude:: ../../../examples/encode_decode_storage/encode_decode_storage.cpp
    :language: c++
    :start-after: //! [0]
    :end-before: //! [1]
    :linenos:


Specifying the coding parameters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

As in most other examples we have to specify the number of symbols and
the size of each symbol which we would like to use for the individual
encoders and decoders. One thing to notice here is that these values
are maximum values (i.e. we will never exceed these). However,
depending on the block partitioning scheme used we might not use
exactly those values.

.. note:: When encoding/decoding large objects we have to assign
          different parts of the data to different encoders/decoders,
          the strategy for how this is done is called the block
          partitioning scheme.

For more information about the block partitioning scheme see the
:ref:`customize_partitioning_scheme` example.

In addition we will also specify the size of the object we want to code.

.. literalinclude:: ../../../examples/encode_decode_storage/encode_decode_storage.cpp
    :language: c++
    :start-after: //! [2]
    :end-before: //! [3]
    :linenos:

Specifying the encoder and decoder types
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The ``kodo::object::storage_encoder`` and
``kodo::object::storage_decoder`` classes take one template argument
which is the actual type of the erasure correcting code to use. In
this case we are using the ``kodo::shallow_full_rlnc_encoder`` for
encoding and ``kodo::shallow_full_rlnc_decoder`` for decoding. These
are standard RLNC (Random Linear Network Coding) codes.

.. note:: We use the shallow variant of the RLNC codes. This simply
          means that Kodo will not copy the data into the
          encoder/decoder, but operate directly on the user provided
          buffer (this is currently the only supported mode).

.. literalinclude:: ../../../examples/encode_decode_storage/encode_decode_storage.cpp
    :language: c++
    :start-after: //! [4]
    :end-before: //! [5]
    :linenos:


Using the object encoder and decoder
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

As with :ref:`the_basics` example we can now create the input and
output data buffers and use it to initialize the object encoder/decoder.

.. literalinclude:: ../../../examples/encode_decode_storage/encode_decode_storage.cpp
    :language: c++
    :start-after: //! [6]
    :end-before: //! [7]
    :linenos:

The encoding/decoding loop has changed a bit since we now have several
encoders and decoders that need to finish before the entire object has
been encoded and decoded. However, the general structure is very
similar to using just a single encoder and decoder.

.. literalinclude:: ../../../examples/encode_decode_storage/encode_decode_storage.cpp
    :language: c++
    :start-after: //! [8]
    :end-before: //! [9]
    :linenos:
