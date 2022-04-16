/*****************************************************************************
 * client-go.go                                                                 
 * Name: Tachun Lin
 *****************************************************************************/

package main

import (
	"fmt"
 //  "io"
	"os"
	"log"
	"net"
	"bufio"
)
 
 const SEND_BUFFER_SIZE = 2048
 
 /* TODO: client()
  * Open socket and send message from stdin.
 */
 func client(server_ip string, server_port string) {
   	 conn, err := net.Dial("tcp", server_ip + ":"+ server_port)
   	 if err != nil {
     	 fmt.Println("Error connecting:", err.Error())
     	 os.Exit(1)
   	 }

   	 defer conn.Close()

   	 var (
     	 buf = make([]byte, SEND_BUFFER_SIZE)
     	 r   = bufio.NewReader(os.Stdin)
     	 w   = bufio.NewWriter(conn)
   	 )

   	 for {
     	 n, _ := r.Read(buf)
     	 if n <= 0 {break}
     	 _, err  := w.Write(buf[:n])
     	 if err != nil {
       		 // handle the error
       		 panic(err)
     	 }
     	 w.Flush()
   	 }
 }
 
 // Main parses command-line arguments and calls client function
 func main() {
   	 if len(os.Args) != 3 {
     	 log.Fatal("Usage: ./client-go [server IP] [server port] < [message file]")
   	 }
   	 server_ip := os.Args[1]
   	 server_port := os.Args[2]
   	 client(server_ip, server_port)
 }
