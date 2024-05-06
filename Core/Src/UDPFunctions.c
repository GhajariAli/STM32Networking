#include "UDPFunctions.h"

static struct netconn *conn;
static struct netbuf *buf;
char msg[100];
char smsg[200];
static ip_addr_t *addr;
static unsigned short port;

static void udp_thread(void *arg)
{
	err_t err, recv_err;
	struct pbuf *txBuf;

	conn = netconn_new(NETCONN_UDP);
	if (conn!= NULL){
		err = netconn_bind(conn, IP_ADDR_ANY, 7400);
		if (err == ERR_OK){
			while (1){
				recv_err = netconn_recv(conn, &buf);
				if (recv_err == ERR_OK){
					addr = netbuf_fromaddr(buf);
					port = netbuf_fromport(buf);
					strcpy (msg, buf->p->payload);
					int len = sprintf (smsg, "\"%s\" was sent by the Client\n", (char *) buf->p->payload);
					txBuf = pbuf_alloc(PBUF_TRANSPORT,len, PBUF_RAM);
					pbuf_take(txBuf, smsg, len);
					buf->p = txBuf;
					netconn_connect(conn, addr, port);
					netconn_send(conn,buf);
					buf->addr.addr = 0;
					pbuf_free(txBuf);
					netbuf_delete(buf);
				}
			}
		}
		else
		{
			netconn_delete(conn);
		}
	}
}

void UDPServer_init(void)
{
  sys_thread_new("udp_thread", udp_thread, NULL, DEFAULT_THREAD_STACKSIZE,osPriorityLow);
}
