# Synchronous Requests

A synchronous request is sent from the Host to the Client (the Device) and **must** be evaluated immediately after its reception.

__BEWARE:__ The core parts of the specification is implemented with synchronous requests, as there is mathematical certainty of the evaluation of every request sent and correctly received.

## Packets structure

The structure of a synchronous request packet is:

<table border="1">
  <tr>
    <td>bytes</td>
    <td>0</td>
    <td>1</td>
    <td>any other</td>
  </tr>
  <tr>
    <td>type</td>
    <td>byte (8bits)</td>
    <td>byte (8bits)</td>
    <td> ??? </td>
  </tr>
  <tr>
    <td>description</td>
    <td>CMD_REQUEST</td>
    <td>request code</td>
    <td> ??? </td>
  </tr>
</table>

Changing the request code the required action changes accordingly, read the following section for the complete list of synchronous commands.

## Synchronous commands

#### Enable Transmission

The ENABLE_TRANSMISSION code, defined as 01h, enables the OUT packets transmission.

Typically this is the first command sent to the Device.

When using ENABLE_TRANSMISSION the bytes that follow are meaningless, and thus will be ignored.

#### Disable Transmission

The DISABLE_TRANSMISSION code, defined as 02h, disables the OUT packets transmission.

Typically this is the last command sent to the Device.

When using DISABLE_TRANSMISSION the bytes that follows are meaningless, and thus ignored.
