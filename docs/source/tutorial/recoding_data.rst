.. _recoding_data:

Recoding Data
-------------

One of the key features of RLNC is the ability to recode. What this
means is if the data is to be transmitted through a chain of nodes,
the data can be re-encoded at each node.  A examples of such networks
include:

* Relay networks
* P2P networks, and
* multi-path networks

The complete example code for this is as follows.

.. literalinclude:: ../../../examples/tutorial/recoding.cpp
    :language: c++
    :linenos:

Again only a few lines has changed from the basic example. We've
changed the setup a bit so that we now have two instead of one
decoder.  In the example the ``encoder`` encodes and sends data to
``decoder1``.  This data is then decoded and further recoded by
``decoder1`` and then finally sent to ``decoder2`` where it's decoded.

.. literalinclude:: ../../../examples/tutorial/recoding.cpp
    :language: c++
    :start-after: //! [0]
    :end-before: //! [1]
    :linenos:
