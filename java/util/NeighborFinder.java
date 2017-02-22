/**
 * Helper for generating files containing IDs of neighbors 
 * of all links in a given graph.
 * 
 * Input: edge list file
 * ASSUMPTION: link id is the order it appears in the edge list file
 */

import java.nio.file.Files;
import java.nio.file.Paths;
import java.io.IOException;
import java.util.stream.Stream;

public class NeighborFinder
{
    public static void main(String[] args) throws IOException
    {
        if (args.length < 1)
        {
            System.out.println("Must provide input file name");
            System.exit(1);                
        }

        
        doGenerate(args[0]);
                
    }

    private static void doGenerate(String fileName) throws IOException
    {
        Stream<String> lines = Files.lines(Paths.get(fileName));

        lines.forEach(line -> {
                System.out.println("Processing line " + line);
        });
    }
}
