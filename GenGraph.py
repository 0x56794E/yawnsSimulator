import snap
import sys
import math #for ceil and floor stuff

def genGraph(argv):
    node_count = int(argv[1])
    node_deg = int(argv[2])
    p = int(argv[3])
            
    rnd = snap.TRnd()
    graph = snap.GenPrefAttach(node_count, node_deg, rnd)
    edge_count = len(list(graph.Edges()))

    #determine if to use ceil or floor
    #math.ceil(<num>); math.floor(<num>)
    if edge_count % p >= p / 2:
        print 'p = %d, edge_count = %d, p/2 = %f => use ceil\n' % (p, edge_count, p/2)
    else:  
        print 'p = %d, edge_count = %d, p/2 = %f =>  use floor\n' % (p, edge_count, p/2)
        
    #graph_file_name = 'g' + str(node_count) + '_' + str(node_deg)
    #graph_file = open(graph_file_name, 'w')

    #start with proc 0
    #update every edge_count/p
    #per_proc_file_name = graph_file_name + '0'
    #per_proc_file = open(per_proc_file_name)
            
    #count = 0

            
#    for edge in graph.Edges(): Write the edge list to file print "%d
        # -- %d;" % (edge.GetSrcNId(), edge.GetDstNId())

        # Additionally, write the edge ID to the appropriate file for
        # ea proc ++count


def writeToFile(data, output_file):
    # write data to output_file
    #rtype: None

    f = open(output_file, 'w')
    for line in data:
        f.write(line[0] + ',' + line[1] + '\n')

    pass
    
if __name__ == '__main__':
    if len(sys.argv) != 4:
        print 'Usage: python GenGraph.py <node_count> <node_degree> <proc_count>.'
        sys.exit(2)

    genGraph(sys.argv)

    
