# -*- encoding: cp949 -*-
import socket,threading,Tkinter
 
sem = threading._Semaphore()
count = 0 #클라이언트 순서번호 매기기
th = []; conns = [] #쓰레드 집어넣을 리스트,
def to_client(conn,addr,count):
    cnt = count
    global conns
    for i in range(len(conns)):
        conns[i].sendall('%s에서, 유저 %d님이 접속하였습니다.' %(addr[0],cnt))
    print '%s에서, 유저 %d님이 접속하였습니다.' %(addr[0],cnt)
    conn.sendall('서버에 접속하셨습니다.\n당신은 유저 %d입니다' %cnt)
    try:
        while 1:
            read = conn.recv(1000)
            if read=='-1':
                conn.sendall('-1')
                exit(0)
            read = '유저 %d : %s' %(cnt,read)
            print read
            for i in range(len(conns)):
                conns[i].sendall(read)
    except:
        print '유저 %d 님이 나가셨습니다.' %cnt
        conns.remove(conn)
        for i in range(len(conns)):
            conns[i].sendall('유저 %d 님이 나가셨습니다.' %cnt)
        exit(0)
 
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.bind(('127.0.0.1',50000))
s.listen(1)
 
while 1:
    conn,addr = s.accept()
    conns.append(conn)
    sem.acquire(); count += 1; sem.release()
    client = threading.Thread(target=to_client,args=(conn,addr,count))
    client.start()
    th.append(client);
for t in th:
    t.join()

