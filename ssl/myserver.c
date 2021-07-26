/* serv.cpp  -  Minimal ssleay server for Unix
   30.9.1996, Sampo Kellomaki <sampo@iki.fi> */
/* mangled to work with SSLeay-0.9.0b and OpenSSL 0.9.2b
   Simplified to be even more minimal
   12/98 - 4/99 Wade Scholine <wades@mail.cybg.com> */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <memory.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/rsa.h>       /* SSLeay stuff */
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

/* define HOME to be dir for key and cert files... */
#define HOME "./"

/* 서버 인증서 */
#define CERTF  HOME "server-req.pem"
/* 서버 비밀키 */
#define KEYF  HOME  "server-key.pem"

#define CHK_NULL(x) if ((x)==NULL) exit (1)
#define CHK_ERR(err,s) if ((err)==-1) { perror(s); exit(1); }
#define CHK_SSL(err) if ((err)==-1) { ERR_print_errors_fp(stderr); exit(2); }

// SSL 핸드쉐이크 메시지교환 과정을 알려주는 콜벡함수
void  ssl_info_callback(const SSL *s, int where, int ret);

// 화면에 표시 하기 위한 파일 BIO생성
BIO * errBIO;

int main (int argc, char **argv)
{
    int err;
    int listen_sd;
    int sd;
    struct sockaddr_in sa_serv;
    struct sockaddr_in sa_cli;
    size_t client_len;

    SSL_METHOD *meth;
    SSL_CTX* ctx;
    SSL*     ssl;
    X509*    client_cert;

    char*    str;
    char     buf [4096];

    /* SSL preliminaries. We keep the certificate and key with the context. */
    // 모든 에러 스트링 로드
    SSL_load_error_strings();
    // 모든 알고리즘 로드
    SSLeay_add_ssl_algorithms();
    // SSL 버전3 프로토콜 사용
    meth = SSLv3_method();
    //meth = TLSv1_server    // create a new SSL_CTX object as framework for TLS/SSL enabled functions
    // SSL 컨텍스트 생성
    ctx = SSL_CTX_new (meth);
    if (!ctx) {
        ERR_print_errors_fp(stderr);
        exit(2);
    }

    // SSL 핸드쉐이크 메시지교환 과정을 알려주는 콜벡함수
    SSL_CTX_set_info_callback(ctx,ssl_info_callback);

    // 자신의 인증서를 파일에서 로딩한다.
    if (SSL_CTX_use_certificate_file(ctx, CERTF, SSL_FILETYPE_PEM) <= 0) {
        ERR_print_errors_fp(stderr);
        exit(3);
    }

    fprintf(stderr, "======== PEM pass phrease confirm here\n");
    // 자신의 개인키를 파일에서 로딩한다.
    if (SSL_CTX_use_PrivateKey_file(ctx, KEYF, SSL_FILETYPE_PEM) <= 0) {
        fprintf(stderr,"======== PEM pass phrase does not match the password\n");
        ERR_print_errors_fp(stderr);
        exit(4);
    }

    // 읽은 인증서와 개인키가 맞는지 확인 한다.
    if (!SSL_CTX_check_private_key(ctx)) {
        fprintf(stderr,"Private key does not match the certificate public key\n");
        exit(5);
    }

    listen_sd = socket (AF_INET, SOCK_STREAM, 0);
    CHK_ERR(listen_sd, "socket");

    memset (&sa_serv, '\0', sizeof(sa_serv));
    sa_serv.sin_family      = AF_INET;
    sa_serv.sin_addr.s_addr = INADDR_ANY;
    sa_serv.sin_port        = htons (1111);          /* Server Port number */

    err = bind(listen_sd, (struct sockaddr*) &sa_serv, sizeof (sa_serv));
    CHK_ERR(err, "bind");

    err = listen (listen_sd, 5);
    CHK_ERR(err, "listen");

    client_len = sizeof(sa_cli);
    sd = accept (listen_sd, (struct sockaddr*) &sa_cli, &client_len);
    CHK_ERR(sd, "accept");

    close (listen_sd);

    printf ("Connection from %lx, port %d\n",
    sa_cli.sin_addr.s_addr, sa_cli.sin_port);

    // TCP connection is ready. Do server side SSL.
    // create a new SSL structure for a connection
    // SSL 구조체 생성
    ssl = SSL_new (ctx);
    CHK_NULL(ssl);

    // connect the SSL object with a file descriptor
    // 연결된 소켓과 SSL과의 연결
    SSL_set_fd (ssl, sd);
    // 가장 중요한 함수, 클라이언트와의 초기 협상과정, 즉 핸드쉐이크 과정을 수행
    printf ("SSL_accept start =========================\n");
    err = SSL_accept (ssl);
    CHK_SSL(err);
    printf ("SSL_accept end =========================\n");

    // Get the cipher - opt
    // 현재 클라이언트와 정의된 암호화 파라메터정보를 얻음
    printf ("SSL connection using %s\n", SSL_get_cipher (ssl));
    printf ("SSL connection using %s\n", SSL_CIPHER_get_name(SSL_get_current_cipher(ssl)));

    // Get client's certificate (note: beware of dynamic allocation) - opt
    client_cert = SSL_get_peer_certificate (ssl);


    if (client_cert != NULL)
    {
        printf ("Client certificate:\n");

        str = X509_NAME_oneline (X509_get_subject_name (client_cert), 0, 0);
        CHK_NULL(str);
        printf ("\t subject: %s\n", str);
        free (str);

        str = X509_NAME_oneline (X509_get_issuer_name  (client_cert), 0, 0);
        CHK_NULL(str);
        printf ("\t issuer: %s\n", str);
        free (str);

        /* We could do all sorts of certificate verification stuff here before
           deallocating the certificate. */

        X509_free (client_cert);
    }
    else
    {
        printf ("Client does not have certificate.\n");
    }

    // DATA EXCHANGE - Receive message and send reply.
    // 클라이언트로 부터 SSL 통신을 통해 메시지 받음
    err = SSL_read (ssl, buf, sizeof(buf) - 1);
    CHK_SSL(err);

    buf[err] = '\0';
    // 받은 데이터를 화면에 표시
    printf ("Got %d chars:'%s'\n", err, buf);

    // 클라이언트에게 SSL 통신을 통해 메시지 보냄
    err = SSL_write (ssl, "I hear you.", strlen("I hear you."));
    CHK_SSL(err);
    /* Clean up. */
    close (sd);
    SSL_free (ssl);
    SSL_CTX_free (ctx);
}

// SSL 핸드쉐이크 메시지교환 과정을 알려주는 콜벡함수
void  ssl_info_callback(const SSL *s, int where, int ret)
{
  char * writeString;
  int w;
  // 현재 어떤 메시지 교환 과정인지를 나타냄
  w = where & ~SSL_ST_MASK;

  // 클라이언트가 연결 했을 때
  if (w & SSL_ST_CONNECT)
    writeString="SSL_connect";
  // 서버가 연결을 받았을 때
  else if (w & SSL_ST_ACCEPT)
    writeString="SSL_accept";
  // 알 수 없는 경우
  else
    writeString="undefined";

    fprintf(stderr, "======== writeString = [%s]\n", writeString);

  // 일반적인 핸드쉐이크 프로토콜 메시지일 경우
  if (where & SSL_CB_LOOP)
  {
    // SSL_state_string_long(s) 함수로 부터 현재 진행되는 메시지가 무엇인지 표시
    BIO_printf(errBIO,"%s:%s\n",writeString,SSL_state_string_long(s));
    fprintf(stderr, "======== writeString = [%s], SSL_state_string_long(s) = [%s]\n", 
        writeString, SSL_state_string_long(s));
  }
  else if (where & SSL_CB_ALERT)
  { // alert 프로토콜일 경우
    writeString=(where & SSL_CB_READ)?"read":"write";
    BIO_printf(errBIO,"SSL3 alert %s:%s:%s\n",writeString,SSL_alert_type_string_long(ret),SSL_alert_desc_string_long(ret));
    fprintf(stderr, "======== writeString = [%s], SSL_alert_type_string_long(ret) = [%s], SSL_alert_desc_string_long(ret) = [%s]\n", 
          writeString, SSL_alert_type_string_long(ret), SSL_alert_desc_string_long(ret));
  }
  else if (where & SSL_CB_EXIT)
  { // 종료 과정일 경우
    if (ret == 0) {
      BIO_printf(errBIO,"%s:failed in %s\n",writeString,SSL_state_string_long(s));
      fprintf(stderr,"======== writeString = [%s], SSL_state_string_long(s) = [%s]\n", 
          writeString, SSL_state_string_long(s));
    }
    else if (ret < 0)
    {
      BIO_printf(errBIO,"%s:error in %s\n",writeString,SSL_state_string_long(s));
      fprintf(stderr,"======== writeString = [%s], SSL_state_string_long(s) = [%s]\n", 
          writeString, SSL_state_string_long(s));
    }
  }
  return 1;
}