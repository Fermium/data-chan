# Protocol

The protocol can be customized starting from the very USB basics.

The protocol minimum entity is a packet.

The protocol makes __HUGE__ usage of the __GENERIC_REPORT_SIZE__ macro,
that macro defines the number of bytes in each USB packet.

The data-chan protocol uses *little-endian* then serializing data larger than one byte.


## Packets Structure

Both IN and OUT packets are formed this way:

<table border="1">
	<tr>
		<td>byte</td>
		<td>first</td>
		<td>following</td>
		<td>last</td>
	</tr>
	<tr>
		<td>desc</td>
		<td>type</td>
		<td>data</td>
		<td>CRC-8</td>
	</tr>
</table>

The first byte contains the packet type: how data is used depends on the very first byte.

Each packet ends with the last byte used for the CRC code (standard 8 bits).

From the host perspective there are two types of packets IN packets and OUT packets.


## IN packets

An incoming packet can be flagged as either one of these types:
<ul>
	<li>00h => NONE</li>
	<li>01h => CMD_RESPONSE</li>
	<li>03h => MEASURE</li>
</ul>
	
When the packets is flagged as NONE it must be ignored.

By default every packet will be flagged as NONE, thus avoiding sending data to the Operating System,
which can't decode packets.

When the device is active every type of packet will be used.

A MEASURE packet has the data field formed this way:

<table border="1">
	<tr>
		<td>bytes</td>
		<td>0</td>
		<td>1</td>
		<td>2 to 6</td>
		<td>7</td>
		<td>8 to 12</td>
		<td>13 to 14</td>
	</tr>
	<tr>
		<td>descr</td>
		<td>type</td>
		<td>channel</td>
		<td>value</td>
		<td>mu</td>
		<td>timestamp</td>
		<td>millis</td>
	</tr>
</table>
