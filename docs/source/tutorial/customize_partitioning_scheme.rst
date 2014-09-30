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
symbol size fixed but creates blocks with exactly the same number of
symbols in each.

.. note:: We do not recommend using the partitioning scheme presented
          here in practice. Since for certain input we will only have
          one symbol per block.

          An alterntive apporach would be to adjust the symbol size

.. literalinclude:: ../../../examples/customize_partitioning_scheme/customize_partitioning_scheme.cpp
    :language: c++
    :linenos:
