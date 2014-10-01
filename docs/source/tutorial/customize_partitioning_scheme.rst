.. _customize_partitioning_scheme:

Customize Partitioning Scheme
-----------------------------

In many applications we need to deal with data which does not fit into
a single encoder/decoder. In such cases we need a strategy for how to
partition and assign chucks of the data to separate encoders and
decoders.

.. note:: Ideally we would create a single encoder/decoder pair for
          any object. We could do this simply by increasing the number
          of symbols or the size of the symbols until the data would
          fit. However in practice this does not work well.

          When we increase the number of symbols in an encoder/decoder
          we also increase the computational complexity and it often
          not practical to have blocks with more than a few thousand
          symbols. Likewise increasing the size of a symbol may not be
          feasible due to constraints in the underlying systems
          e.g. the network MTU (Maximum Transfer Unit) etc.

In Kodo we call the algorithm defining how to segment a large object
into smaller ones the block partitioning scheme.

In the following we will show how to define new partitioning schemes
and thereby customize the behavior of our storage/file encoders and
decoders (:ref:`encode_decode_storage` and :ref:`encode_decode_file`).

Block partitioning API
~~~~~~~~~~~~~~~~~~~~~~

In Kodo a valid block partitioning scheme must implement the following
API:

.. literalinclude:: ../../../doxygen/block_partitioning_type.doxygen
    :language: c++
    :linenos:

Defining a custom scheme
~~~~~~~~~~~~~~~~~~~~~~~~

In this case we will implement a partitioning scheme which keeps the
symbol size fixed and creates blocks with exactly the same number of
symbols in each.

.. note:: We do not recommend using the partitioning scheme presented
          here in practice. Since for certain input we will only have
          one symbol per block. Furthermore it has not been tested to
          work will all input values.

Lets see the implementation of the example partitioning scheme.

.. literalinclude:: ../../../examples/customize_partitioning_scheme/customize_partitioning_scheme.cpp
    :language: c++
    :start-after: //! [0]
    :end-before: //! [1]
    :linenos:

In the following we used the :ref:`encode_decode_storage` example as a
starting point but changed the types of the ``storage_encoder`` and
``storage_decoder`` as follows:

.. literalinclude:: ../../../examples/customize_partitioning_scheme/customize_partitioning_scheme.cpp
    :language: c++
    :start-after: //! [4]
    :end-before: //! [5]
    :linenos:

It is possible to query the partitioning scheme about the way the
object was split. The object encoder/decoder uses the
``kodo::object::partitioning`` layer (defined in
``SEC/kodo/object/partitioning.hop``). We can query that layer to get
information about the how partitioning was done.

.. literalinclude:: ../../../examples/customize_partitioning_scheme/customize_partitioning_scheme.cpp
    :language: c++
    :start-after: //! [6]
    :end-before: //! [7]
    :linenos:

For the given input values:

.. literalinclude:: ../../../examples/customize_partitioning_scheme/customize_partitioning_scheme.cpp
    :language: c++
    :start-after: //! [2]
    :end-before: //! [3]
    :linenos:

We would see the following partitioning:

.. code-block:: none

   Block = 0 symbols = 20 symbol size = 64
   Block = 1 symbols = 20 symbol size = 64
   Block = 2 symbols = 20 symbol size = 64
   Block = 3 symbols = 20 symbol size = 64
   Block = 4 symbols = 20 symbol size = 64

As we expect all blocks have the same number of symbols.
