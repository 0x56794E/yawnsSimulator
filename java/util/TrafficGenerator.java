
import sim.pojo.ILP;
import sim.enums.LPType;

import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Random;
import org.jgrapht.alg.DijkstraShortestPath;
import org.jgrapht.graph.DefaultWeightedEdge;
import org.jgrapht.graph.SimpleGraph;
import sim.Simulator;
import sim.pojo.impl.SimpleLP;

/**
 * Created by VyNguyen on 2/22/2017
 */
public class TrafficGenerator
{    
    public static final String Poisson_Traffic_FileName_Format = "PoissonTraffic_%s_%d_%d%s";
    
    public static void main(String[] args) throws IOException
    { 
        String fileName = "pl_g50000_3"; //pl_g<node count>_<power law deg/lambda>
        int numPacket = 5000;
        int arrivalRate = 2; //lambda = 1/arrivalRate; (unit time)
        int maxEdge = 20;
        TrafficGenerator.genTraffic(fileName, maxEdge, numPacket, arrivalRate);
    }
    
    public static String getPoissonTrafficFileName(String topologyFileName, int numPacket, int arrivalRate)
    {
        //TODO: if not exist, generate
        return String.format(Poisson_Traffic_FileName_Format, topologyFileName, numPacket, arrivalRate, ".csv");
    }
    
    public static String getResultDirectory(String topologyFileName, int numPacket, int arrivalRate)
    {        
        return String.format(Poisson_Traffic_FileName_Format, topologyFileName, numPacket, arrivalRate, "Result");
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
    public static void genTraffic(String fileName, int maxEdge, int numInitPacket, int arrivalRate) throws FileNotFoundException, IOException
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
    
    public static SimpleGraph<ILP, DefaultWeightedEdge> constructTopology(String fileName, int arrivalRate) throws FileNotFoundException, IOException
    {
        //A simple undirected graph.
        //At most one edge exist btw any two vertices
        SimpleGraph<ILP, DefaultWeightedEdge> graph = new SimpleGraph<ILP, DefaultWeightedEdge>(DefaultWeightedEdge.class);
        
        BufferedReader br = new BufferedReader(new FileReader(fileName));
        String[] toks;
        int id1, id2;
        ILP lp1, lp2;

        //Construct the LPs
        //Line format: ID of node #1, ID of node #2
        //Each line reps connectivity
        for (String line = br.readLine(); line != null; line = br.readLine())
        {
            System.out.println(line);
            toks = line.split(",");
            id1 = Integer.parseInt(toks[0].trim());
            id2 = Integer.parseInt(toks[1].trim());

            lp1 = new SimpleLP(id1, arrivalRate);
            lp2 = new SimpleLP(id2, arrivalRate);
            
            //Since the check is done automatically, just add the vertices
            graph.addVertex(lp1);
            graph.addVertex(lp2);
            DefaultWeightedEdge e = graph.addEdge(lp1, lp2);
            graph.setEdgeWeight(e, 1);
        }
        
        return graph;
    }
    
    //=======================================
    ///OLD ALGORITHM
    //Packets make random walk thru the network
    public static final String RW_Traffic_FileName_Format = "RandomWalkTraffic_%s_%d_%d_%s.csv";    
    public static final int MAX_VALUE = 5000; //Max unit time
    
    /**
     * Generate Traffic and Export to file
     * Each line reps a packet and contains a list of IDs of the LPs the packet to traverse:
     * [src ID] [dest 1] [dest 2] ... [dest n, n <= maxHops]
     * @param numPackets number of packets to generate
     * @param maxHop maximum number of edges a packet can go thru, inclusive
     * @param fileName name of the file describing the network
     */
    public static void genTraffic(int numPackets, int minHop, int maxHop, String fileName, long transTime, long switchTime, Simulator simulator) throws IOException
    {
        genTraffic(LPType.getNodeTopology(fileName, transTime, switchTime, simulator), numPackets, minHop, maxHop, fileName);
    }

    public static void genTraffic(Map<Integer, ILP> lpMap, int numPackets, int minHop, int maxHop, String fileName) throws IOException
    {
        Random rand = new Random();
        StringBuilder sb = new StringBuilder();
        int maxLPId = lpMap.size();
        FileWriter fw = new FileWriter(String.format(RW_Traffic_FileName_Format, numPackets, minHop, maxHop, fileName));
        String newLine = System.getProperty("line.separator");

        int hopCount, curLPId;
        ILP lp;

        for (int i = 0; i < numPackets; ++i)
        {
            //Gen the start time
            sb.append(rand.nextInt(MAX_VALUE)).append(",");

            //Gen the first stop
            curLPId = rand.nextInt(maxLPId);
            sb.append(curLPId);
            lp = lpMap.get(curLPId);

            hopCount = lp.genHopCount(maxHop, minHop);

            //Gen the rest of the stops
            for (int j = 1; j <= hopCount; ++j)
            {
                curLPId = lp.nextNeiId(); //rand choose a neighbor as next stop
                sb.append(",").append(curLPId);
                lp = lpMap.get(curLPId);
            }
            sb.append(newLine);
            fw.write(sb.toString());
            sb.setLength(0);
        }
        fw.close();
    }

    //Traffic is generated based on NODE oriented model.
    //The stop IDs are those of the node NOT the link.
    //Need to be able to tell which link corresp. with which node
    public static void loadTraffic(String fileName, Map<Integer, ILP> lpMap, LPType type, int packetCount, int minHop, int maxHop) throws IOException
    {
        BufferedReader br = new BufferedReader(new FileReader(String.format(RW_Traffic_FileName_Format, packetCount, minHop, maxHop, fileName)));
        String[] toks;

        for (String line = br.readLine(); line != null; line = br.readLine())
        {
            toks = line.split(",");
            ArrayList<Integer> stops = new ArrayList<Integer>();
            for (int i = 1; i < toks.length; ++i)
            {
                stops.add(Integer.parseInt(toks[i]));
            }
        }
    }

    /*
    private void scheduleInitialEvents()
    {
        //Schedule x (20 max) initial events on n LPs
        Random rand = new Random();
        int mapSize = lpMap.size();
        int n = rand.nextInt(mapSize);
        int lpId, x;
        ILP lp;
        for (int i = 0; i < n; ++i)
        {
            lpId = rand.nextInt(mapSize);
            x = rand.nextInt(21);
            lp = lpMap.get(lpId);
            for (int j = 0; j < x; ++j)
                lp.scheduleEvent(new MaxStopAwareEvent(rand.nextLong(), EventType.ARRIVAL, rand.nextInt(mapSize)));
        }
    }
    */
}
