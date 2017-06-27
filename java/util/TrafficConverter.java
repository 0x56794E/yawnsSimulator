import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.io.IOException;

import java.util.stream.Stream;
import java.util.*;

/**
 * Split the traffic file into
 * (0) File for diff procs
 * (1) Diff btw link and node models
 */
public class TrafficConverter
{
	//VERY hacky but desperate times...
	private static class Counter
	{
		private int idx = 0;

		public void inc()
		{
			++idx;
		}

		public int getIdx()
		{
			return idx;
		}
	}

	private static class TrafficContainer
	{
		public final int linkId; //link ID => line number
		public final String node1;
		public final String node2;

		private List<String> traff = new ArrayList<>();
		public TrafficContainer(int linkId, String n1, String n2)
		{
			this.linkId = linkId;
			this.node1 = n1;
			this.node2 = n2;
		}

		public void addTraffic(String str)
		{
			traff.add(str);
		}

		public List<String> getTraffic()
		{
			return traff;
		}
	}

	//Key: <node1><space><node2>
	//Val: TrafficContainer
	private static Map<String, TrafficContainer> linkIDMap = new HashMap<>();
	private static final Counter counter = new Counter();

	//Input: traffic file name + p count
	public static void main(String[] args) throws Exception
	{
		//Assuming there exists file gen'ed by UniversalTrafficGenerator
		//This file has name: uniTraffic_<graphFile>.txt

		String graphFile = args[0];
		int p = Integer.parseInt(args[1]);

		toLinkTraffic(p, graphFile);
		toNodeTraffic(p, graphFile);
	}

	private static void constrIDMap(String topoFile)
		throws IOException
	{
		Path file = Paths.get(topoFile);

		Stream<String> lines = Files.lines(file);
		lines.forEach(line -> {
			String[] toks = line.split("\\s+");
			//Load the map
			linkIDMap.put(String.format("%s %s", toks[0], toks[1]), 
						 new TrafficContainer(counter.getIdx(), toks[0], toks[1]));
			counter.inc();
		});
	}

	private static String getUniversalTrafficFileName(String graphFileName)
	{
		return "uniTraffic_" + graphFileName + ".txt";
	}

	/**
	 * Generate p files ea file containing the traffic data for that proc
     * Requirement: LP-on-ea-proc files have already been generated
	 * The traffic file is gen'ed by UniversalTraffGen.
     * Ea line containing
     *  <start node id> <next node id> <start time> <stop count == num node>
	 */
	private static void toLinkTraffic(int p, String graphFileName)
		throws IOException
	{	
		String trafficFile = getUniversalTrafficFileName(graphFileName);
		String outputFileName;
		//Read the topo file to construct the ID map
		constrIDMap(graphFileName);
		
		//Read from the traffic file
		Path file = Paths.get(trafficFile);	

		Stream<String> rawTrafflines = Files.lines(file);

		//Ea line's format: <start node id> <next node id> <start time> <stop count == num node>
		//Want: <start link ID> <last node id> <start time> <stop count>
		rawTrafflines.forEach(line -> {
			String[] toks = line.split("\\s+");
					
			TrafficContainer cont = linkIDMap.get(String.format("%s %s", toks[0], toks[1]));
			if (cont == null)
				cont = linkIDMap.get(String.format("%s %s", toks[1], toks[0]));
			cont.addTraffic(String.format("%d %s %s %s", 
								cont.linkId, toks[0], toks[2], toks[3]));
			
		});

		//TODO: determine the proc ea LP belongs to => write to appropriate file
		//At this point, we have a map where ea link has all their initial traffic
		for (int rank = 0; rank < p; ++rank)
        {
            //Lines to write for each proc.
            //Ea line has format: <Source LP ID> <start time> <stop count>
            final List<String> outputlines = new ArrayList<>();
            
            //Get input file for ea proc - the LP list for ea proc
            Stream<String> lps = Files.lines(Paths.get("link_" + graphFileName + "_" + p + "_" + rank));

            lps.forEach(lp -> processLink(lp, outputlines));
            
            //Output file for ea proc
            outputFileName = "link_" + graphFileName + "_traffic_" + p + "_" + rank;
            Files.write(Paths.get(outputFileName), outputlines);
        }
	}

    private static void processLink(String lpInfo, List<String> lines)
    {
        //Line format: <LINK ID><delim><SRCID><delim><DSTID>
        String[] toks = lpInfo.split("\\s+");
        
		//Get add all traffic to lines
		TrafficContainer cont = linkIDMap.get(String.format("%s %s", toks[1], toks[2]));
		if (cont == null)
			cont = linkIDMap.get(String.format("%s %s", toks[2], toks[1]));

		if (cont != null)
			lines.addAll(cont.getTraffic());
    }

	/**
     * Simply remove next stop?
	 * Ea line's format: <start node id> <next node id> <start time> <stop count == num node>
	 * Want: <start node ID> <last visited node id> <start time> <stop count>
	 */
	private static void toNodeTraffic(int p, String graphFileName)
		throws IOException
	{
		String trafficFile = getUniversalTrafficFileName(graphFileName); 
		String outputFileName;

		//Categorize lines into the right bin
		//key: start node Id
		//val: List<String> trafficLines
		Map<String, List<String>> trafMap = new HashMap<>();

		Stream<String> trafLines = Files.lines(Paths.get(trafficFile));
		trafLines.forEach(line -> {
			String[] toks = line.split("\\s+");

			if (!trafMap.containsKey(toks[0]))
				trafMap.put(toks[0], new ArrayList<String>());

			trafMap.get(toks[0]).add(String.format("%s %s %s %s", 
					toks[0], toks[0], toks[2], toks[3]));
		});

		for (int rank = 0; rank < p; ++rank)
		{
			//Lines to write for each proc.
            //Ea line has format: <Source LP ID> <start time> <stop count>
            final List<String> outputlines = new ArrayList<>();
            
            //Get input file for ea proc - the LP list for ea proc
			//ea line is the node id
            Stream<String> lps = Files.lines(Paths.get("node_" + graphFileName + "_" + p + "_" + rank));
			lps.forEach(lpId -> {
				if (trafMap.containsKey(lpId.trim()))
					outputlines.addAll(trafMap.get(lpId.trim()));
			});

            //Output file for ea proc
            outputFileName = "node_" + graphFileName + "_traffic_" + p + "_" + rank;
            Files.write(Paths.get(outputFileName), outputlines);
			
		}
	}
	
}
