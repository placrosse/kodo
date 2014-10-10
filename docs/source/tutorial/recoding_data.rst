.. _recoding_data:

Recoding Data
-------------

One of the key features of RLNC is the ability to recode. Recoding is when the
encoded data is re-encoded. This is very useful in scenarios where the data is
to be transmitted through a chain of nodes. Here the data can be re-encoded at
each node. Examples of such networks include:

* Relay networks
* P2P networks,
* multi-path networks, and
* mesh networks.

In this example a total of three nodes is in the network. At each link a loss
can occure at a 50% chance. The complete example code for this is as follows.

.. literalinclude:: ../../../examples/tutorial/recoding.cpp
    :language: c++
    :linenos:

Compared to the basic example, the setup and decoding loop has changed.
In the setup phase two decoders are now created, instead just one.

.. literalinclude:: ../../../examples/tutorial/recoding.cpp
    :language: c++
    :start-after: //! [0]
    :end-before: //! [1]
    :linenos:

The decoding loop has changed to simulate a lossy multi-hop network.

.. literalinclude:: ../../../examples/tutorial/recoding.cpp
    :language: c++
    :start-after: //! [2]
    :end-before: //! [3]
    :linenos:

The ``encoder`` encodes data which is then "transmitted" to ``decoder1``.
This data is then both decoded and recoded by ``decoder1``. The resulting
recoded data is then finally "transmitted" to ``decoder2`` where it's decoded.
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
