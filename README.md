# pw_bindsh

POC for a password protected bindshell.

## Usage

-t      TCP port, on which the bindshell listens
-u      UDP port, for filtering UDP packets on raw socket or bind UDP socket to, when `cap_net_raw` is not effective or `EUID != 0`
-b      Background / daemon task

## Build

**Release:**
`make`

**Debug:** (enables printing and compiles with -ggdb for debug symbols)
`make debug`

## How it works

1. Checks if `EUID == 0` or `cap_net_raw` is effective
2. If true, opens a raw socket and filters UDP packets for a given port
    OR
   If false, opens a udp socket
3. Reads user-provided password from UDP-packet and hashes it (SHA256)
4. Compares hash to hardcoded hash
5. If correct, open new socket on provided TCP-port

## Considerations

This is just a POC and therefore it is easily detectable from within the machine it is running on.
When using raw socket (`cap_net_raw` is effective or `EUID` is 0), it should not be detectable from the outside.

## Todo

[ ] Overwrite argv[0] to kernel process ([kworker...]) 
