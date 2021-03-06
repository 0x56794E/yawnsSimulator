import java.nio.file.Files;
import java.nio.file.Paths;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.stream.Stream;

/**
 * Generate traffic 
 * Input:
 * (0) Files containting edge lists for each proc (1 file /proc)
 * (1) Number of processors to be used p
 * (2) TODO: interarrival time
 *
 * Output: p files each containing traffic for ea proc 
 * (i.e., packets that originate from the local LPs on ea proc.)
 * Ea line has this format: <source LP ID> <start time> <stop count>
 * 
 * TODO: make interarrival time follow poisson
 * @obsolete as of May 1
 * @author Vy Thuy Nguyen
 * @since 2/22/2017
 */
public class TrafficGenerator
{
    //TODO: tentative for now
    private static int MAX_PACKET = 20;
    private static int MAX_STOP = 10;
        
    public static void main(String[] args) throws IOException
    {
        if (args.length < 3)
        {
            System.out.println("Usage: java TrafficGenerator <graphFileName> <procCount> <delim of file spec the edge list for ea proc>");
            System.exit(1);
        }
           
        //If max packet and max stop are spec
        if (args.length == 5)
        {
        	MAX_PACKET = Integer.parseInt(args[3]);
        	MAX_STOP = Integer.parseInt(args[4]);
        }
        
        TrafficGenerator gen = new TrafficGenerator();
        gen.genTraffic(args[0], Integer.parseInt(args[1]), args[2]);
    }

    public void genTraffic(final String graphFileName, final int p, final String delim) throws IOException
    {
        int numPacket; //Num packet on ea LP
        int numStop; //Num stop of ea packet
        int startTime; //Arrival time => TODO: make this follow poisson dist.
        String outputFileName;
        
        for (int rank = 0; rank < p; ++rank)
        {
            //Lines to write for each proc.
            //Ea line has format: <Source LP ID> <start time> <stop count>
            final List<String> lines = new ArrayList<>();
            
            //Get input file for ea proc - the LP list for ea proc
            Stream<String> lps = Files.lines(Paths.get(graphFileName + "_" + p + "_" + rank));

            lps.forEach(lp -> process(lp, lines, delim));
            
            //Output file for ea proc
            outputFileName = graphFileName + "_traffic_" + p + "_" + rank;
            System.out.printf("Writing for proc %d to file %s; line count: %d\n",
                              rank, outputFileName, lines.size());
            Files.write(Paths.get(outputFileName), lines);
        }
    }

    private void process(String lpInfo, List<String> lines, final String delim)
    {
        //Line format: <LINK ID><delim><SRCID><delim><DSTID>
        String[] toks = lpInfo.split(delim);
        Random rand = new Random(System.currentTimeMillis());
        Random timeRand = new Random(System.currentTimeMillis());
        
        int numPacket = rand.nextInt(MAX_PACKET); //Num packet on ea LP
        int numStop; //num stop for ea packet
        int startTime; //Arrival time for ea packet; TODO: make this follow poisson

        for (int i = 0; i < numPacket; ++i)
        {
            numStop = rand.nextInt(MAX_STOP);
            startTime = timeRand.nextInt(Integer.MAX_VALUE);

            //Output format (ea line): <start LP> <arrival time> <num stop>
            lines.add(String.format("%s %d %d\n", toks[0], startTime, numStop));
        }
    }
    
    /**
     * Generate traffic such that the interarrival times on each node follow a Poisson process.
     * There are two main random "step": time and space
     * 1. The src is randomly selected among the "leaf" vertices.
     * The leaf vertex is defined as those with the minimum degree.
     * 2. The src generates the arrival time such that it follows the predefined Poisson process 
     * with the given rate parameter lambda.
     * 3. The src randomly select another leaf vertex as destination.
     * 4. A shortest path between the src and the dest is computed.
     * 
     * @param fileName
     * @param maxEdge max num of edge a vertex can have to be considered leaf vertex
     * @param numInitPacket
     * @param arrivalRate a new packet will arrive every x minute
     * @throws FileNotFoundException
     * @throws IOException 
     */
    /*
    public static void genTrafficPoisson(String fileName, int maxEdge, int numInitPacket, int arrivalRate) throws FileNotFoundException, IOException
    {
        SimpleGraph<ILP, DefaultWeightedEdge> graph = constructTopology(fileName, arrivalRate);
        System.out.println("Finish constructing graph\nStart gen traffic");
        //File to write the traffic to
        FileWriter fw = new FileWriter(getPoissonTrafficFileName(fileName, numInitPacket, arrivalRate));
        String newLine = System.getProperty("line.separator");
        
        List<ILP> sources = new ArrayList<ILP>();
               
        //Only take leaf nodes - nodes with minimum degrees
        List<ILP> leafNodes = new ArrayList<ILP>();
        int minDeg = 1;
        for (ILP leaf : graph.vertexSet())
        {
            if (graph.degreeOf(leaf) <= minDeg) //Only take those with deg <= maxEdge
                minDeg = graph.degreeOf(leaf);
        }
        System.out.printf("minDeg = %d\n", minDeg);
        for (ILP leaf : graph.vertexSet())
        {
            if (graph.degreeOf(leaf) <= minDeg) //Only take those with deg <= maxEdge
                leafNodes.add(leaf);
        }
        System.out.printf("Found %d leaf nodes\n", leafNodes.size());
        
        Random rand = new Random(System.currentTimeMillis());
        ILP src, dest;
        int srcIndex, destIndex, leafCount = leafNodes.size();
        long arrivalTime;
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < numInitPacket; ++i)
        {
            //Determine src and arrival time
            srcIndex = rand.nextInt(leafCount);
            src = leafNodes.get(srcIndex);
            arrivalTime = src.nextArrival();
            
            //Determine dest
            do
            {
                destIndex = rand.nextInt(leafCount);
            }
            while (destIndex == srcIndex);
            dest = leafNodes.get(destIndex);
            
            //Generate path between src and dest
            //System.out.printf("Gen path between %d and %d; Arrival time = %d\n",
            //        src.getId(), dest.getId(), arrivalTime);
            DijkstraShortestPath<ILP, DefaultWeightedEdge> shortestPath 
                    = new DijkstraShortestPath<>(graph, src, dest);
            
            //Once the path and arrival time are determined,
            //print to file
            //Format: <T>,<SRC>,<Node1>,...,<DEST>
            //first token <T> == arrival time
            //Then the rest of the nodes
            int lastSrc = src.getId();
            int tmpId; 
            List<DefaultWeightedEdge> edges = shortestPath.getPathEdgeList();
            if (edges != null)
            {
                sb.append(arrivalTime).append(",").append(lastSrc);          
                for (DefaultWeightedEdge edge : edges)
                {
                    tmpId = graph.getEdgeSource(edge).getId();
                    if (tmpId == lastSrc)
                        tmpId = graph.getEdgeTarget(edge).getId();
                    
                    sb.append(",").append(tmpId);
                    lastSrc = tmpId;
                }
                sb.append(newLine);
                fw.write(sb.toString());
                sb.setLength(0);
            }  
        }
        fw.close();
    }
    */    
}
