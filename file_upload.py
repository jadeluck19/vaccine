from tkinter import *

import tkinter.filedialog


title = 'Save project as'

ftypes = [('csv file', '.csv'), ('All files', '*')]

filename = tkinter.filedialog.asksaveasfilename(filetypes=ftypes, title=title,

                                                        initialfile='noname.csv')

