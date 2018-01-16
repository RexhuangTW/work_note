---
title: Multicast 實作
categories: 
- C
tags: C socket network
---

kernel space和user space的溝通方法有許多種，其中一種：netlink就可以支援multicast。
kernel會有許多event會發送資料給user space中有去listen的process。

這個機制可以一次將資料發送給所有有需要的process，以下將實作使IPC也可以做到multicast。

## Example
以下是Server端及Client端的code

### Server -> send
{% highlight C linenos %}
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>


#define HELLO_PORT 7788
#define HELLO_GROUP "225.0.0.1"
#define LOCAL	"127.0.0.1"

main(int argc, char *argv[])
{
	struct sockaddr_in addr = {0};
	struct in_addr ipaddr = {0};
     	int fd, cnt;
     	char *message="Hello, I am server!";

     	/* create what looks like an ordinary UDP socket */
     	if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
      	perror("socket");
      	exit(1);
     	}

     	/* set up destination address */
     	addr.sin_family=AF_INET;
     	addr.sin_port=htons(HELLO_PORT);
	inet_pton(AF_INET, HELLO_GROUP, &addr.sin_addr.s_addr);
	inet_pton(AF_INET, LOCAL, &ipaddr.s_addr);

	/* for IPC */
	if(setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, (char*)&ipaddr, sizeof(ipaddr)) != 0)
	{
    		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

     	/* now just sendto() our destination! */
     	while (1) 
	{
      		if (sendto(fd, message, sizeof(message), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) 
		{
           		perror("sendto");
           		exit(1);
      		}
      		sleep(1);
     	}
}
{% endhighlight %}

### Client -> receive
{% highlight C linenos %}
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>


#define HELLO_PORT 7788
#define HELLO_GROUP "225.0.0.1"
#define LOCAL   "127.0.0.1"
#define MSGBUFSIZE 256

main(int argc, char *argv[])
{
     struct sockaddr_in addr;
     int fd, nbytes,addrlen;
     struct ip_mreq mreq;
     char msgbuf[MSGBUFSIZE];

     u_int yes=1;            /*** MODIFICATION TO ORIGINAL */

     /* create what looks like an ordinary UDP socket */
     if ((fd=socket(AF_INET,SOCK_DGRAM,0)) < 0) {
	  perror("socket");
	  exit(1);
     }


/**** MODIFICATION TO ORIGINAL */
    /* allow multiple sockets to use the same PORT number */
    if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0) {
       perror("Reusing ADDR failed");
       exit(1);
       }
/*** END OF MODIFICATION TO ORIGINAL */

     /* set up destination address */
     memset(&addr,0,sizeof(addr));
     addr.sin_family=AF_INET;
     addr.sin_addr.s_addr=htonl(INADDR_ANY); /* N.B.: differs from sender */
     addr.sin_port=htons(HELLO_PORT);

     inet_pton(AF_INET, HELLO_GROUP, &addr.sin_addr.s_addr);
     
     /* bind to receive address */
     if (bind(fd,(struct sockaddr *) &addr,sizeof(addr)) < 0) {
	  perror("bind");
	  exit(1);
     }
     
     /* use setsockopt() to request that the kernel join a multicast group */
     mreq.imr_multiaddr.s_addr=inet_addr(HELLO_GROUP);
     inet_pton(AF_INET, LOCAL, &mreq.imr_multiaddr.s_addr);
     if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) != 0) {
	  perror("setsockopt");
	  exit(1);
     }

     /* now just enter a read-print loop */
     while (1) {
	  addrlen=sizeof(addr);
	  if ((nbytes=recvfrom(fd, msgbuf, MSGBUFSIZE, 0, (struct sockaddr *)&addr, &addrlen)) < 0) {
	       perror("recvfrom");
	       exit(1);
	  }
	  if (nbytes > 0)
	  {
		printf("%s\n", msgbuf);
	  }
     }
}
{% endhighlight %}

