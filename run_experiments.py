import matplotlib.pyplot as plt
import sys, getopt
import subprocess

KB = 1024
MB = 1024 * 1024
sizes = [ 512, 1 * KB, 4 * KB, 8 * KB, 16 * KB, 32 * KB, 1 * MB, 2 * MB, 4 * MB ]

def plot_graph(bps_list):
    plt.plot(bps_list, sizes, "ro")
    plt.xlabel("Speed (bytes/sec)")
    plt.ylabel("Block size (in bytes)")
    plt.show()

def run_exp(cmd):
    bps_list = []
    for size in sizes:
        output = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
        for line in output.stdout:
            if "Data rate" in line:
                line_list = line.split(" ")
                print(line_list[2])
                bps_list.append(line_list[2])
    plot_graph(bps_list)


def main(argv):
    cmd = "".join(str(e) for e in argv)
    run_exp(cmd)


if __name__ == "__main__":
    main(argv)
