light-net.
> GNU GPL(General Public License) license.

## Network Thread Policy.
* light-net using spin-lock. [boost]

## Network Thread Category
1. IOCP Session Thread. [on_receieve, receive, on_reuse, on_send, on_connect, on_connect_failed by connector]
2. Accept Thread. [only on_connect, on_connect_failed by listener]

## Common Socket Events
[receieve : without lock]

* receive : iocp thread
* on_receive : iocp thread

[close : without lock. using atomic operation]

* close : user thread, iocp thread.
* on_close : iocp thread.

[reuse : without lock. using atomic operation]

* reuse : user thread
* on_reuse : iocp thread, another_thread.

[reuse : without lock. using atomic operation]

* disconnect : user thread, iocp thread
* on_disconnect : iocp thread.

[send : with lock]

* send : user thread
* on_send : iocp thread

## Lisneter's Socket Events
[connect : without lock]

* connect : iocp thread
* on_connected : iocp thread
* on_connect_failed : iocp thread

## Connector's Socket Events
[connect : without lock, using atomic operation]

* connect : user thread
* on_connected : iocp thread
* on_connect_failed : iocp thread
 
### Link
[blog](http://elky84.github.io)