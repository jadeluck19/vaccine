# -*- encoding: cp949 -*-
import socket,threading,Tkinter
 
th = []
def listen(s):
    global text
    while 1:
        read = s.recv(1000)
        if read=='-1':
            exit(0)
        text.insert('insert',unicode(read,'cp949'))
        text.insert('insert','\n')
 
def button1Click(event):
    global s
    global E
    if E.get()=='-1':
        s.sendall('-1')
    s.sendall( E.get().encode('cp949') )
     
def button2Click(event):
    global s
    s.sendall('-1')
    root.destroy()
 
s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
s.connect(('127.0.0.1',50000))
try:
    root = Tkinter.Tk()
    f = Tkinter.Frame(root)
    f.pack()
    L = Tkinter.Label(f, text=unicode("메시지 : ",'cp949'))
    L.pack( side = 'left')
    E = Tkinter.Entry(f, bd =5)
    E.pack(side = 'left')
 
    text=Tkinter.Text(root)
    text.pack()
    text.insert('insert',unicode(s.recv(1000),'cp949'))
    text.insert('insert','\n\n')
 
    button2=Tkinter.Button(f,text=unicode('나가기','cp949'),background='red')
    button2.pack(side='right')
    button2.bind('<Button-1>',button2Click)
 
    l = threading.Thread(target=listen,args=(s,))
    th.append(l)
    l.start()
 
    button1=Tkinter.Button(f,text=unicode('보내기','cp949'),background='white')
    button1.pack(side='right')
    button1.bind('<Button-1>',button1Click)
 
    root.mainloop()
except:
    pass
    exit(0)
for t in th:
    t.join()


