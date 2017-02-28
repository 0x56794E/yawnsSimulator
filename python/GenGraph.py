######
# Generate scale-free graph and output the edge list
# Usage: python GenGraph.py <node_count> <node_degree> <output fmt, optional>
# Default output format to %d,%d (comma separated)
######

import snap
import sys

def genGraph(argv):
    node_count = int(argv[1])
    node_deg = int(argv[2])

    fmt = "%d,%d"

    if (len(argv) > 3):
        fmt = argv[3]
    
    rnd = snap.TRnd()
    graph = snap.GenPrefAttach(node_count, node_deg, rnd)
    for edge in graph.Edges():
        #Write the edge list to file
        print fmt % (edge.GetSrcNId(), edge.GetDstNId())
    
if __name__ == '__main__':
    if len(sys.argv) < 3:
        print 'Usage: python GenGraph.py <node_count> <node_degree> [format str, optional]'
        sys.exit(1)

    genGraph(sys.argv)

    
