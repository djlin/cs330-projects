/*****************************************************************************
 * server-go.go                                                                 
 * Name: Tachun Lin
 *****************************************************************************/

package main

import (
	"fmt"
//	"io"
	"os"
	"log"
	"net"
	"bufio"
)

const RECV_BUFFER_SIZE = 2048

/* TODO: server()
 * Open socket and wait for client to connect
 * Print received message to stdout
*/
func server(server_port string) {
  	ln, err := net.Listen("tcp", ":"+server_port)
  	if err != nil {
    	// handle error
    	fmt.Println("Error listening:", err.Error())
    	os.Exit(1)
  	}
  	defer ln.Close()

  	for {
    	conn, err := ln.Accept()
    	if err != nil {
      		// handle error
      		fmt.Println("Error accepting:", err.Error())
      		return
    	}

    	defer conn.Close()

    	var (
      		buf = make([]byte, RECV_BUFFER_SIZE)
      		r   = bufio.NewReader(conn)
    	)

    	for {
      		n, err := r.Read(buf)
      		if n < 0 {break}
      		if err != nil {
				//panic(err)
    			conn.Close()
    			break
      		}
      		data := string(buf[:n])
      		fmt.Print(data)
    	}
  	}
}


// Main parses command-line arguments and calls server function
func main() {
  	if len(os.Args) != 2 {
    	log.Fatal("Usage: ./server-go [server port]")
  	}
  	server_port := os.Args[1]
  	server(server_port)
}
