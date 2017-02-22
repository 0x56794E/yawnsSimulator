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

import java.util.stream.Stream;
import java.util.Map;
import java.util.HashMap;
import java.util.List;
import java.util.ArrayList;

public class NeighborFinder
{
    
    public static void main(String[] args) throws IOException
    {
        if (args.length < 2)
        {
            System.out.println("Must provide input file name");
            System.exit(1);                
        }

        
        doGenerate(args[0], args[1]);
                
    }
    
    private static void doGenerate(String fileName, String delim) throws IOException
    {
        Stream<String> lines = Files.lines(Paths.get(fileName));

        //Key: node
        Map<Integer, List<Integer>> nodeMap = new HashMap<>();

        lines.forEach(line -> {
                String[] toks = line.split(delim);


        });
    }

    private static class Link
    {
        public final int linkId;
        public final String  srcId;
        public final String dstId;

        public Link(int linkId, String srcId, String dstId)
        {
            this.linkId = linkId;
            this.srcId = srcId;
            this.dstId = dstId;
        }

        /**
         * Stupid yet simple and working
         */
        public int hashCode()
        {
            return String.format("%s_%s", srcId, dstId).hashCode();
        }

        public boolean equals(Object obj)
        {
            if (!(obj instanceof Link))
                return false;

            Link that = (Link) obj;
            return this.srcId.equals(that.srcId)
                && this.dstId.equals(that.dstId);
        }
    }
}
