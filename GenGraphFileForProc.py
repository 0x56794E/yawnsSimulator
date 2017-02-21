####################
# Gen files containing link IDs for ea proc
# Usage: python GenGraphFileForProc.py <graph_file_name> <delim> <proc_count> <link_count>
#
# Output format:
# (1) File name: <graph_file_name>_<proc_count>_<proc the file is intended for>
# (2) Line format: <link ID><delim><Link data - as spec in given file>
####################

import sys #for argv and exit stuff
import math #for floor

# Generate files containing link IDs for ea proc
# Assumption: p == 4?
# General but haven't tested for p != 4
# Params:
# file_name: name of file containing edge list
# delim: delimeter used in the file to separate source and target nodes' ID
# p: number of procs
# n: number of links, i.e., line count of the given file
def genLinkIdFiles(file_name, delim, p, n):
    #TODO:
    #Output file format: <link_id> <src_id> <dst_id>

    rem = n % p    
    counts = [] #number of links for ea proc
    count = math.floor(n/p)
    
    for i in range (p):
        counts.append(count)

    #Additionally, first "rem" proc has 1 more
    for i in range (rem):
        counts[i] = counts[i] + 1

    cur_id = -1
    cur_p = 0
    count = 0 #number of link printed so far
    out_file_name = file_name + '_' + str(p) + '_'
    out_file = open(out_file_name + str(cur_p), 'w')
    
    #Generate for ea proc
    with open(file_name) as f:
        for line in f:
            # Haven't filled up for one proc yet
            if count < counts[cur_p]:
                
                #update ID
                cur_id = cur_id + 1

                if count == 0:
                    print 'Printing link with ID %d for proc %d\n' % (cur_id, cur_p)
                
                
                #print to file
                out_file.write(str(cur_id) + delim + line)
                count = count + 1
                
            # Move on to next proc
            else:
                print 'Moving from proc %d; last count = %d; id = %d\n' % (cur_p, count, cur_id)
                count = 0 # reset count
                cur_p = cur_p + 1

                #update output file name
                out_file.close()
                out_file = open(out_file_name + str(cur_p), 'w')
            

    pass
    

def writeToFile(data, output_file):
    # write data to output_file
    # rtype: None

    f = open(output_file, 'w')
    for line in data:
        f.write(line[0] + ',' + line[1] + '\n')

    f.close()        
    pass
    
if __name__ == '__main__':
    if len(sys.argv) != 5:
        print 'Usage: python GenGraphFileForProc.py <file_name> <delim> <proc_count> <link_count>\n'
        sys.exit(1)

    genLinkIdFiles(sys.argv[1], sys.argv[2], int(sys.argv[3]), int(sys.argv[4]))
