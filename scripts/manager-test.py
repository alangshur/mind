#!/usr/bin/python3

import subprocess
import os
import time
import threading

# build process pipes
r_man, w_ctrl = os.pipe()
r_ctrl, w_man = os.pipe()
in_ctrl, out_man = os.pipe()

# fork engine process
pid = os.fork()
if pid:

    # close unused file descriptors
    os.close(r_man)
    os.close(w_man)
    os.close(out_man)

    # define polling thread
    def print_thread(read_file):
        line = ""
        while True: 
            line = read_file.readline()
            if line == "\0\n": return
            print(line, end="")

    # launch polling thread
    read_file = os.fdopen(in_ctrl)
    thread = threading.Thread(target=print_thread, 
        args=(read_file,)) 
    thread.start()

    # write tests


    # join polling thread
    thread.join()

    # close file descriptors
    os.close(r_ctrl)
    os.close(w_ctrl)
    read_file.close()

    # wait for child process
    os.waitpid(pid, 0)

else:

    # close unused file descriptors
    os.close(r_ctrl)
    os.close(w_ctrl)
    os.close(in_ctrl)
    
    # launch manager
    os.dup2(out_man, 1)
    subprocess.call(["./mind-engine", "manager", str(0), 
        str(r_man), str(w_man)])
    
    # send end signal
    print("\0\n")
    
    # close file descriptors
    os.close(r_man)
    os.close(w_man)
    os.close(out_man)