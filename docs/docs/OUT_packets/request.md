# Synchronous Requests

A synchronous request is sent from the Host to the client, aka the Device and
**must** be evaluated immediately after its receipt.

__NOTICE THAT:__ core parts of the specification is a synchronous request as you
can be mathematically certain of the evaluation of every request sent and correctly
received!

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

Changing the request code the required action changes accordingly, read the
following sections for the complete list of synchronous commands.

## Enable Transmission

The ENABLE_TRANSMISSION code, defined as 01h, enables the OUT packets transmission.

Typically this is the first command sent to the Device.

When using ENABLE_TRANSMISSION following bytes are meaningless, and thus ignored.

## Disable Transmission

The DISABLE_TRANSMISSION code, defined as 02h, disables the OUT packets transmission.

Typically this is the last command sent to the Device.

When using DISABLE_TRANSMISSION following bytes are meaningless, and thus ignored.
