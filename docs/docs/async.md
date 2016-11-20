# Asynchronous Requests

An asynchronous request is a request that the Host may, or may not send to the
Device and can be evaluated potentially at any time.

Everything that is not core part in the specification, or user-defined is an
asynchronous request.

__NOTICE THAT:__ only with time that tends to the infinite you have the mathematical
proof of the execution of every asynchronous request sent.

## OUT Packets Structure

The structure of an OUT packet containing an async request is:

<table border="1">
  <tr>
    <td>bytes</td>
    <td>0</td>
    <td>4</td>
    <td>1</td>
    <td>any other</td>
  </tr>
  <tr>
    <td>type</td>
    <td>byte (8bits)</td>
    <td>dword (32bits)</td>
    <td>byte (8bits)</td>
    <td> ??? </td>
  </tr>
  <tr>
    <td>description</td>
    <td>CMD_ASYNC_REQUEST</td>
    <td>Request ID</td>
    <td>Request Code</td>
    <td>Code-Specific</td>
  </tr>
</table>

## IN Packets structure

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

## Implementation

The *Request ID* is a number, usually ascending that is used to recognize a response.

Even if not in ascending or descending order, each async request __MUST__ have
its __unique__ *Request ID*!

## Request Parse & Response Generation

When parsing an async request you have to keep in mind that it is formed by **GENERIC_REPORT_SIZE -6** bytes, whereas an async response can be made up of
**GENERIC_REPORT_SIZE -5** bytes.

Those size differences are given by the asymmetry of the two packets:
only the OUT packet contains the *Request Code*, which is one byte large.
