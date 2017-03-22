# Measure

A MEASURE packet has the data field formed this way:

| bytes  | 0    | 1:9      | 10:46    | 47             | 48      | 49:52     | 53:54     |
|--------|------|----------|----------|----------------|---------|-----------|-----------|
| type   | byte | byte[9]  | float[9] | byte           | byte    | big int   | small int |
| descr  | type | channels | measures | channels count | mu      | timestamp | millis    |

Additional Notes:
- a data type followed by two square parenthesis means an array of that type.
- a big int is large: 32 bits <=> 4 bytes
- a small int is large: 16 bits <=> 2 bytes
- mu means: "Unit of Measurement"

Allowed measure types are:

* 00h => NONREALTIME
* 01h => OFFSET_REALTIME
* 02h => REALTIME
* 03h => PROGRESSIVE


## Types

#### NONREALTIME

In a NONREALTIME measure both time and millis fields are unused, the host will replace their values with its timestamp and millis upon receipt.

#### OFFSET_REALTIME

In an OFFSET_REALTIME measure time and millis refers to the moment the data-logging procedure started.

#### REALTIME

In a REALTIME measure both time and millis contains the correct values.

This is useful if you have a long-running system with it's own [RTC](https://en.wikipedia.org/wiki/Real-time_clock) that may have a infrequent connection to the host.

#### PROGRESSIVE

In a PROGRESSIVE measure millis:time contains an index number, increased by one each measure.


## Channels

The channels field is used to identify the channel that has generated the measure.

Channels can match measurement channels (such as the one in ADCs or different sensors) or can be considered as endpoints for different functions of your device.

The channel numeration starts from 1 and goes up to 255.

There are "channels count" valid channels in a measure packet:
each one is specific to a single measure.

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
