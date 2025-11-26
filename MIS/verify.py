import sys
from config import *

def files_equal(path1, path2, block_size=4096):
    with open(path1, "rb") as f1, open(path2, "rb") as f2:
        while True:
            b1 = f1.read(block_size)
            b2 = f2.read(block_size)
            if b1 != b2:
                return False
            if not b1: 
                return True

if __name__ == "__main__":
    if len(sys.argv) != 3:
        exit(1)
    algo1 = str(sys.argv[1])
    algo2 = str(sys.argv[2])
    for graph in graphs:
        path1 = algo1 + "/output/" + graph + ".txt"
        path2 = algo2 + "/output/" + graph + ".txt"
        print(files_equal(path1, path2), graph)

# python3 verify.py 01_sequential 02_sequential_dag