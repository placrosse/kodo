Overview
========

.. _overview:

Kodo includes many iteresting techniques and codes. THis document provides an
overview of the most important features in Kodo and which codes Kodo support.

Features
--------

Recoding
    One of the most prominent features of Network Coding is the possibility to
    use coding in the network (recoding) and not only at the sender (encoding)
    and the receiver (decoding).

Systematic coding
    The sender can send some or all of the symbols in the original data block
    uncoded. This is useful in simple topologies as it increases the decoding
    throughput.

On-the-fly coding
    allows a sender to encode over a growing block of data. Useful for live
    content where the data becomes available over time, but where coding is
    applied when the first bit arrive.

Partial decoding
    Make it possible for a receiver to decode some of the data it is receiving
    before the entire data block have been decoded. Makes coding more compatiable
    with error resiliant codecs (video, audio) as instead of receiving wither
    the whole data block or nothing, a partial data block can be received.

Variable symbol length (comming soon)
    The symbols in a block of coded data can have different lengths. This can e.g.
    be useful when coding over message type data, in order to allocate on symbol
    per message.

Real-time adjustable density
    The density at the sender can be adjusted realtime which permits adaptation
    to chainging network conditions.

Symbol pruning
    dropping symbols at the encoder which have been decoded at the decoder
    description

File encoder
    Encode directly from a file at the sender, the data in the file is
    automatically split into generations
    Allows
    to directly encode files (unfortunately we don't have a file decoder yet)
    description

Zero copy api
    Allows fast and memory efficient copying.

Active memory management
    Description

hardware optimized (on select hardware)
    Optimization for varios CPU architectures, using SIMD instructinos and
    various coding algorithms to provide the best performance.


Random Linear Network Coding (RLNC) Variants
............................................

Kodo provides several different codes, primarily standard Random Linear
Network Code and multiple variants thereof.

Standard RLNC
    All the symbols are combined uniformly at random. Generally this type of
    coding is "dense" since the symbols in the data block is mixed as much as
    possible, note that for small field sizes this does not hold.


Sparse RLNC with uniform densiy
    With some probability a symbols is not used when encoding a symbol, for the
    remaining symbols coding is performed as in the standard RLNC case. This is
    typcally useful in cases where the block size is very high since the density
    can be reduced very significantly without any negative effect and at the
    same time increase the decoding throughput significantly.

Sparse RLNC with fixed density
    A fixed number of symbols are combined at random. This is typically used in
    cases where feedback from the decoder is possible which allows the coding at
    the encoder to be tuned to the state of the decoder.

Seed RLNC
    Instead of sending the full coding vector a small seed used to generate the
    coding vector can be sent. This reduces the overhead but makes recoding
    difficult and in some cases impossible, so this is typically used when
    recoding is not necessary or used very sparingly.


Other Code Variants and Approaches
..................................

Other supported codes and approches:

Reed-Solomon code
    Traditional RS-code which does not support recoding.

Carousel code
    Also called a repetition code, the data is simply transmitted in a round
    robin fashion.

Random Annex overlay code
    Enables mixing of several generations. Through the use of multistage
    decoding this techniques can offer increased decoding throughput at the cost
    of increased decoding delay.



Platforms
---------

Kodo is written in C++ library so it is portable to a wide range of platforms.
To ensure that we do not break compatibility with a supported platform we
have a buildbot instance building the Kodo libraries. You can check the
status on the `Steinwurf Buildbot`_ page.

.. _Steinwurf Buildbot: http://176.28.49.184:12344

.. note:: The buildbot is used for several different projects you will find the
  Kodo project in the overview on the main page.

The buildbot pages will also provide you with up-to-date information on which
platforms and compilers we are currently testing. If you have a specific
platform or compiler which you would like to see Kodo support, `drop us a line`_.

.. _drop us a line: http://steinwurf.com/contact-us/



