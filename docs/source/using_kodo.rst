Using Kodo
==========

.. _using_kodo:

This page provides background information and useful guidelines about
using Kodo in common communication scenarios.

Network Coding
--------------

Network coding is an interesting technique which can provide throughput
improvements and a high degree of robustness in packet networks.
It breaks with a fundamental principle of conventional communication networks
by allowing any network node to recombine several input packets into one,
instead of simply forwarding them.


Overview of Random Linear Network Coding (RLNC)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
The figure below gives a basic overview of the operations performed in a
network coding system. If you intend to encode a large file then it should 
be split into several blocks, also called generations each consisting
of **g** packets. If the whole file was considered one big block, then the
computational complexity of the encoding and decoding operations would
be very high.

.. image:: ../images/nc_overview.png
   :scale: 50
   :align: center

The top component in the figure is the **encoder** that
generates and transmits linear combinations of the original data packets
in a given generation. Addition and multiplication are performed over
a Galois field, therefore a linear combination of several packets will
have the same size as a single packet.
Note that any number of encoded packets can be generated for a single
generation.
The middle layer in this system is the wireless **channel** where packet
erasures may occur depending on the channel conditions. The network
nodes receive a series of encoded packets that are passed to the
**decoder** (the bottom component in the figure) which will be able to
reconstruct the original data packets after receiving at least *g*
linearly independent packets.


Scenarios
---------

Network coding is useful for ensuring reliability and/or increasing
throughput in a communication network.
An obvious benefit of using network coding is that a network node is
no longer required to gather all data packets one-by-one, instead it
only has to receive enough linearly independent encoded packets.

Point-to-point Communication
~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Reliable Multicast
~~~~~~~~~~~~~~~~~~

Let's assume that we want to reliably transmit some data from a mobile
device or computer on the network to several nearby receivers via a
lossy wireless link.
The problem is that unicast transmissions provide a poor utilization of the
available network resources for one-to-many scenarios.
If all receivers are interested in the same content, we can efficiently
utilize the wireless channel with broadcast transmissions.

Under ideal channel conditions all broadcast packets would be delivered to
all nodes simultaneously. In real-life wireless networks packet losses
frequently occur, thus some sort of retransmission scheme is necessary to
ensure reliability. We need to correct packet losses at the receivers.
A simple solution would be that the receiver nodes request all missing
packets from the original source. This would imply that every lost packet
is transmitted again, and if packet losses are uncorrelated then most
retransmissions will not be useful to specific receivers since they have
already received those packets in the first place.
To put it differently, it is likely that a single retransmission will
only benefit a single receiver.

By using Network Coding, we can simultaneously benefit
multiple nodes with a single retransmission by sending a coded packet
instead of choosing a specific original packet.
Due to the uncorrelated nature of packet losses, the receivers usually hold
different sets of packets. The source can create and send random linear
combinations of the original data. One coded packet
carries information which can potentially correct different errors at
different nodes simultaneously.




