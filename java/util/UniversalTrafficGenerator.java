import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.HashMap;
import java.util.stream.Stream;
import java.util.Map.Entry;
import java.nio.file.StandardOpenOption;
import java.util.Random;

/**
 * Given a graph topo file => generate traffic
 * ==> diff btw node and link should be det'ed in loader (using TrafficConverter)
 * Line output: <first stop's node id> <second stop's node id> <arrival time> <stop/hops count>
 *
 * Generate traffic for all links/nodes on all procs 
 * 
 * IMPORTANT: STRICT CONTRACT!!
 * input: dir containing files spec neighbors of ea node where the files' name is the node ID.
 * @author: Vy Thuy Nguyen
 */
public class UniversalTrafficGenerator
{
	
	private static final Map<String, List<String>> nodeMap = new HashMap<>();

	public static void main(String[] args)
		throws IOException
	{	
		String graphFile = args[0];
		String dirName = graphFile + "_node_nei"; //dir containing neighbors for nodes
		loadNeiMap(dirName); 
		genTraffic(graphFile);
	}

	/**
	 * REQ: the traffic map has already been loaded
	 * Gen traffic for ALL nodes
	 */
	private static void genTraffic(String graphFile)
		throws IOException
	{
		int maxPacket = 100; //max packet per node
		int maxStop = 10; //Number of nodes the packet goes thru => incls both ends.

		//Output file name
		String outputFileName = "uniTraffic_" + graphFile + ".txt";
		Path outFile = Paths.get(outputFileName);
		//TODO: use one rand gen for now
		//FIX THIS!!
		Random rand = new Random();

		int packetCount; 
		int stopCount;		
		int neiIdx;
		int neiCt;
		int arrTime;
		String nodeId;
		List<String> neis;

		for (Entry<String, List<String>> entry : nodeMap.entrySet())
		{
			nodeId = entry.getKey();
			neis = entry.getValue();
			neiCt = neis.size();

			//Spec the file to write to
			final List<String> lines = new ArrayList<String>();
			
			//For ea node gen some x number of packets
			// with some interarrival times
			// <src = nodeId> <next stop == vary> <arrival time> <stop count>
			packetCount = rand.nextInt(maxPacket);

			//For ea packet gen:
			//- randomly select second stop
			//- gen arrival time
			//- gen stop count
			for (int i = 0; i < packetCount; ++i)
			{
				//Second stop Idx
				neiIdx = rand.nextInt(neiCt);
		
				//Arrival time
				arrTime = rand.nextInt(Integer.MAX_VALUE);

				//Stop count
				stopCount = rand.nextInt(maxStop);

				//Create the line
				lines.add(String.format("%s %s %d %d", 
						  nodeId, neis.get(neiIdx), arrTime, stopCount));
			}
			
        	Files.write(outFile, lines, StandardOpenOption.CREATE, StandardOpenOption.APPEND);
		}
	}	

	private static void loadNeiMap(String dirName)
		throws IOException
	{
		Path dirPath = Paths.get(dirName);
		Stream<Path> files = Files.walk(dirPath);

		files.forEach(file -> 
		{
			if (!Files.isDirectory(file))
			{
				try
				{
					//Filename is the node's ID
					final String nodeId = file.getFileName().toString().split("\\.")[0];
					final List<String> neis = new ArrayList<>();
					
					nodeMap.put(nodeId, neis);
					Stream<String> lines = 	Files.lines(file);
					lines.forEach(line -> 
					{
						neis.add(line);
					});
				}
				catch (Exception exc)
				{
					System.out.printf("Unexpected prob for file %s\n", file);
				}
			}
		});
	}
	
}
