Frequently Asked Questions
==========

.. _faq:

This page tries to answer common questions about Network Coding and Kodo.

*. What is a generation?
------------------------

A generation is a group of symbols that are encoded and decoded together.
A generation is also known as a "block".

*. What is the encoding vector?
------------------------

The encoding vector is a part of a coded packet that contains the coding
efficients that indicate which symbols (and which multiple of those symbols)
were used to calculate the coded packet.

*. What is a finite field?
------------------------

A finite field or Galois field is a field that contains a finite number of
elements. Essentially, it is a set of numbers (elements) and rules for
arithmetic operations (primarily addition and multiplication) which are defined
so that their result is always a field element.

*. What does linear dependency mean?
------------------------

*. How does coding affect delay?
------------------------

The fact that packets need to be decoded has an impact on delay.
The actual delay depends heavily on the size of generations, since decoding is
usually completed after receiving all encoded packets in a generation.
The generation size should be chosen to fit the required delay values.

*. How does coding affect the overhead?
------------------------

*. Why do we need generations?
-----------------------------

If a whole file was considered one big block, then the
computational complexity of the encoding and decoding operations would
be very high. This is especially problematic on mobile and embedded devices
with limited computational capabilities. Therefore, large data sets are
typically split into several equal-sized generations.