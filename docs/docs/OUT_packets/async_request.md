# Asynchronous Requests
An asynchronous request is a request sent by the host to the Device that can be evaluated potentially at any time.

Every request, excluding the requests of the core protocol, is an asynchronous request.

__NOTICE:__ You would need infinite time to have the mathematical proof of the execution of an asynchronous request.

## Packet Structure

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

## Request ID

The *Request ID* is a number (that should be increasing) used to identify the response.

Even if the *Request IDs* are not ordered, they must be unique to each request.

