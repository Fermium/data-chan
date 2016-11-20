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

* 00h => NONREALTIME
* 01h => OFFSET_REALTIME
* 02h => REALTIME
* 03h => PROGRESSIVE


## Types

##### NONREALTIME

In a NONREALTIME measure time and millis fields are unused, the host will replace their values with its timestamp and millis upon receipt.

##### OFFSET_REALTIME

In a OFFSET_REALTIME measure time and millis refers to the moment the datalogging procedure started.

##### REALTIME

In a REALTIME measure time and millis contains the correct values.

This is useful if you have a long-running system with it's own [RTC](https://en.wikipedia.org/wiki/Real-time_clock) that may have only occasional connection to the host.

##### PROGRESSIVE

In a PROGRESSIVE measure millis:time contains an index number, increased by one each measure.


## Channel

The channel field is used to identify the channel that has generated the measure.

Channels can match measurement channels (such as the one in ADCs or different sensors) or can be considered as endpoints for different functions of your device.

The channel numeration starts from 1 and goes up to 255.

## Unit of Measurement

The Unit of Measurement field is a byte that reveals the used unit of measurement, according to the following list:

* 0 => meter
* 1 => ampere
* 2 => volt
* 3 => coulomb
* 4 => watt
* 5 => kilogram
* 6 => kelvin
* 7 => candela
* 8 => mole
* 9 => hertz
* 10 => radian
* 11 => steradian
* 12 => newton
* 13 => pascal
* 14 => joule
* 15 => farad
* 16 => ohm
* 17 => siemens
* 18 => weber
* 19 => tesla
* 20 => henry
* 21 => lumen
* 22 => lux
* 23 => becquerel
* 24 => gray
* 25 => sievert
* 26 => katal

The measurement should always be in the expected basic unit of measurement of the [SI system](https://en.wikipedia.org/wiki/International_System_of_Units).

Example: If you are measuring Volts, use Volts; not millivolts or kilovolts.

The last five units codes (251, 252, 253, 254, 255) are application-reserved and how they are used depends on the device vendor.

Heck: that means you can use them to do everything you want!
