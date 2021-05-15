#!/usr/bin/python3
import numpy as np
import matplotlib.pyplot as plt
from sys import argv

def getfrom_stdin():
    size = []
    cachemiss = []
    while True:
        try:
            a = input()
            a = a.split()
            size.append(int(a[0]))
            cachemiss.append(int(a[1]))
        except EOFError:
            break
    return np.asarray(size), np.asarray(cachemiss)

def getfrom_file(name):
    data = np.loadtxt(name)
    size = data[:,0]
    cachemiss = data[:,1]
    return size, cachemiss

def main():
    if (len(argv) == 1):
        size, cachemiss = getfrom_stdin()
    elif len(argv) == 2:
        size, cachemiss = getfrom_file(argv[1])
    else:
        print("Usage:\nTo read from stdin: {0}\nTo read from file: {0} [file_name]".format(argv[0]))
        exit(-1)

    plt.plot(size, cachemiss)

    if len(argv) == 1:
        save_file = "stdin.png"
    else:
        save_file = "{0}.png".format(argv[1])

    plt.savefig(save_file)
    
    if len(argv) == 1:
        plt.show()

    

if __name__ == "__main__":
    main();
