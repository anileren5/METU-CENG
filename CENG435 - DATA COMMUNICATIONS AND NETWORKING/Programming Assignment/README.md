
# CENG 435 - Programming Assignment

## Fine-tuning
Normally, we planned to explain this part in the report but since resubmittion is not enabled, we will discuss the effect of the fine tuning parameters here.
We have two parameters that have effect on the performance of our reliable object transfer protocol over UDP. 

NUMBER_OF_TRIAL_FOR_ADRESS_TRANSFER: This is the number of maximum attempts for client to initiate transfer. 1 is enough for the nearly perfect network state and packet duplication. For
all the other cases, 2 is enough actually, but to be 100% sure we decided it to be 1000.

WINDOW_SIZE = This is the most important parameter of our code. It does not have a linear relationship with the performance. Therefore, we found the most optimum values by trail and error.
If the WINDOW_SIZE becomes too large, then the server spends too much time for step in which it is trying collect initial acknowledgements. If the WINDOW_SIZE becomes too small, the core part of our implementation which is eliminating missing acks does not show its impact because it can only recover small amount of missing acks since the window size is already too small.
We found that 210 is the best value for the delay case, and 30 is the best value for the rest of the cases.

## How to execute TCP codes

After you create the Docker containers as specified in [the official homework repository](https://github.com/cengwins/ceng435). Since we hardcoded the ip address of the server, use the direct method not the one with the compose plugin in order to get the same ip address as the one we hardcoded. If you still encounter any problems like "Cannot assign requested address [Errno 99]", you can adjust the HOST variable in tcp/client/client.py and tcp/server/server.py as well as SERVER_ADDRESS variable in udp/client.py and udp/server.py acording to the ip address of your containers.

In the server container, go to /app/tcp/server and run the following command in order to run the TCP Server:

```bash
python3 server.py
```

Then, in the client container, go to /app/udp/client and run the following command in order to run the TCP Client:

```bash
python3 client.py
```

After these commands, server will start to tranfer objects to client and client will print the elapsed the once the transfer is completed.


## How to execute UDP codes

After you create the Docker containers as specified in [the official homework repository](https://github.com/cengwins/ceng435).

In the server container, go to /app/udp and run the following command in order to run the TCP Server:

```bash
python3 server.py <WINDOW_SIZE> <NUMBER_OF_TRIAL_FOR_ADRESS_TRANSFER>
```

Then, in the client container, go to /app/udp and run the following command in order to run the TCP Client:

```bash
python3 client.py <NUMBER_OF_TRIAL_FOR_ADRESS_TRANSFER>
```

After these commands, server will start to tranfer objects to client and client will print the elapsed the once the transfer is completed.

The command line parameters have to be choses according to the table below:

| Experiment        | WINDOW_SIZE |  NUMBER_OF_TRIAL_FOR_ADRESS_TRANSFER  |
| ----------------- | ----------- | ------------------------------------- |
| No rules          |     30      |                  1                    |
| Packet loss       |     30      |                1000                   |
| Packet duplication|     30      |                  1                    |
| Packet corruption |     30      |                1000                   |
| Packet delay      |     210     |                1000                   |


## Folder structure

<pre>
|-----------------------------|------------------------------------------------------------------------------------------------|
| File / Folder               | Description                                                                                    |
|-----------------------------|------------------------------------------------------------------------------------------------|
| code                        | Mounted to /app path of the containers                                                         |
|   ├── tcp                   | TCP codes                                                                                      |
|   |   ├── client            | TCP Client Codes                                                                               |
|   |   |   ├── objects       | Objects coming from the server are saved here                                                  |
|   |   |   ├── client.py     | TCP Client code to be executed                                                                 |
|   |   |   └── diff_check.sh | A bash script used by client.py to check if the objects are received correctly                 |
|   |   └── server            | TCP Server Codes                                                                               |
|   |       ├── objects       | server.py reads objects from here; it already includes objects generated by generateobjects.sh |
|   |       └── server.py     | TCP Server code to be executed                                                                 |
|   └── udp                   | UDP Codes                                                                                      |
|       ├── client_objects    | Objects coming from the server are saved here                                                  |
|       ├── server_objects    | server.py reads objects from here; it already includes objects generated by generateobjects.sh |
|       ├── client.py         | UDP Client code to be executed                                                                 |
|       ├── common.py         | Common utility functions used by client.py and server.py                                       |
|       ├── diff_check.sh     | A bash script used by client.py to check if the objects are received correctly                 |
|       └── server.py         | TCP Server code to be executed                                                                 |
| authors.txt                 | Names and ids of the authors                                                                   |
| Dockerfile                  | Dockerfile for creating containers                                                             |
| generateobjects.sh          | A bash script for generating objects                                                           |
| README.md                   | It includes all the necessary instructions to run codes                                        |
|-----------------------------|------------------------------------------------------------------------------------------------|
<pre>
