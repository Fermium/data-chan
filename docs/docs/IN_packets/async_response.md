# Asynchronous Requests
This chapter completes the [async request](../OUT_packets/async_request.md) one.

An async respose maintains the ID of its corresponding request.

## Packet structure
The structure of an IN packet containing an async response is:

<table border="1">
  <tr>
    <td>bytes</td>
    <td>0</td>
    <td>4</td>
    <td>any other</td>
  </tr>
  <tr>
    <td>type</td>
    <td>byte (8bits)</td>
    <td>dword (32bits)</td>
    <td> ??? </td>
  </tr>
  <tr>
    <td>description</td>
    <td>CMD_ASYNC_RESPONSE</td>
    <td>Request ID</td>
    <td>Response</td>
  </tr>
</table>

## Request Parsing & Response Generation

When parsing an async request you have to keep in mind that it is formed by **GENERIC_REPORT_SIZE -6** bytes, whereas an async response can be made up of **GENERIC_REPORT_SIZE -5** bytes.

Those size differences are given by the asymmetry in the length of the two packets: only the OUT packet contains the *Request Code*, which is one byte long.
