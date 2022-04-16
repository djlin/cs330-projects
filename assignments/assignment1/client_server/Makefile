default: all

all: c go

CC=clang

c: server-c client-c

go: client-go server-go

client-c: client-c.c
	$(CC) client-c.c -o client-c

server-c: server-c.c
	$(CC) server-c.c -o server-c

client-go: client-go.go
	go build -o client-go client-go.go

server-go: server-go.go
	go build -o server-go server-go.go

clean:
	rm -f server-c client-c server-go client-go *.o
