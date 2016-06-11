# Multi-client-Server

## Install

Change directory until to be inside directory where client.c and server.c are stored. Execute makefile.

```
make
```

## Usage

Launch server first

```
./server
```

Then type port when it will be asked.

Or, specify it on command line:

```
./server 3434
```

Then, launch many of clients you want

```
./client
```

Then type IP address and port of targeted server when it will be asked.

Or, specify them on command line:

```
./client 127.0.0.1 3434
```
