.. _tutorial:

Tutorial
========

In this tutorial we will demonstrate different features of Kodo and
see how it can be used to encode and decode data.

A minimal example
-----------------

The following code shows a complete and working encoding/decoding
example.

.. literalinclude:: ../../examples/tutorial/basic.cpp
    :language: c++
    :linenos:

In this example we have chosen to use a particular version of a Random
Linear Network Code (RLNC) which we call full RLNC located in the
header:

.. literalinclude:: ../../examples/tutorial/basic.cpp
    :language: c++
    :start-after: //! [0]
    :end-before: //! [0]
    :linenos:

The full RLNC codec is a standard RLNC code where the encoding vector
i.e. the coefficients used for the encoding is included in each
packet.

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
