import snap
import sys


def main(argv):
    node_count = int(argv[1])
    node_deg = int(argv[2])

    rnd = snap.TRnd()
    graph = snap.GenPrefAttach(node_count, node_deg, rnd)

    for edge in graph.Edges():
        print "%d, %d" % (edge.GetSrcNId(), edge.GetDstNId())
    
if __name__ == '__main__':
    if len(sys.argv) != 3:
        print 'Usage: python GenGraph.py <node_count> <node_degree>.'
        sys.exit(2)

    main(sys.argv)

    
