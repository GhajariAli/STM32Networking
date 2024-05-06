#include "TCPFunctions.h"

static struct netconn *conn, *newconn;
static struct netbuf *buf;
char msg1[100];
char smsg1[200];

static void tcp_thread(void *arg){
	err_t err;

	conn = netconn_new(NETCONN_TCP);
	if (conn!=NULL){
		err = netconn_bind(conn, IP_ADDR_ANY, 80);
		if (err == ERR_OK){
			netconn_listen(conn);
			while (1){
				if (netconn_accept(conn, &newconn) == ERR_OK){
					while (netconn_recv(newconn, &buf) == ERR_OK){
						do{
							strncpy (msg1, buf->p->payload, buf->p->len);
							int len = sprintf (smsg1, "\"%s\" Received\n", msg1);
							netconn_write(newconn, smsg1, len, NETCONN_COPY);
							memset (msg1, '\0', 100);
						}while (netbuf_next(buf) >0);
						netbuf_delete(buf);
					}
					netconn_close(newconn);
					netconn_delete(newconn);
				}
			}
		}
		else	netconn_delete(conn);
	}
}


void TCPServer_init(void)
{
  sys_thread_new("tcp_thread", tcp_thread, NULL, DEFAULT_THREAD_STACKSIZE,osPriorityNormal);
}
