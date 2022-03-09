# Assignment 2: Web Client and Server

Through this assignment, you will learn how a web client (browser) and a web server
talk to each other. You will modify the client and server programs from the
textbook, so the client program acts as a browser and sends HTTP request to
a real web server and receives responses from the web server. Similarly, your server
program accepts the queries from a real web browser, processes the HTTP request, and
determines if the request is a valid one or not. If it is a valid request, it responds
to the request accordingly.

Also, we will learn how to read the RFC (Request for Comments) which provides the
specification of a protocol. 

* [Reference: RFC 2616 - HTTP/1.1](https://datatracker.ietf.org/doc/html/rfc2616/)

This is not a group assignment. **You are not allowed to copy or look at code
from other students.** However, you are welcome to discuss the assignments with
other students without sharing code.

## How to Read the HTTP Protocol

* According to [Section 5](https://datatracker.ietf.org/doc/html/rfc2616/#section-5) 
  in RFC 2616, the HTTP client will form a request in the following format.

      Request = Request-Line        ; Section 5.1
          *(( general-header        ; Section 4.5
            | request-header        ; Section 5.3
            | entity-header ) CRLF) ; Section 7.1
            CRLF
            [ message-body ]        ; Section 4.3

* How to *interpret* and understand the meaning of the request?

* An "HTTP request" is composed of multiple lines

1. The Request-Line, which is composed of

      Request-Line   = Method SP Request-URI SP HTTP-Version CRLF

  Here, (1) "SP" means a space, (2) [CRLF](https://developer.mozilla.org/en-US/docs/Glossary/CRLF) 
  means carriage return and line feed, and (3) "Method" indicates the action taken on the URL.

  Regarding Request-URI (Uniform Resource Identifier), Section 5.1.2 provides the 
  detail information that it is

      Request-URI    = "*" | absoluteURI | abs_path | authority

  The meaning of | is that the Request-URL can be **any one** of the four options -
  "*", absoluteURI, abs_path, or authority.

  For simplicity, the client in this project will only handle 
  [abs_path](https://datatracker.ietf.org/doc/html/rfc2616/#section-3.2.1), 
  which provides the relative **path** of a page with respect to the server **root** 
  on the server.

  For example, given the Bradley Computer Science Department webpage,
  [https://www.bradley.edu/academic/departments/csis/](https://www.bradley.edu/academic/departments/csis/)
  the **abs_path** here is 

`/academic/departments/csis/`

  with respect to the Bradley web service at

`https://www.bradley.edu`

2. To complete an HTTP request, one or more of the 
   ( genera-header | request-header | entity-header ) may come after the Request-Line

        Request = Request-Line        ; Section 5.1
            *(( general-header        ; Section 4.5
              | request-header        ; Section 5.3
              | entity-header ) CRLF) ; Section 7.1
              CRLF
              [ message-body ]        ; Section 4.3

    Note: If the Request has one or more of the three options above, each header line
    is ended with a CRLF

    For example, the following Request is composed of

        GET /pub/WWW/TheProject.html HTTP/1.1 CRLF
        Host: www.w3.org CRLF
        CRLF

    Note: Here the *CRLF* shall be replaced by the characters mentioned above
    If we decompose the HTTP request, and we get the following

        Method: GET
        abs_path URI: /pub/WWW/TheProject.html
        HTTP-Version: HTTP/1.1
        request-header: Host: www.w3.org

3. Once the HTTP request is received on the server side, the server analyzes the
   request and check if it is a valid one

   In terms of valid, the server looks for

   (1) If the request is composed of the items mentioned above

   (2) If the request does contain the correct information, check if the
   abs_path provides a valid path to either a sub-directory under the server root
   or a valid path to a file 

   For example, assuming that you run the server program residing under

`/Users/tachun/projects/SP22-CS330-Projects/assignments/assignment2/client_server`

    the default abs_path

        /

    means to access the **index.html** or **index.htm** file under the same folder

`/Users/tachun/projects/SP22-CS330-Projects/assignments/assignment2/client_server/index.html`

  
    If there is a sub-directory called **images** under the server root, and hi.jpg 
    sits inside this directory,

`/Users/tachun/projects/SP22-CS330-Projects/assignments/assignment2/client_server/images/hi.jpg`


    the web client may send a request with abs_path

`/images/hi.jpg`

    to get the content of the image

4. If the HTTP request is valid, and the (sub-)directory and/or file does exist,
   the server will read the content of the file, send it back to the client, terminate
   the connect with the client, and wait for the other client to connect

## Client specification

* The client should take one command-line argument: the hostname or IP address of the
  server, followed by the abs_path

  For example, the following

  `./client-c daemon.bradley.edu/research.html`

  provides abs_path `/research.html` to a file `research.html` under the web root
  of the web server `daemon.bradley.edu`

* The default abs_path is

          /
  if no other subpath/file name is provided

  For example, the following should return the **default** page configured on
  the server

  `./client-c daemon.bradley.edu`

  or

  `./client-c daemon.bradley.edu/`

* The client should use port 80 to connect to the server
* The client will form an HTTP request based on the hostname (or IP address), default
  port number 80, and abs_path
* After sending out the HTTP request to the server, the client will wait for the
  web server to respond
* Each client should gracefully handle error values potentially returned by
  socket programming library functions.

## Server specification

* The server program should listen on a socket, wait for a client to connect,
  receive the HTTP request from the client, send the content of the page back
  to the client (when the request is valid and the file does exist), and then
  wait for the next client indefinitely.
* Each server should take one command-line argument: the default file name with
  the web content

  For example, if the argument is `hello.html` and the HTTP request has abs_path

          /

  the server will get the content from `hello.html` file and send its content
  to the client

* Each server should accept and process client communications in an infinite
  loop, allowing multiple clients to send messages to the same server. The
  server should only exit in response to an external signal (e.g. SIGINT from
  pressing `ctrl-c`).
* Each server should maintain a short (5-10) client queue and handle multiple
  client connection attempts sequentially. In real applications, a TCP server
  would fork a new process to handle each client connection concurrently, but
  that is **not necessary** for this assignment.
* Each server should gracefully handle error values potentially returned by
  socket programming library functions (see specifics for each language below).
  Errors related to handling client connections should not cause the server to
  exit after handling the error; all others should.


## What to Submit for the Assignment

You will need to add socket programming and I/O code in the files `client-c.c`
and `server-c.c`, similar to the prior assignment

For error handling, you can call `perror` for socket programming functions that
set the global variable `errno` (Beej's Guide will tell you which do). For
those that don't, simply print a message to standard error.

You should build your solution by running `make` in the
`assignment2/client_server` directory. Your code *must* build using the
provided Makefile. Depending on your system, you may habe to change  

`CC=gcc` 

to 

`CC=gcc-9`

in the Makefile

The server should be run as 

`./server-c [(path)/file name of the default HTML page]> [output file]`. 

The client should be run as

`./client-c absoluteURI`.

## Testing

Three pairs of testing will be carried out.

* Your web client to a real web server
* A real web browser to your web server
* Your web client to your web server

The server can be run in the background (append a `&`
to the command) or in a separate SSH window. You can kill a background
server with the command `fg` to bring it to the foreground then `ctrl-c`.


### Submission and grading

Submit the assignment by committing and pushing your modified client and server
files to GitHub.

Double check the specifications above and perform your own tests before
submitting.

Code that does not compile is graded harshly; if you want partial credits,
*make sure your file compiles!*


### NOTES

* Do it immediately
* NEVER GIVE UP
* Ask questions any time
