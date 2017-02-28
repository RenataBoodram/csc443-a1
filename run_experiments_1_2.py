import matplotlib.pyplot as plt
import sys, getopt
import subprocess
import math


# KB = 1024
# MB = 1024 * 1024
# sizes = [ 512, 1 * KB, 4 * KB, 8 * KB, 16 * KB, 32 * KB, 1 * MB, 2 * MB, 4 * MB ]

def plot_graph(x, y, xlabel, ylabel):
    # plt.plot(x, y, "ro")
    width = 1/1.5

    plt.bar(x, y, width, color="blue")
    plt.xlabel(xlabel)
    plt.ylabel(ylabel)
    plt.show()

    # plt.bar(x, y, width, color="blue")


# def run_exp(cmd):
#     bps_list = []
#     for size in sizes:
#         output = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
#         for line in output.stdout:
#             if "Data rate" in line:
#                 line_list = line.split(" ")
#                 print(line_list[2])
#                 bps_list.append(line_list[2])
#     plot_graph(bps_list)


# def main(argv):
#     cmd = "".join(str(e) for e in argv)
#     run_exp(cmd)


if __name__ == "__main__":
    xlabel = "log_2 of memory sizes"
    ylabel = "Real time"

    x = [
    104857600,
    52428800,
    26214400, 
    13107200, 
    6553600
    ]

    y = [
    4.88,
    4.15,
    4.35,
    4.65,
    4.21]


    x_log = []

    for elem in x:
    	x_log.append(math.log(elem, 2))
    

    plot_graph(x_log, y, xlabel, ylabel);
