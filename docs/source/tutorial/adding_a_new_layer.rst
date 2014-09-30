.. _adding_a_new_layer:

Adding a New Layer
------------------

Kodo is a set of building blocks rather than one specific code. It is
meant to be modified.

Use-cases:

* You want to change which symbols are included based on receiver
  feedback.
* You want to track the time elapsed since a specific decoder last
  received a packet.
* You want to collect statistics about the decoding
* You want to encode and decode using a sliding window

Adding a Layer
..............

.. literalinclude:: ../../../examples/tutorial/add_layer.cpp
    :language: c++
    :linenos:

link hacking kodo