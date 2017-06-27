####################
# Gen files containing node IDs for ea proc
# Usage: python GenNodeProc.py <graph_file_name> <proc_count> <node count>
#
# Output format:
# (1) File name: node_<graph_file_name>_<proc_count>_<proc the file is intended for>
# (2) Line format: <node ID>
####################

import sys #for argv and exit stuff
import math #for floor

# Generate files containing node IDs for ea proc
# Assumption: p == 4?
# General but haven't tested for p != 4
# Params:
# file_name: name of file containing edge list
# delim: delimeter used in the file to separate source and target nodes' ID
# p: number of procs
# n: number of nodes
def genNodeIdFiles(graph_file_name, p, n):
    #Output file format: <node_id>

    rem = n % p    
    counts = [] #number of nodes for ea proc
    count = math.floor(n/p)
    for i in range (p):
        counts.append(count)

    #Additionally, first "rem" proc has 1 more
    for i in range (rem):
        counts[i] = counts[i] + 1

    cur_id = -1
    cur_p = 0
    count = 0 #number of nodes printed so far
    out_file_name = 'node_' + graph_file_name + '_' + str(p) + '_'
    out_file = open(out_file_name + str(cur_p), 'w')
   
    #Generate for ea proc
    #Since the nodes are ID'ed from 0 => no need to read from file for IDs
    #IDs would be in range [0, n-1]
    for node_id in range (n):
        # move on to next proc
        if count >= counts[cur_p]:
			count = 0
			cur_p = cur_p + 1

			#update output file name
			out_file.close()
			out_file = open(out_file_name + str(cur_p), 'w')

        # Write the line to the file
        out_file.write(str(node_id) + '\n')
        count = count + 1
    pass
    
if __name__ == '__main__':
    if len(sys.argv) != 4:
        print 'Usage: python GenNodePerProc.py <file_name> <proc_count> <node_count>\nExiting...'
        sys.exit(1)

    genNodeIdFiles(sys.argv[1], int(sys.argv[2]), int(sys.argv[3]))
