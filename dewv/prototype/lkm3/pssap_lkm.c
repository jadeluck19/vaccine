/**
 * @file 	pssap_lkm.c
 * @author 	SecuritySchool	
 * @brief	Airplane Prototype - LKM
 */
 
#include <linux/init.h>             // For init, exit macro
#include <linux/module.h>           // Core header for LKM
#include <linux/kernel.h>           // Core header for kernel
#include <linux/binfmts.h>			// For bprm

#include <linux/net.h>			// socket_create
#include <net/sock.h>			// struct socket
#include <linux/inet.h>			// in_aton

#include <linux/kthread.h>		// kthread_XXX
#include <linux/version.h>		// LINUX_VERSION_CODE

///////////////////////////////////////////////////////////////////////////////

MODULE_LICENSE("GPL");
MODULE_AUTHOR("SecuritySchool");
MODULE_DESCRIPTION("Airplane Prototype - LKM");
MODULE_VERSION("0.3");	// 0.3: kernel tcp socket

///////////////////////////////////////////////////////////////////////////////

#define MYCLIENT_MAX_MSG  512

///////////////////////////////////////////////////////////////////////////////

// kernel thread for socket client
static struct task_struct * pssapl_sock_thread = 0;
// socket
struct socket * sock = NULL;

///////////////////////////////////////////////////////////////////////////////

// Kernel Module Init
static int __init pssapl_init(void);
// Module Exit function
static void __exit pssapl_exit(void);

// Set init function to kernel
module_init(pssapl_init);
// Set exit function to kernel
module_exit(pssapl_exit);

// symbols from kernel
typedef int (*security_bprm_check_func)(struct linux_binprm *bprm);
extern void security_bprm_check_set_pss_filter( security_bprm_check_func pfunc);
extern void security_bprm_check_unset_pss_filter(void);
// filter function 
static int pssapl_filter_func(struct linux_binprm *bprm);

// Send socket wrapper
ssize_t pssapl_send(struct socket *sk, const void *buffer, size_t length, int flags);
// kernel recv socket wrapper
ssize_t pssapl_recv(struct socket *sk, void *buffer, size_t length, int flags);
// Shutdown socket
int pssapl_shutdown(struct socket *sk, int how);
// kernel socket close
void pssapl_close(struct socket *sk);
// client socket thread function
static int pssapl_client_func(void *arg);

///////////////////////////////////////////////////////////////////////////////

// Module Init function	
static int __init pssapl_init(void)
{
	printk(KERN_INFO "[pssapl] Hello LKM!\n" );
	// set filter 	
	security_bprm_check_set_pss_filter(pssapl_filter_func);

	// Start thread for client 
	pssapl_sock_thread = kthread_create(pssapl_client_func,NULL,"pss_filter_thread");
	if(NULL != pssapl_sock_thread)
		wake_up_process(pssapl_sock_thread);

	return 0;
}
 

// Module Exit function
static void __exit pssapl_exit(void)
{
	// Shutdown recv socket to therminate thread
	pssapl_shutdown(sock,SHUT_RDWR);

	// Stop thread for client
	if( 0 != pssapl_sock_thread )
	{
		kthread_stop(pssapl_sock_thread);
		pssapl_sock_thread = 0;
	}

	// unset filter before exit
	security_bprm_check_unset_pss_filter();
	printk(KERN_INFO "[pssapl] Goodbye LKM!\n" );
}


// filter function 
static int pssapl_filter_func(struct linux_binprm *bprm)
{
	printk(KERN_INFO "[pssapl] New process (file:%s)\n", bprm->filename);

	if( NULL != bprm->filename && 0 == strcmp(bprm->filename, "/bin/my_block_sample") )
	{
		printk(KERN_INFO "[pssapl] blocked\n");
		return -EACCES;
	}
	return 0;
}


// kernel socket send wrapper
ssize_t pssapl_send(struct socket * sk, const void *buffer, size_t length, int flags)
{
	struct 	msghdr msg;
	struct 	iovec iov;
	int 	ret = 0;

	// Set message
	iov.iov_base = (void *)buffer;
	iov.iov_len = (__kernel_size_t)length;

#if LINUX_VERSION_CODE <= KERNEL_VERSION(4,0,0)
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
#else
	iov_iter_init(&msg.msg_iter, WRITE, &iov, 1, length);
#endif
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_control = NULL;
	msg.msg_controllen = 0;
	msg.msg_flags = flags;

#if LINUX_VERSION_CODE <= KERNEL_VERSION(4,0,0)
	// Send message
	ret = sock_sendmsg(sk, &msg, length);
#else
	// Send message
	ret = sock_sendmsg(sk, &msg);
#endif


	return ret;
}


// kernel recv socket wrapper
ssize_t pssapl_recv(struct socket * sk, void *buffer, size_t length, int flags)
{
	struct msghdr msg;
	struct iovec iov;
	int ret;
	
	// Set message
	iov.iov_base = (void *)buffer;
	iov.iov_len = (__kernel_size_t)length;

#if LINUX_VERSION_CODE <= KERNEL_VERSION(4,0,0)
	msg.msg_iov = &iov;
	msg.msg_iovlen = 1;
#else
	iov_iter_init(&msg.msg_iter, READ, &iov, 1, length);
#endif
	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	msg.msg_control = NULL;
	msg.msg_controllen = 0;
	msg.msg_flags = flags;
	
	// Receive message
	ret = sock_recvmsg(sk, &msg, length, flags);

	return ret;
}


// Shutdown socket wrapper
int pssapl_shutdown(struct socket *sk, int how)
{
	int ret = 0;

	if (sk)
		ret = sk->ops->shutdown(sk, how);
	
	return ret;
}


// kernel socket close
void pssapl_close(struct socket *sk)
{
	sk->ops->release(sk);
	if(sk)
		sock_release(sk);
}

// client socket thread function
static int pssapl_client_func(void *arg)
{
    struct sockaddr_in serv_addr;
    static const char * cli_msg = "Hello Server?";
    char serv_msg[MYCLIENT_MAX_MSG] = {0x00, };
    int ret = -1;
     
    // Create Socket
    ret = sock_create(AF_INET , SOCK_STREAM , 0, &sock);
    if (ret != -1)
    {
    	// TODO: change IP address as your updat server's
    	serv_addr.sin_addr.s_addr = in_aton("127.0.0.1");
   	    // serv_addr.sin_addr.s_addr = in_aton("192.168.177.100");
	    serv_addr.sin_family = AF_INET;
	    serv_addr.sin_port = htons( 8888 );
	 
	 	do{
		    // Connect to server
		    ret = sock->ops->connect(sock , (struct sockaddr *)&serv_addr , sizeof(serv_addr), 0);
		    if(0 <= ret)
		    {
		        // Send a message to the server
		        ret = pssapl_send(sock , cli_msg , strlen(cli_msg) , 0);
		        if(0 >= ret)
		        {
		            printk(KERN_ERR "[pssapl] Send failed");
		            break;
		        }
		        printk("[pssapl] Sent : %s\n", cli_msg); 
		         
		        while(1)
		        {
			        // Receive a reply from the server
			        ret = pssapl_recv(sock , serv_msg , MYCLIENT_MAX_MSG-1 , 0);
			        if(0 >= ret)
			        {
			            printk(KERN_ERR "[pssapl] recv failed");
			            break;
			        }
			        serv_msg[ret] = 0;
			    
			        // Print reply from the server    
			        printk("[pssapl] Received : %s\n", serv_msg); 
				}

				// In this prototype, never reach here except error
		    }
		    else
		    {
		        printk(KERN_ERR "[pssapl] connect failed. Error");
		        break;
		    }
		}while(0);

		// If need, close socket
		if(NULL != sock)
		{
	    	pssapl_close(sock);
	    	sock = NULL;
		}
	}
    else
	{
		printk("[pssapl] Could not create socket");
    }

    // Reset thread
    pssapl_sock_thread = NULL;

    return ret;
}

/////////////////////////////////////////////////////////////////////////////
