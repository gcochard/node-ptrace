node-ptrace
==================

Enables to trace a process, get and send signals for the traced process.

Instructions to compile:
------------------------
```
gcc -shared -fpic src/childtrace.c  -o libchildtrace.so 

node lib/childtrace.js <process id> <number of retries>
```

Interfaces:
-----------
```
add: Takes process pid and number of retries to attach to this process
detach: Takes process pid and number of retries to detach from this process
getsignal: Takes a process pid and runs infinitely to get the traced process signals.
setsignal: Sends a signal to a given process by pid.
```

Sample Output:
-------------
```
 rchunduru@rchunduru-develop:~$ ps -aef |grep server
 1000     26725 26313 99 18:57 pts/5    00:00:03 ./server


 node examples/childtrace.js 26725 5

 rchunduru@rchunduru-develop:~$ kill -SIGHUP 26725

 arguments are  26725 5
 Starting to attach to the process and monitor it
 start with pid 26725 retries 5
 successfully attached to the process
 Got signal  stopped by signal 1

 Detach from the process
 successfully detached from process
 Signalled the process.
``` 
