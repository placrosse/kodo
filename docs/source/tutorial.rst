.. _tutorial:

Tutorial
========

In this tutorial we will demonstrate different features of Kodo and
see how it can be used to encode and decode data.

A minimal example
-----------------

In the following we will walk through the process of creating an
encoder and decoder for a single buffer of data. We will start from an
empty file and slowly expand it. Once we have a working example we
will modify it in a number of steps to show different properties and
features of the erasure correcting codes and the library.

Our starting point will be the following:

.. literalinclude:: ../../examples/tutorial/empty.cpp
    :language: c++
    :linenos:

Kodo implements a number of different erasure correcting codes. In
this example we have chosen to use a particular version of a RLNC code
(Random Linear Network Code) located in the following header file:

.. literalinclude:: ../../examples/tutorial/basic.cpp
    :language: c++
    :start-after: //! [0]
    :end-before: //! [1]
    :linenos:

The full RLNC code is one of the most common RLNC variants, and
provides several of the advantages that RLNC codes have over
traditional erasure correcting codes. However, for the time being we
will just use it as a standard erasure correcting code, namely to
encode and decode some data.

In the following we will go through three of the the key-parameters to
choose when configuring an erasure correcting code. The first two we
will define are the number of ``symbols`` and the ``symbol_size``.

.. literalinclude:: ../../examples/tutorial/basic.cpp
    :language: c++
    :start-after: //! [2]
    :end-before: //! [3]
    :linenos:

In general if we want to encode a block of data, we split this block
into some number of symbols of a specific size. If you multiply the
number of symbols with the symbol size you get the total amount of
data in bytes that will be either encoded or decoded.

.. note:: Sizes in Kodo are always measured in bytes. So if you see a
          variable of function name that includes the word size bytes
          is the unit used.

.. note:: In network applications a single symbol typically
          corresponds to a single packet (for example an UDP
          datagram).

Let us briefly outline the impact of changing the number of symbols
and the symbol size.

symbols
.......
* Increasing the number of symbols will also increases the
  computational complexity and can therefore result in slower
  encoding/decoding.
* For some variants of RLNC increasing the number of symbols also
  increases the per-packet overhead (due to added coding
  coefficients).
* Increasing the number of symbols may result in a larger per-symbol
  decoding delay. The reason for this is that when we increase the
  number of symbols that are encoded the decoder has to received more
  symbols before decoding.

symbol size
...........
* The choice of symbol size typically depends on the application. For
  network applications we may choose the symbol size according to the
  network MTU (Maximum Transfer Unit) so that datagrams do not get
  fragmented as they traverse the network. In those cases symbols
  sizes are typically around 1300-1400 bytes. On the other hand for
  storage applications the symbol size is typically much larger
  e.g. in the order of several megabytes.

The third key parameter is the finite field, or more specifically the
field size used. The core mathematics used in most erasure correcting
codes are based on finite fields. This parameter also introduces a set of
trade-offs.

Field size
..........
* Increasing the field size will also increase the probability of
  successful decoding.
* On the other hand increasing field size typically also leads to
  higher computational complexity which results in slower
  applications.

In the given example the following two lines selects the field size
for both the encoder and decoder.

.. literalinclude:: ../../examples/tutorial/basic.cpp
    :language: c++
    :start-after: //! [4]
    :end-before: //! [5]
    :linenos:

As shown above this is done by passing a type defining the finite
field as the first argument to the chosen encoder and decoder. Since
fast finite field computations are not only useful in erasure
correcting codes this part of the functionality has be split into a
second library called Fifi (www.github.com/steinwurf/fifi). The Fifi
library defines a number of different finite fields such as
``binary``, ``binary4``, ``binary8`` and ``binary16``. To switch
between the different field you can simple replace the
``fifi::binary8`` string with one of the other field types
e.g. ``fifi::binary``.

Once the key parameters have been selected we are ready to create an
encoder and decoder to perform the actual coding.

.. literalinclude:: ../../examples/tutorial/basic.cpp
    :language: c++
    :start-after: //! [6]
    :end-before: //! [7]
    :linenos:

The encoder and decoder types define a nested type called the
``factory``. Using the factory we can configure and build encoders and
decoders. We instantiate the factory using chosen number of symbols
and symbol size. Invoking the ``build()`` function will return a
smart-pointer to a new encoder or decoder. In C++ a smart-pointer is
one which behaves just like a normal pointer to an object but which
will delete the object when there are no more references to
it. Typically the factory type used is a *pooled* factory which means
that when an encoder or decoder is about to be delete instead they
will be returned to a memory pool for reuse. The next call to build
will then return one of the reused encoders/decoders. This type of
memory use increases performance and reduces the pressure on the
memory allocation system by performing less allocations.

We are now ready to encode and decode some data, but first we need to
define two buffers.

.. literalinclude:: ../../examples/tutorial/basic.cpp
    :language: c++
    :start-after: //! [6]
    :end-before: //! [7]
    :linenos:


The first buffer we will create is the ``payload`` buffer. Once we
start coding this buffer will contain an single encoded symbol which
we can pass to the decoder. Besides the encoded symbol data the
payload buffer will also contain some internal meta-data describing
how the symbol was encoded. The format and size of this data depends
on the chosen erasure correcting code. But, fortunately we don't have
to worry about that, as long as we provide a buffer large enough. The
needed size of the buffer is returned by the
``payload_size`` call.

The second buffer contains the data we wish to encode. As mentioned
earlier the size of this buffer is the number of symbols multiplied by
the symbol size. For convenience we can call the ``block_size``
function to get this value. In this case we are not encoding real data
so we just fill the ``block_in`` buffer with some randomly generate data.

Once the buffers have been created we can call the ``set_symbols``
function on the encoder to specify which buffer it should encode.

.. literalinclude:: ../../examples/tutorial/basic.cpp
    :language: c++
    :start-after: //! [10]
    :end-before: //! [12]
    :linenos:

for the encoder to  the
The encodeThis buffer will contain single encoded symbol now have an
encoder and a decoder ready to


.. source: http://sphinx-doc.org/markup/code.html#includes
