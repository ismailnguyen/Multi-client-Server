# Multi-client-Server

## Install

Change directory until to be inside directory where client.c and server.c are stored. Execute makefile.

```
make
```

## Usage

### Server

Launch server first

```
./server
```

Then type port when it will be asked.

Or, specify it on command line:

```
./server 3434
```

### Client

Then, launch many of clients you want

```
./client
```

Then type IP address and port of targeted server when it will be asked.

Or, specify them on command line:

```
./client 127.0.0.1 3434
```

#### Commands

Through client, you will be able to run some commands:

Local:
- ls (Return list of files and sub-directories on current directory)
- pwd (Return current directory)
- cd (Change directory to given path)
- rm (Remove file from given path)

Distant:
- rls (Show remote directory contents)
- rcd (Change remote directory path)
- rpwd (Return remote directory path)
- upld (Upload file)
- downl (Download file)
