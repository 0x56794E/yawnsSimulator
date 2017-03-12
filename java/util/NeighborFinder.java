/**
 * Helper for generating files containing IDs of neighbors 
 * of all links in a given graph.
 * 
 * Input: edge list file
 * ASSUMPTION: link id is the order it appears in the edge list file
 *
 * Output: files whose name has this format: 
 * <input_file_name>_<own_link_id>.txt
 * Each file contains a list of IDs of the link's neighbors
 */

import java.nio.file.Files;
import java.nio.file.Paths;
import java.io.IOException;
import java.io.File;

import java.util.stream.Stream;
import java.util.Map;
import java.util.HashMap;
import java.util.Set;
import java.util.HashSet;

public class NeighborFinder
{
    private final String fileName;
    private final String delim;
    private int idx = 0; //TODO: hacky way to keep track of idx of item in stream
    
    //Key: node ID; Value: list of links touching node with given ID
    private Map<String, Set<Link>> nodeMap = new HashMap<>();

    //Key: link ID; value: Link obj
    private Map<Integer, Link> linkMap = new HashMap<>();
    
    public static void main(String[] args) throws IOException
    {
        if (args.length < 2)
        {
            System.out.println("Usage: java NeighborFinder <graph file name> <delim>");
            System.exit(1);                
        }

        NeighborFinder finder = new NeighborFinder(args[0], args[1]);
        finder.exec();                
    }

    private NeighborFinder (String fileName, String delim)
    {
        this.fileName = fileName;
        this.delim = delim;            
    }
    
    private void exec() throws IOException
    {
        Stream<String> lines = Files.lines(Paths.get(fileName));
        lines.forEach(line -> process(line));

        updateNeighborSet();
        writeToFile();
    }

    /**
     * Line has format: <number><delim><number>
     * 
     */
    private void process(String line)
    {
        String toks[] = line.split(delim);
        Link link = new Link (idx, toks[0], toks[1]);
        linkMap.put(idx, link);
        
        if (!nodeMap.containsKey(toks[0]))
            nodeMap.put(toks[0], new HashSet<Link>());

        if (!nodeMap.containsKey(toks[1]))
            nodeMap.put(toks[1], new HashSet<Link>());

        nodeMap.get(toks[0]).add(link);
        nodeMap.get(toks[1]).add(link);        
        
        //Incr the idx => this is the ID of the link
        ++idx;
    }

    /**
     * Traverse the nodeMap and linkMap
     * to update links' neighbor set
     */
    private void updateNeighborSet()
    {
        for (Map.Entry<Integer, Link> entry : linkMap.entrySet())
        {
            Link link = entry.getValue();
            
            //Add links assoc with srcNode as neis
            for (Link nei : nodeMap.get(link.srcId))
            {
                link.addNei(nei);
                nei.addNei(link);
            }

            //Add links assoc with dstNode as neis
            for (Link nei : nodeMap.get(link.dstId))
            {
                link.addNei(nei);
                nei.addNei(link);
            }
        }
    }

    /**
     * Write the neighbor set to file.
     * File name format: <input_file_name>_<link_id>.txt
     * Line format: <nei_id>
     */
    private void writeToFile() throws IOException
    {
        //Make dir to store these files
        String dirName = fileName + "_link_nei";
        new File(dirName).mkdir();
        
        for (Link link : linkMap.values())
        {
            Files.write(Paths.get(dirName + "/" + link.linkId + ".txt"), link.getNeiIDs());
        }
    }
    
    private static class Link
    {
        public final int linkId;
        public final String srcId;
        public final String dstId;

        private Set<String> neis = new HashSet<>();
        
        public Link(int linkId, String srcId, String dstId)
        {
            this.linkId = linkId;
            this.srcId = srcId;
            this.dstId = dstId;
        }

        public void addNei(Link nei)
        {
            neis.add(String.valueOf(nei.linkId));
        }

        public Set<String> getNeiIDs()
        {
            return neis;
        }

        /**
         * Stupid yet simple and working
         */
        public int hashCode()
        {
            return linkId;
        }

        public boolean equals(Object obj)
        {
            if (!(obj instanceof Link))
                return false;

            Link that = (Link) obj;
            return this.linkId == that.linkId;
        }
    }
}
