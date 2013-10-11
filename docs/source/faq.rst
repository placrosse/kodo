==========================
Frequently Asked Questions
==========================

.. _faq:

This page tries to answer common questions about Network Coding and Kodo.


What is a generation?
---------------------

A generation is a group of symbols that are encoded and decoded together.
A generation is also known as a "block".

Why do we need generations?
-------------------

If a whole file was considered one big block, then the
computational complexity of the encoding and decoding operations would
be very high. This is especially problematic on mobile and embedded devices
with limited computational capabilities. Therefore, large data sets are
typically split into several equal-sized generations.

What is the encoding vector?
------

The encoding vector is a part of a coded packet that contains the coding
efficients that indicate which symbols (and which multiple of those symbols)
were used to calculate the coded packet.

What is a finite field?
-------

A finite field or Galois field is a field that contains a finite number of
elements. Essentially, it is a set of numbers (elements) and rules for
arithmetic operations (primarily addition and multiplication) which are defined
so that their result is always a field element.

What does linear dependency mean?
-------

A packet is considered non-innovative or linearly dependent if it only
contains information about previously known symbols. In other words, the
packet can be reduced to a zero vector using the linear combination of some
(partially) decoded symbols.

How does coding affect delay?
-------

The fact that packets need to be decoded has an impact on delay.
The actual delay depends heavily on the size of generations, since decoding is
usually completed after receiving all encoded packets in a generation.
The generation size should be chosen to fit the required delay values.

What is systematic coding?
--------

It is not always necessary to transmit encoded packets while using
Network Coding. Uncoded packets can be considered primitive linear combinations,
so they can be processed by the decoder. Systematic coding means
transmitting each generation in two stages. In the first stage, the sender
transmits all packets uncoded. Each of these packets will contain new useful
information for the individual receivers. In the second stage, the sender
generates random linear combinations of the original symbols in order to
correct any packet losses which might have occurred during the first stage.

What is the code density?
------------------------

The code density can be defined as the ratio of non-zero scalars in an
encoding vector. Full density can be achieved by selecting coding coefficients
according to a random uniform distribution. In contrast, sparse codes use
many zero coefficients in the encoding vectors which makes the encoding process
significantly faster.

How does coding affect the overhead?
------------------------------------

Network Coding involves some overhead as it is necessary to communicate
additional information in the coded packets (in the encoding vectors).
In practice, the size of the encoding vector is generally small compared to
the packet payload. The exact size depends on the finite field size, the
generation size and the coding vector representation.

Another source of overhead is linear dependency since a random code might
produce a small number of linearly dependent (redundant) coded packets.
This should be considered if we choose a small field size or low code density.

In practice, we can use a systematic code to ensure reliability with a
low overhead. This is the recommended approach in single-hop networks.

When are the lost symbols/packets recovered?
--------------------------------------------

Let's suppose the N packets were lost from a generation and the sender does
not have any information about which packets were lost. In this case, at least
N coded packets are required to recover them. Note that the packets will not be
recovered one-by-one, but all at once after the decoder processes N innovative
coded packets.
