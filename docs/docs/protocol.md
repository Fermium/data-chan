# Protocol

The protocol can be customized starting from the very USB basics, it's minimun entity is a packet.

The protocol itself makes **huge** usage of the *GENERIC_REPORT_SIZE* macro which defines the number of bytes in each USB packet.

The Data-chan protocol uses *little-endian* before serializing data larger than one byte.


## Packets Structure

Both IN and OUT packets are formed this way:

| first byte | following bytes | last byte|
| --- | --- | --- |
| type | data | CRC-8 |

The first bytes is the one that controls how the data is used.

From the host perspective there are two types of packets:

* IN packets
* OUT packets.


#### IN packets

An incoming packet can be flagged as either one of these types:

* 00h => NONE
* 01h => CMD_RESPONSE
* 03h => MEASURE

	
Packets flagged with NONE must be ignored.

By default every packet will be flagged as NONE, thus avoiding sending data to the operating system of the host which can't decode packets by itself.

When the device is active every type of packet will be used.

A MEASURE description can be found [here](IN_packets/Measure.md), whereas a CMD_RESPONSE description can be read [here]().