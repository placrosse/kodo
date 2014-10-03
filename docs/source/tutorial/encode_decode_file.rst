.. _encoding_and_decoding_files:

Encoding and decoding files
---------------------------

In this example we show how to encode/decode files with Kodo. In Kodo
this is example is nearly identical to the example
:ref:`encoding_and_decoding_large_objects`. For this reason we will mainly
highlight the differences.

.. contents:: Table of Contents
   :local:

The complete example
~~~~~~~~~~~~~~~~~~~~

.. literalinclude:: ../../../examples/encode_decode_file/encode_decode_file.cpp
    :language: c++
    :linenos:

Adding the includes
~~~~~~~~~~~~~~~~~~~

First we have to provide the appropriate includes which defines the
codec that we want to use and the ``kodo::object::file_encoder``
and ``kodo::object::file_decoder`` classes.

.. literalinclude:: ../../../examples/encode_decode_file/encode_decode_file.cpp
    :language: c++
    :start-after: //! [0]
    :end-before: //! [1]
    :linenos:


Specifying the coding parameters
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

For the file encoder/decoder case three options are new. The first is
the file name of the file we want to encode, the seconds is the file
name of the file we want to decode data into and finally the size of
file.

.. note:: In a real application we would most likely not use different
          file names for the encoder and decoder.

.. note:: The file size is only needed by the file decoder. The file
          encoder knows the file size after opening the file.

As with the :ref:`encoding_and_decoding_large_objects` we pass type of
the actual encoding and decoding algorithm as a template argument.

.. literalinclude:: ../../../examples/encode_decode_file/encode_decode_file.cpp
    :language: c++
    :start-after: //! [2]
    :end-before: //! [3]
    :linenos:


Creating a test file
~~~~~~~~~~~~~~~~~~~~

Here we create a test file to use for the encoder. This is just for
the sake of the example.

.. literalinclude:: ../../../examples/encode_decode_file/encode_decode_file.cpp
    :language: c++
    :start-after: //! [4]
    :end-before: //! [5]
    :linenos:


Using the file encoder and decoder
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

As with the storage encoders we now build the file encoder and
decoder.

.. literalinclude:: ../../../examples/encode_decode_storage/encode_decode_storage.cpp
    :language: c++
    :start-after: //! [6]
    :end-before: //! [7]
    :linenos:

Also the encoding/decoding loop is similar to the
:ref:`encoding_and_decoding_large_objects` example since we
potentially need more than one encoder/decoder pair to code the entire
file.

.. literalinclude:: ../../../examples/encode_decode_storage/encode_decode_storage.cpp
    :language: c++
    :start-after: //! [8]
    :end-before: //! [9]
    :linenos:
