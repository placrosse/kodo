.. _recoding_data:

Recoding Data
-------------

One of the key features of RLNC is the ability to recode. What this
means is if the data is to be transmitted through a chain of nodes,
the data can be re-encoded at each node. Examples of such networks
include:

* Relay networks
* P2P networks,
* multi-path networks, and
* mesh networks.

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

To simulate a lossy multi-hop network the decoding loop has also been modified,
a bit.

.. literalinclude:: ../../../examples/tutorial/recoding.cpp
    :language: c++
    :start-after: //! [2]
    :end-before: //! [3]
    :linenos:

Both the channel from ``encoder`` to ``decoder1`` and the channel from
``decoder1`` to ``decoder2`` is simulated to have a 50% success rate.

.. image:: ../../images/recoding_setup.svg
   :align: center

In a network without recoding, the overall success rate would be the product of
all the networks success rates, i.e., for this network the success rate would be
50% * 50% = 25%.
When using recoding the overall success rate will be the minimum success rate
for any of the nodes, i.e., 50% in this example.

The output when running this example looks like so:

.. code-block:: none

    Encoded count = 28
    Dropped count = 19

