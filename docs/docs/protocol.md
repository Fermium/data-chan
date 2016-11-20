# Protocol

The protocol can be customized starting from the very USB basics, it's minimum entity is a packet.

The protocol itself makes **huge** usage of the __GENERIC_REPORT_SIZE__ macro which defines the number of bytes, minus one, in each USB packet.

That means VENDOR_IO_EPSIZE = GENERIC_REPORT_SIZE +1, where the last byte
(the +1) is used for the CRC-8!

The Data-chan protocol uses *little-endian* while serializing data larger than one byte.


## Packets Structure

Both IN and OUT packets are formed this way:

<table>
	<tr>
		<td>Position</td>
		<td>first byte</td>
		<td>following bytes</td>
		<td>last byte</td>
	</tr>
	<tr>
		<td>Description</td>
    <td>type</td>
		<td>data</td>
		<td>CRC-8</td>
	</tr>
	<tr>
		<td>Offset</td>
    <td>0</td>
		<td>1</td>
		<td>GENERIC_REPORT_SIZE</td>
	</tr>
</table>

The first bytes is the one that controls how the data is used.

From the host perspective there are two types of packets:

* IN packets
* OUT packets.


#### IN packets

An incoming packet can be flagged as either one of these types:

* 00h => NONE
* 02h => CMD_ASYNC_RESPONSE
* 04h => MEASURE


Packets flagged with NONE must be ignored, even if theirs CRC is valid.

By default every packet will be flagged as NONE, thus avoiding sending data to the operating system of the host which can't decode packets by itself.

When the device is active every type of packet will be used.

A MEASURE description can be found [here](IN_packets/Measure.md),
whereas a CMD_RESPONSE description can be read [here]().
