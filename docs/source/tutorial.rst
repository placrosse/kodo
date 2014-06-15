.. _tutorial:

Tutorial
========

In this tutorial we will demonstrate different features of Kodo and
see how it can be used to encode and decode data.

A minimal example
-----------------

The following code shows a complete and working encoding/decoding
example:

.. literalinclude:: ../../examples/tutorial/basic.cpp
    :language: c++
    :linenos:

Kodo implements a number of different erasure correcting codes and in
particular a number of different variants of different codes. In this
example we have chosen to use a particular version of a RLNC code
(Random Linear Network Code) located in the following header file:

.. literalinclude:: ../../examples/tutorial/basic.cpp
    :language: c++
    :start-after: //! [0]
    :end-before: //! [0]
    :linenos:

The full RLNC code is one of the most common RLNC variants, and
provides several of the advantages that RLNC codes have over
traditional erasure correcting codes. However, for the time being we
will just use it as a standard erasure correcting code, namely to
encode and decode some data.

In the following we will go through three of the the key-parameters to
choose when configuring an erasure correcting code. The first two
parameters are the number of ´´symbols´´ and the ´´symbol_size´´.



 *
 * Rate-less (this means that we can in principle generate an infinite amount of redundancy
 * Supports recoding

We will discuss its properties more in the
following.


is the canonical RLNC code. It    is a standard RLNC code where the encoding vector
i.e. the coding coefficients used for the encoding is included in each
packet. Most erasure correcting codes all

Adding code samples
-------------------

This is an example of an included code sample.

.. literalinclude:: ../../examples/tutorial/basic.cpp
    :language: c++
    :start-after: main()
    :end-before: return 0;
    :emphasize-lines: 12,15-18
    :linenos:

We should consider only using start-after and end-before as this would allow the
documentation to change with the code.
By using linenos and emphasize-lines we can possibly introduces bugs in the
docs as they are line dependent.

.. source: http://sphinx-doc.org/markup/code.html#includes
