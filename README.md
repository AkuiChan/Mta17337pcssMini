# PCSS Mini-Project
This is the repository for the group MTA17337's mini-project in Programming for Complex Software Systems.

## Getting Started
This program is a multichat program, with multiple clients being able to connect to the same server.

To download the client and server
```
Step 1: On GitHub, click Release tab
Step 2: Download Source code (zip) and unpack
```

To have different clients on more than one computer, you must first change the following in visual studio:
```
Step 1: Open Command Prompt on the computer where the server will be run
Step 2: Run the following command: ipconfig
Step 3: Find the IPv4 address
Step 4: Replace 127.0.0.1 with the IPv4 address in the server in line 41
Step 5: Replace 127.0.0.1 with the IPv4 address in the client in line 47
```
Note that you might have to find the IPv4 address a different way if you're not using Windows.

Open the server and wait for the message "Server is online" before trying to open the clients.

## Running Server and Client
To run the server:
```
Step 1: Go to folder Server/Server/x64/Debug
Step 2: Open the server application
```
To run the client:
```
Step 1: Go to folder Client/x64/Debug
Step 2: Open the client application
```

## Built With
* Visual Studio in C++

## Authors
* **Bikkittannguaq Geisler** - bgeisl16
* **Michelle Fly** - AkuiChan
* **Nikolaj Løvig** - Zekrom9
* **Kasper Schøn Henriksen** - Schonsteren
* **Katrine Sigmer** - KatrineTS
