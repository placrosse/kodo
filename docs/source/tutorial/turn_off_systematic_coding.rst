

.. _turn_off_systematic_coding:

Turn Off Systematic Coding
--------------------------

A simple, yet clever technique called systematic encoding can be used
to improve the performance of network coding. The way it works is to
initially send everything uncoded, and then start the encoding.  The
benefit of this is that as the receivers initially has no data, all
data will be useful for them. So if the symbols are safely received by
the decoder, it can get the data "for free" without the need for
decoding.  The Kodo library has built-in support for this
approach. The sample code, for this is again build based on the basic
example.

.. literalinclude:: ../../../examples/tutorial/turn_systematic_off.cpp
    :language: c++
    :linenos:

What's added in this example here is the use of ``is_systematic_on`` and
``set_systematic_off``.

.. literalinclude:: ../../../examples/tutorial/turn_systematic_off.cpp
    :language: c++
    :start-after: //! [0]
    :end-before: //! [1]
    :linenos:

Initially Kodo's Full RLNC encoder has the systematic phase enabled
per default.  As seen in the previous example, this is automatically
turned off when all symbols has been send once. In this example we
turn off the systematic phase before entering the coding loop. This
can easily seen from the output when running the example:

.. code-block:: none

    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Bytes used = 1417
    Encoded count = 27
    Dropped count = 11

Here the bytes used is always the same as all the symbols are encoded
by the encoder.  For most use cases the systematic phase is
beneficial. However it should be avoided if

* The state of receivers is unknown. If that's the case, using the
  systematic approach might result in sending redundant data, as the
  receivers might already have some of the data.
* The setup has multiple sources. If this is the case, the sources
  should not send the same data, as this can be redundant for the
  receivers.
