# -*- coding: UTF-8 -*-

import time
import Tkinter as tk
import subprocess

class Window:
    def __init__(self, title='Guard', width=480, height=160):
        self.w = width
        self.h = height
        self.stat = True
#        self.startFunc = startFunc
#        self.stopFunc = stopFunc
        self.staIco = None
        self.stoIco = None

        self.root = tk.Tk(className=title)

    def run_command(self, cmd, *args):
        if not cmd:
            return ''
        if args:
            cmd = ' '.join((cmd,) + args)
        p = subprocess.Popen(cmd, shell=True,stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        print (cmd)
        rf, ef = p.stdout, p.stderr
        errors = ef.read()
        if errors:
            print("ERROR: %s" % errors)
        return rf.read().strip()

    def start(self):
        channel = self.channel.get()

        print('start, channel : %s' % channel)

        print self.run_command("ls", "-al")

        print self.run_command("echo", "-c " + channel)

        return True

    def stop(self):
        print('stop.')
        return True

    def center(self):
        ws = self.root.winfo_screenwidth()
        hs = self.root.winfo_screenheight()
        x = int( (ws/2) - (self.w/2) )
        y = int( (hs/2) - (self.h/2) )
        self.root.geometry('{}x{}+{}+{}'.format(self.w, self.h, x, y))


    def packBtn(self):
        self.lab = tk.Label(self.root, text='监测信道(1,6,11):')
        self.lab.grid(row = 0, column = 0)
        self.channel = tk.Entry(self.root)
        self.channel.grid(row = 0, column = 1, sticky = tk.E)

        self.btnSer = tk.Button(self.root, command=self.event, width=10, height=2)
        self.btnSer.grid(row = 3, column = 0, sticky = tk.W)
        self.btnQuit = tk.Button(self.root, text='关闭窗口', command=self.root.quit, width=10, height=2)
        self.btnQuit.grid(row = 3, column = 1, sticky = tk.E)

    def event(self):
        self.btnSer['state'] = 'disabled'
        if self.stat:
            if self.stop():
                self.btnSer['text'] = '启动服务'
                self.stat = False
#                self.root.iconbitmap(self.stoIco)
        else:
            if self.start():
                self.btnSer['text'] = '停止服务'
                self.stat = True
#                self.root.iconbitmap(self.staIco)
        self.btnSer['state'] = 'active'

    def loop(self):
        self.root.resizable(False, False)   #禁止修改窗口大小
        self.packBtn()
        self.center()                       #窗口居中
        self.event()

        self.root.mainloop()

########################################################################

if __name__ == '__main__':
    import sys, os

    w = Window()
#    w.staIco = os.path.join(sys.path[0], 'test.xbm')
#    w.stoIco = os.path.join(sys.path[0], 'stop.xbm')
    w.loop()

