Frequently Asked Questions
==========================

.. _faq:

This page tries to answer common questions about Network Coding and Kodo.


.. contents::


Network Coding
--------------

What is a source?
.................

A source is a node that transmits data to one or more other node(s).

What is a sink?
...............

A sink is a node that receives data from other node(s).

What is a relay?
................

A relay is a node that receives data from other node(s) and transmit that data to other node(s), typically the relay is not itself interested in receiving the data.

Original data
.............
 
Some data that is to be transferred from a source to one or more sinks.

What is encoding?
.................

Encoding is performed at source(s) where the original data to be send is encoded and packetized and transmitted via the network.

What is decoding?
.................

Decoding is performed at sink(s) where the coded data is decoded in order to recreate the original data from the source(s).

What is recoding?
.................

Recoding can be performed in the network at relay node(s) between the source and the sink. Such a node can combine received coded packets in order to create new coded packets. A sink that has not fully decoded the data can also recode.


What is a finite field?
.......................


A finite field is a mathematical construct and entails to much explanations to be included here. It is not necessary to have a deep understanding of finite fields, however, you must be familiar with a couple of related terms. Finite fields can be used to implement linear codes on computers with fixed precision, which is what they are used for in Kodo. Several finite fields are implemented in Fifi, which you can find alongside Kodo on GitHub.

A finite field (or Galois field) is a field that contains a finite number of
elements. In other words, it is a set of numbers (elements) and rules for
arithmetic operations (primarily addition and multiplication) which are defined
so that their result is always a field element. In Network Coding, the typically
used finite fields are FF(2), which only contains 2 elements, and FF(2^8) that
contains 256 elements. With GF(2), one field element signifies a single bit of
data. With FF(2^8), one field element signifies one byte of data.


What is an element?
...................

An element is a variable with the type of a specific finite field variable.

What is a symbol?
.................

A symbol is a vector of elements that represent some data. E.g. 8 elements in FF(2) represents a byte.


What is a generation?
.....................

A generation is a group of symbols that are encoded and decoded together.

The original data is divided into generations, of size :math:`g \cdot m`. Each generation constitutes :math:`g` symbols, each of size :math:`m`. The data from each generation is encoded, decode and recoded separately.

====  ====  =====  ====
Generation
-----------------------
*S1*  *S2*   ...   *Sg*
====  ====  =====  ====

A generation is sometimes also referred to as a *source block* or a *batch*.

What is the generation size?
............................

is the number of symbols in the generation denoted :math:`g`


What is a coded symbol?
.......................

A coded symbol is


What is the coding vector?
..........................

The coding vector describes how a coded symbol was coded. It contains a coeffcient (which is a element) for each symbol in the generation. This vector of elements are the coefficients which have been multiplied onto the original symbols.



What is a coded packet?
.......................

Is a pair of a coded symbol and a coding vector. To decode a coded symbol the corresponding codeding vector must be known and therefore typically the two are tranmitted together in a single packet.

===============  ===============
          Packet
--------------------------------
Vector          Coded Symbol
===============  ===============


What is linear dependency?
..........................

A packet is non-innovative or linearly dependent if it only
contains information about previously known symbols. In other words, the
packet can be reduced to the zero vector using the linear combination of some
(partially) decoded symbols.

What is systematic coding?
..........................

It is not always necessary to transmit encoded packets while using
Network Coding. Systematic coding means transmitting each generation in two
stages. In the first stage, the sender transmits all original symbols uncoded.
In the second stage, the sender generates random linear combinations of the
original symbols in order to correct any packet losses which might have
occurred during the first stage.

What is the code density? 
.........................

Sometimes also refered to as the degree

The code density can be defined as the ratio of non-zero scalars in an
encoding vector. Full density can be achieved by selecting coding coefficients
according to a random uniform distribution. In contrast, sparse codes use
many zero coefficients in the encoding vectors which makes the encoding process
significantly faster.

The density of a coding vector is the ratio of non-zero elements in the vector

:math:`d(\boldsymbol{h}) = \frac{\sum_{k=1}^g \boldsymbol{h}_k \neq 0}{g}`

where:

:math:`\boldsymbol{h}_i` is the vector

:math:`g` is the generation size



How can the role of a node change during a session?
...................................................

How does coding affect delay?
.............................

The fact that packets need to be decoded has an impact on delay.
The actual delay depends heavily on the size of generations, since decoding is
usually completed after receiving all encoded packets in a generation.
The generation size should be chosen to fit the required delay values.


How does coding affect the overhead?
....................................

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

How does the generation size affect coding?
...........................................

How does the field size impact coding?
......................................

How does the density impact coding?
...................................

Why do we need generations?
...........................

If a whole file was considered one big block, then the
computational complexity of the encoding and decoding operations would
be very high. This is especially problematic on mobile and embedded devices
with limited computational capabilities. Therefore, large data sets are
typically split into several equal-sized generations.


When are the lost symbols/packets recovered?
............................................

Let's suppose the N packets were lost from a generation and the sender does
not have any information about which packets were lost. In this case, at least
N coded packets are required to recover them. Note that the packets will not be
recovered one-by-one, but all at once after the decoder processes N innovative
coded packets.

Kodo
----



