from tkinter import *
from tkinter import filedialog

root = Tk()
root.title('filedialog study')

def save():
    global my_image # 함수에서 이미지를 기억하도록 전역변수 선언 (안하면 사진이 안보임)
    root.filename = filedialog.asksaveasfilename(initialdir='', title='파일선택', filetypes=(
    ('png files', '*.png'), ('jpg files', '*.jpg'), ('all files', '*.*')))


my_btn = Button(root, text='save', command=save).pack()
 
root.mainloop()
