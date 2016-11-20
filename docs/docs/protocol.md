# Protocol

The protocol can be customized starting from the very USB basics, it's minimum entity is a packet.

The protocol itself makes **huge** usage of the __GENERIC_REPORT_SIZE__ macro which defines the number of bytes, minus one, in each USB packet.

That means VENDOR_IO_EPSIZE = GENERIC_REPORT_SIZE +1, where the last byte
(the +1) is used for the CRC-8!

The Data-chan protocol uses *little-endian* while serializing data larger than one byte.

## Packets Structure

Both IN and OUT packets are formed this way:

<table border="1">
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


#### IN Packets

An incoming packet can be flagged as either one of these types:

* 00h => NONE
* 02h => CMD_ASYNC_RESPONSE
* 04h => MEASURE


Packets flagged with NONE must be ignored, even if theirs CRC is valid.

By default every packet will be flagged as NONE, thus avoiding sending data to the operating system of the host which can't decode packets by itself.

When the device is active every type of packet will be used.

A MEASURE description can be found [here](IN_packets/measure.md),

#### OUT Packets

An outgoing packet can be:

* 00h => NONE
* 01h => CMD_REQUEST
* 03h => CMD_ASYNC_REQUEST

Packets flagged with NONE must be ignored, even if theirs CRC is valid.

A CMD_REQUEST description can be found [here](OUT_packets/request.md).

#### Async requests

To understand how CMD_ASYNC_REQUEST and CMD_ASYNC_RESPONSE work you should read
the [Async](async.md) chapter.

#### Tips & Tricks

Any outgoing packet has the last bit of the first byte set to 1, whereas any
incoming packet has the last bit of the first byte set to zero.

A NONE packet definition does exists in both: IN and OUT packets.

__NOTICE THAT:__ This fact can be used to detect bad packets!
