Overview
========

.. _overview:

Kodo supports many interesting techniques and codes. This document provides an
overview of the most important features of Kodo and the supported codes.

Features
--------

Recoding
    One of the most prominent features of Network Coding is the possibility to
    use coding at the intermediate network nodes (recoding) and not only at the
    sender (encoding) and the receiver (decoding).

Systematic coding
    The sender can send some or all of the original symbols within a given block
    uncoded. Coded packets can be generated later to repair any packet losses.
    Systematic coding is useful in simple topologies as it increases the decoding
    throughput and decreases the coding overhead.

On-the-fly coding
    The sender can encode over a growing block of data. This is useful for live
    content where the data becomes available over time, potentially at a
    variable rate.

Partial decoding
    The receiver can decode some of the original symbols before the entire data
    block is decoded. This approach is more compatible with error-resilient
    codecs (video, audio) as instead of receiving the whole data block or nothing,
    a partial data block can be retrieved.

Variable symbol length (coming soon)
    The symbols in a block of coded data can have different lengths. The
    fragmentation and aggregation is handled transparently by the library.
    This can be useful when the size of the incoming data packets is variable.

Real-time adjustable density
    The density at the sender can be adjusted in real time which permits
    adaptation to changing network conditions.

Symbol pruning
    The encoder can drop certain symbols which were already decoded at the decoder.

File encoder
    The sender can directly encode data files that are automatically split
    into generations.

Zero copy API
    The encoder and decoder can operate directly on user provided buffers,
    eliminating the need for costly data copies.

Object pooling
    The library can re-use existing encoder and decoder instances to facilitate
    efficient memory management.

Hardware optimized (on select hardware)
    Optimization for various CPU architectures, using SIMD instructions and
    various coding algorithms to provide the best performance.


Random Linear Network Coding codecs
...................................

Kodo provides several different codecs, primarily the standard Random Linear
Network Code (RLNC) and multiple RLNC-variants.

Standard RLNC
    All symbols are combined uniformly at random. In general, this type of
    coding is "dense", since the symbols in the data block are mixed as much as
    possible. Density is lower for small field sizes.

Sparse RLNC with uniform density
    Some symbols are excluded with a given probability during encoding.
    The remaining symbols are combined as in the standard RLNC case. This is
    typically useful when the block size is very high. The density
    can be reduced significantly without any negative effect and the
    the decoding throughput can be increased substantially at the same time.

Sparse RLNC with fixed density
    A fixed number of symbols are combined at random. This can be used
    when feedback is available from the decoder. The encoding process can be
    tuned at the encoder according to the state of the decoder.

Seed-based RLNC
    Instead of sending the full coding vector, a small random seed can be sent
    to generate the coding vector. This reduces the overhead, but makes recoding
    difficult and in some cases impossible. This is typically used when
    recoding is not necessary or used very sparingly.

On-the-fly RLNC
    Symbols can be encoded as they are made available and data is released from
    the decoder as decoding progresses. This is different from traditional block
    codes where all data has to be available before encoding or decoding takes
    place.
    This codec is well suited for low-delay services such as messaging, voice
    over IP or video streaming.


Other codecs and approaches
...........................

Other supported codes and approaches:

Reed-Solomon code
    Traditional Reed-Solomon (RS) code which does not support recoding. The
    current implementation uses a systematic Vandermonde matrix as described in
    `RFC 5510 <http://tools.ietf.org/html/rfc5510>`_.

Carousel code
    Also called a repetition code, the data is simply transmitted in a
    round-robin fashion. This code is mostly useful for simulation purposes and
    performance evaluations. Furthermore it can be used to provide the Compact
    No-Code scheme described in
    `RFC 5445 <http://tools.ietf.org/html/rfc5445>`_.

Random Annex overlay code
    Enables mixing of several generations. By using multi-stage decoding,
    this technique can offer increased decoding throughput at the cost of
    increased decoding delay.
    The Random Annex code is useful in scenarios where large objects needs to be
    transmitted in a feedback constrained system (feedback is expensive or
    impossible) and where using a single large generation is not feasible.


Platforms
---------

Kodo is a C++ library that is portable to a wide range of platforms.
To ensure that we do not break compatibility with a supported platform,
our build system automatically builds and runs the unit tests from Kodo
and the related libraries. You can check the status on the
`Steinwurf Buildbot`_ page.

.. _Steinwurf Buildbot: http://buildbot.steinwurf.dk

.. note:: The buildbot is used for several different projects. The
  Kodo project can be found in the overview on the main page.

The buildbot pages will also provide you with up-to-date information on which
platforms and compilers we are currently testing. If you have a specific
platform or compiler which you would like to see Kodo support, `drop us a line`_.

.. _drop us a line: http://steinwurf.com/contact-us/



