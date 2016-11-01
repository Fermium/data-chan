# Measure

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
		<td>type (8bits)</td>
		<td>byte (8bits)</td>
		<td>byte (8bits)</td>
		<td>float (32bits)</td>
		<td>byte (8bits)</td>
		<td>big int (32bits)</td>
		<td>small int (16bits)</td>
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

Allowed measure types are:
<ul>
	<li>00h => NONREALTIME</li>
	<li>01h => OFFSET_REALTIME</li>
	<li>02h => REALTIME</li>
	<li>03h => PROGRESSIVE</li>
</ul>


## Type

In a NONREALTIME measure time and millis fields are unused, the host will replace their values
with the timestamp and millis upon receipt.

In a OFFSET_REALTIME measure time and millis both refers to the moment of measures beginning.

In a REALTIME measure time and millis contains the correct values.

In a PROGRESSIVE measure millis:time contains a progressive number used to indexing the measure.


## Channel

The channel field is used to identify the channel that has generated the measure.

The channel field starts from 1, this means that an instrument can have 255 possible channels:
from 1 to 255.

## Unit of Measurement

The Unit of Measurement field is a byte that reveils the used unit of measurement, according to the following list:
<ul>
	<li>0 => metre</li>
	<li>1 => ampere</li>
	<li>2 => volt</li>
	<li>3 => coulomb</li>
	<li>4 => watt</li>
	<li>5 => kilogram</li>
	<li>6 => kelvin</li>
	<li>7 => candela</li>
	<li>8 => mole</li>
	<li>9 => hertz</li>
	<li>10 => radian</li>
	<li>11 => steradian</li>
	<li>12 => newton</li>
	<li>13 => pascal</li>
	<li>14 => joule</li>
	<li>15 => farad</li>
	<li>16 => ohm</li>
	<li>17 => siemens</li>
	<li>18 => weber</li>
	<li>19 => tesla</li>
	<li>20 => henry</li>
	<li>21 => lumen</li>
	<li>22 => lux</li>
	<li>23 => becquerel</li>
	<li>24 => gray</li>
	<li>25 => sievert</li>
	<li>26 => katal</li>
</ul>

The last five units codes (251, 252, 253, 254, 255) are application-reserved and how they are used depends on the device vendor.