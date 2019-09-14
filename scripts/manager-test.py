#!/usr/bin/python3

import subprocess
import os
import time
import threading

def print_thread(read_fd):
    char, line = '', ""
    while True: 
        char = os.read(read_fd, 1).decode("utf-8")
        if char == '\0': return
        elif char == '\n':
            print(line)
            line = ""
        else: line += char

def signal_shutdown(read_fd, write_fd) -> bool:
    try:
        os.write(write_fd, str.encode('0'))
        resp = os.read(read_fd, 1).decode("utf-8")
        if resp != '0': return False
        else: return True
    except: return False 

def get_match(read_fd, write_fd) -> tuple:
    try:
        os.write(write_fd, str.encode('1'))
        resp = os.read(read_fd, 1).decode("utf-8")
        cid_a = int.from_bytes(os.read(read_fd, 4), byteorder="little")
        cid_b = int.from_bytes(os.read(read_fd, 4), byteorder="little")
        if resp != '1': return (-1, -1)
        else: return (cid_a, cid_b)
    except: return (-1, -1) 

def add_contribution(read_fd, write_fd, contribution_id) -> bool:
    try:
        os.write(write_fd, str.encode('2'))
        os.write(write_fd, contribution_id.to_bytes(4, byteorder="little"))
        resp = os.read(read_fd, 1).decode("utf-8")
        if resp != '2': return False
        else: return True
    except: return False 
    
# build process pipes
r_man, w_ctrl = os.pipe()
r_ctrl, w_man = os.pipe()
in_ctrl, out_man = os.pipe()

# fork engine process
pid = os.fork()
if pid:   

    # launch polling thread
    thread = threading.Thread(target=print_thread, 
        args=(in_ctrl,)) 
    thread.start()

    # run tests
    time.sleep(3)
    # print(signal_shutdown(r_ctrl, w_ctrl))
    # print(get_match(r_ctrl, w_ctrl))
    # print(add_contribution(r_ctrl, w_ctrl, 101))

    # join polling thread
    thread.join()

    # close file descriptors
    os.close(r_ctrl)
    os.close(w_ctrl)
    os.close(in_ctrl)
    os.close(r_man)
    os.close(w_man)
    os.close(out_man)

    # wait for child process
    os.waitpid(pid, 0)

else:
    
    # launch manager
    os.dup2(out_man, 1)
    subprocess.call([
        "./mind-engine", 
        "manager", 
        str(0), 
        str(r_man), 
        str(w_man),
    ], pass_fds=(r_man, w_man))
    
    # send end signal
    print('\0')
    
    # close file descriptors
    os.close(r_ctrl)
    os.close(w_ctrl)
    os.close(in_ctrl)
    os.close(r_man)
    os.close(w_man)
    os.close(out_man)
