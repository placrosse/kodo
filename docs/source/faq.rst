cd Frequently Asked Questions
==========================

.. _faq:

This page tries to answer common questions about Network Coding and Kodo.


.. contents::


Network Coding
--------------

What is a source, sink, and relay?
..................................

A source is a node that transmits data to one or more other node(s).

A sink is a node that receives data from other node(s).

A relay is a node that receives data from other node(s) and transmit that data to other node(s), typically the relay is not itself interested in receiving the data.

What is original data?
......................
 
The original data is the data that is to be transferred from a source to one or more sinks with a size :math:`B` in bits.

What is a finite field?
.......................

A finite field (FF) (or Galois field) is a mathematical construct and entails to much explanations to be included here. It is not necessary to have a deep understanding of finite fields, but some understanding is useful. Simplified a FF is a variable where special rules are defined for the arithmetic operations. These rules guarentee that the result of operating on a finite field will always result in a value that is in the field which is very useful on computers with fixed precision. One common field is the binary field FF(2) where addition is defined by the XOR operation. Typically FF(2) or FF(2^8) is used since they correspond to a bit and a byte respectively. The size of a field is typically denoted :math:`q`.

What is an element?
...................

An element is an element in a FF which can be though of as a variable with the type of a specific finite field variable.

What is a symbol?
.................

A symbol is a vector of FF elements that represent some data. The size of a symbol is given by the number of elements and the size of each element.

:math:`|\boldsymbol{s}| = n \cdot \log_2(q) ~ [b]`

As an example 16 elements in FF(2) can represent two bytes.

What is a generation?
.....................

Each generation constitutes :math:`g` symbols of size :math:`m`, where :math:`g` is called the generation size. The :math:`g` original symbols in one generation, are arranged in the matrix :math:`\boldsymbol{M}= [ \boldsymbol{m}_1 ; \boldsymbol{m}_2 ; \hdots ; \boldsymbol{m}_g ]`, where :math:`\boldsymbol{m}_i` is a column vector. In an application the block of data can be a file or a part of a media stream, and is divided into :math:`\lceil \frac{B}{m} \rceil` pieces, called symbols. Generation number 0 constitutes the first `g` symbols, or the first :math:`g \cdot m` bytes of data, there are :math:`\lceil \frac{B}{g \cdot m} \rceil` such generations.

What is the generation size?
............................

The generation size is the number of symbols in the generation denoted :math:`g`.


What is the coding vector?
..........................

The coding vector describes how a coded symbol was coded. It contains a coeffcient (which is a element) for each symbol in the generation.

:math:`\boldsymbol{v} = \{v_0; v_1; ... v_{g-1} \}`

This column vector of elements are the coefficients which have been multiplied onto the original symbols.


What is a coded symbol?
.......................

A coded symbol is a symbol which is a combination of the original symbols in a generation. Therefore a coded symbol is a representation of all the data in a generation, but it has the same size as an original symbol.

:math:`\boldsymbol{x} = \boldsymbol{M} \cdot \boldsymbol{v}`

What is a coded packet?
.......................

Is a pair of a coded symbol and a coding vector. To decode a coded symbol the corresponding codeding vector must be known and therefore typically the two are tranmitted together in a single packet; 

:math:`\{ \boldsymbol{v}, \boldsymbol{x} \}`

What is encoding?
.................

.. _encoding:

To encode a new symbol :math:`\boldsymbol{x}` from a generation at the source, :math:`\boldsymbol{M}` is multiplied with a randomly generated coding vector :math:`\boldsymbol{v}` of length :math:`g`, :math:`\boldsymbol{x} = \boldsymbol{M} \times \boldsymbol{v}`. In this way we can construct :math:`g+r` coded symbols and coding vectors, where :math:`r` is any number of redundant symbols as the code is rateless. When a coded symbol is transmitted on the network it is accompanied by its coding vector, and together they form a coded packet. A practical interpretation is that each coded symbol, is a combination or mix of the original symbols from one generation. The benefit is that nearly infinite coded symbols can be created.

What is decoding?
.................

.. _decoding:

In order for a sink to successfully decode a generation, it must receive :math:`g` linearly independent symbols and coding vectors from that generation. All received symbols are placed in the matrix :math:`\boldsymbol{\hat{X}} = [\boldsymbol{\hat{x}_1} ; \boldsymbol{\hat{x}_2} ; \hdots ; \boldsymbol{\hat{x}_g}]` and all coding vectors are placed in the matrix :math:`\boldsymbol{\hat{V}}=[\boldsymbol{\hat{v}_1} ; \boldsymbol{\hat{v}_2} ; \hdots ;\boldsymbol{\hat{v}_g} ]`, we denote :math:`\boldsymbol{\hat{V}}` the coding matrix. The original data :math:`\boldsymbol{M}` can then be decoded as :math:`\boldsymbol{\hat{M}} = \boldsymbol{\hat{X}} \times \boldsymbol{\hat{V}}^{-1}`. In practice if approximately **any** :math:`g` symbols from a generation are received the original data in that generation can be decoded. This is a much looser condition, compared to when no coding is used, where exactly **all** :math:`g` unique original symbols must be collected.

What is recoding?
.................

.. _recoding:

Any node that have received :math:`g'`, where :math:`g' = [2,g]` is the number of received linearly independent symbols from a generation and is equal to the rank of :math:`\boldsymbol{\hat{V}}`, can recode. All received symbols are placed in the matrix :math:`\boldsymbol{\hat{X}} = [\boldsymbol{\hat{x}_1} ; \boldsymbol{\hat{x}_2} ; \hdots ; \boldsymbol{\hat{x}_{g'}}]` and all coding vectors in the matrix :math:`\boldsymbol{\hat{V}} = [\boldsymbol{\hat{v}_1} ; \boldsymbol{\hat{v}_2} ; \hdots ; \boldsymbol{\hat{v}_{g'}}]`. To recode a symbol these matrices are multiplied with a randomly generated vector :math:`\boldsymbol{w}` of length `g'`, :math:`\boldsymbol{\tilde{v}} = \boldsymbol{\hat{G}} \times \boldsymbol{w}`,  :math:`\boldsymbol{\tilde{x}} = \boldsymbol{\hat{X}} \times \boldsymbol{w}`. In this way we can construct :math:`r'` randomly generated recoding vectors and :math:`r'` recoded symbols. :math:`r'>g'` is possible, however a node can never create more than :math:`g'` independent symbols. Note that :math:`\boldsymbol{w}` is only used locally and that there is no need to distinguish between coded and recoded symbols. In practice this means that a node that have received more than one symbol can recombine those symbols into recoded symbols, similar to the way coded symbols are constructed at the source.


What is linear dependency?
..........................

A packet is non-innovative or linearly dependent if it only
contains information about previously known symbols. In other words, the
packet can be reduced to the zero vector using the linear combination of some
(partially) decoded symbols.

What is systematic coding?
..........................

Systematic coding means first transmitting all symbols in two
stages. In the first stage, the sender transmits all original symbols uncoded.
In the second stage, the sender generates random linear combinations of the
original symbols in order to correct any packet losses which might have
occurred during the first stage.

What is the code density? 
.........................

The code density can be defined as the ratio of non-zero scalars in an
encoding vector. Full density can be achieved by selecting coding coefficients
according to a random uniform distribution. In contrast, sparse codes use
many zero coefficients in the encoding vectors which makes the encoding process
significantly faster. The density of a coding vector is the ratio of non-zero elements in the coding vector

:math:`d(\boldsymbol{h}) = \frac{\sum_{k=1}^g \boldsymbol{h}_k \neq 0}{g}`

where: :math:`\boldsymbol{h}_i` is the vector

The density is sometimes also refered to as the degree


How can the role of a node change during a session?
...................................................

From sink to relay to source

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


How does the field size affect the overhead?
............................................

How does the generation size affect delay?
..........................................

The generation size :math:`g` is the number of symbols over which encoding is performed, and defines the maximal number of symbols that can be combined into a coded symbol. Data is decoded on a per generation level, thus at least :math:`ĝ` symbols must be received before decoding is possible. Hence the size of a generation :math:`g \cdot m` dictates the decoding delay which is the minimum amount of data that must be received before decoding is possible.


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



