# Go-Back-N Protocol

## Overview

The Go-Back-N (GBN) protocol is a reliable data transfer protocol used in communication networks to ensure the correct delivery of packets over unreliable connections. It is a sliding window protocol that enables efficient use of network resources by allowing multiple packets to be sent before requiring an acknowledgment. However, if a packet is found to be lost or corrupted, GBN ensures reliability by retransmitting the packet and all subsequent packets in the window.

### Key Features
- **Sliding Window Mechanism**: The sender maintains a window of *N* packets that can be sent without waiting for an acknowledgment.
- **Cumulative Acknowledgments**: The receiver acknowledges the last successfully received packet in order, implicitly confirming all previous packets.
- **Retransmission on Error**: If an acknowledgment is not received within a timeout period, the sender retransmits the unacknowledged packet and all subsequent packets in the window.

### Workflow
1. The sender transmits up to *N* packets without waiting for individual acknowledgments.
2. The receiver checks each incoming packet for errors and sends an acknowledgment for the last correctly received, in-order packet.
3. If the sender detects a missing or delayed acknowledgment (via a timeout), it retransmits all packets from the lost or corrupted one onward.

### Advantages
- Efficient for networks with low error rates due to its pipelining approach.
- Simple implementation compared to other advanced protocols like Selective Repeat.

### Limitations
- Inefficient in high-error environments since retransmitting an entire window increases overhead.
- Increased delays for retransmission when errors occur.

### Applications
- Used in Transport Layer protocols and some network-layer protocols to ensure reliable communication.


![image](https://github.com/user-attachments/assets/35ed990d-eb9c-477e-b927-c73c2bee1d5b)
