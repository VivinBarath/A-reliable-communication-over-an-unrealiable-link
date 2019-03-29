We have been introduced to the function calls socket, bind, sendto, and recvfrom to work
with UDP sockets. Assume that the TCP sockets are not there. We know that UDP
sockets are not reliable, meaning that there is no guarantee that a message sent using a
UDP socket will reach the receiver. We want to implement our own socket type, called
MRP (My Reliable Protocol) socket, that will guarantee that any message sent using a
MRP socket is always delivered to the receiver. However, unlike TCP sockets, MRP
sockets may not guarantee in-order delivery or exactly-once delivery of messages. Thus
messages may be delivered more than once (duplicate messages) or out of order (later
message delivered earlier).

          MRP sockets guarantee reliable delivery using a simple approach. The sender sends a
message using a UDP socket, and stores the message and the time it is sent in a table
called the unacknowledged-message table. The receiver, on receiving the message, sends
an ACK message back to the sender. The sender, on receiving the ACK for a message,
removes that message from the unacknowledged-message table. If either the message or
the ACK is lost, a timeout occurs at the sender, and the sender resends the message, and
resets the sending time of that message in the unacknowledged-message table. This
process repeats for each message.

To implement each MRP socket, we use the following:
      1. One UDP socket through which all actual communication happen.
      
      2. Two threads R and S. Thread R handles all messages received from the UDP
      socket, and thread S, handles the timeouts and retransmissions. More details of R
      and S are given later.
      
      3. Two tables, unacknowledged-message table and received-message table. The
      first table contains the list of all messages that have been sent but not yet
      acknowledged by the receiver, along with the last sending time of the message.
      The second table contains all messages received in the socket.
      
  Thus, if more than one MRP socket is created, one set of socket-thread-tables as above
  will be created for each MRP socket. The threads are killed and the data structures freed
  when the MRP socket is closed. You can assume that the maximum number of MRP
  sockets that can be opened is 25
