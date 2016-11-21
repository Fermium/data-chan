# Asynchronous Requests

An asynchronous request is a request sent by the host to the Device that can be evaluated potentially at any time.

Every request if not otherwise noted in the specifications (or differently defined by the user), is an asynchronous request.

__NOTICE:__ You would need infinite time to have the mathematical proof of the execution of an asynchronous request.

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

## Request ID

The *Request ID* is a number (that should to be increasing) used to identify the response.

Even if the *Request IDs* are not ordered, they must be unique to each request.

## Request Parsing & Response Generation

When parsing an async request you have to keep in mind that it is formed by **GENERIC_REPORT_SIZE -6** bytes, whereas an async response can be made up of **GENERIC_REPORT_SIZE -5** bytes.

Those size differences are given by the asymmetry in the lenght of the two packets: only the OUT packet contains the *Request Code*, which is one byte long.
