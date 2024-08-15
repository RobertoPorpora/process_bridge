# Process Bridge

**Process Bridge** is a library designed to facilitate the creation and management of long-lived child processes, enabling bidirectional communication through pipes.  
It provides a simple interface for spawning, despawning, sending and receiving data between parent and child processes, streamlining the integration and control of parallel processes.  
Ideal for applications requiring multiple persistent and collaborative processes.


## Implementation status

> This repository is a bit messy...  
> The plan is to have multiple repositories:
> - node_process_bridge
> - c_process_bridge
> - python_process_bridge
> - ...
> 
> And then a testing repository that has all of the above as submodules.  
> At the moment it's all mixed up...  

This is the state of implementation

V = Done.  
X = Not Done.  

| Language | Parent library | Parent test | Child library | Child test |
| -------- | -------------- | ----------- | ------------- | ---------- |
| Node.js  | V              | V           | V             | V          |
| C / C++  | X              | X           | V             | V          |
| Python   | X              | X           | X             | X          |
| other    | X              | X           | X             | X          |

## Common high level API

### Definitions

Nothing to say here

### Parent side functions

> This is not exactly high level, this is derived from the C header file.  
> In Node.js for example it's useful to have an event-based library.  
> And then for every language it could be useful to explore different techniques like multi-threading, sync/blocking and async/non-blocking.  

- **STATUS CHECK**
    - **get_status**: gets the status of a child process.
        - input: child process handle.
        - output: the status of the child process.
            - OK
            - NOT_SPAWNED
            - COMPLETED
            - TERMINATED
            - GENERIC_ERROR
    - **get_error**: gets the last error of a child process.
        - input: child process handle.
        - output: a string describing the error.
- **CHILD PROCESS LIFE MANAGEMENT**
    - **spawn**: spawns a new child process with the given CLI command.
        - input: command string.
        - input: child process handle.
        - output: function execution status.
    - **despawn**: terminates the child process.
        - input: child process handle.
        - output: function execution status.
    - **wait**: waits for a child process to finish execution.
        - input: child process handle.
        - output: function execution status.
    - **get_return_code**: gets the return code of a terminated or a completed child process.
        - input: child process handle.
        - output: the presence of a return code.
        - output: the return code.
- **CHILD PROCESS COMMUNICATION**
    - **p2c_send**: send a message to child process stdin:
        - input: message string.
        - input: a child process handle.
        - output: function execution status.
    - **p4c_receive**: receive a message from the child process stdout:
        - input: a child process handle.
        - output: the message received.
        - output: function execution status.
    - **p4c_receive_err**: receive a message from the child process stderr:
        - input: a child process handle.
        - output: the message received.
        - output: function execution status.



### Child side functions

- **STATUS CHECK**
    - **get_parent_bridge_status**: gets the status of the parent process bridge.
        - input: nothing.
        - output: the status of the parent process bridge.
            - OK
            - GENERIC_ERROR
    - **get_parent_bridge_error**: gets the last error of the parent process bridge.
        - input: nothing.
        - output: a string describing the error.
- **PARENT PROCESS COMMUNICATION**
    - **parent_bridge_init**: sets everything up for communication with parent process.
        - input: nothing.
        - output: function execution status.
    - **c2p_send**: send a message to parent through child's stdout:
        - input: message string.
        - output: function execution status.
    - **c2p_send_err**: send a message to parent through child's stderr:
        - input: message string.
        - output: function execution status.
    - **c4p_receive**: receive a message from the parent through child's stdin:
        - input: nothing.
        - output: the message received.
        - output: function execution status.


## Test pattern

- P: Parent process
- C: Child process

every message from  C to P is meant to be sent both via stdout and stderr.

### Test 1

P -> C : (spawn)  
P -> C : 'p1' (C saves as [Crx0])  
P -> C : 'p2' (C saves as [Crx1])  
P -> C : 'p3' (C saves as [Crx2])  

P <- C : 'c1 [Crx0] [Crx1] [Crx2]' (P saves as [Prx0])  
P <- C : 'c2' (P saves as [Prx1])  
P <- C : 'c3' (P saves as [Prx2])  

(C waits 1 second and then returns '12')

P does PB_wait(C) and saves its return code as [Crc]

P checks:
- [Prx0] == "c1 p1 p2 p3"
- [Prx1] == "c2"
- [Prx2] == "c3"
- [Crc] == "12"


### Test 2

P -> C : (spawn)  
P -> C : 'p1' (C saves as [Crx0])  
P -> C : 'p2' (C saves as [Crx1])  
P -> C : 'p3' (C saves as [Crx2])  

P <- C : 'c1 [Crx0] [Crx1] [Crx2]' (P saves as [Prx0])  
P <- C : 'c2' (P saves as [Prx1])  
P <- C : 'c3' (P saves as [Prx2])  

(C waits 1 second and then returns '12')

P does PB_despawn(C) before it exits and saves its return code as [Crc]

P checks:
- [Prx0] == "c1 p1 p2 p3"
- [Prx1] == "c2"
- [Prx2] == "c3"
- [Crc] == "null"
