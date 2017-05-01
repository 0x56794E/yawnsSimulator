import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.HashMap;
import java.util.stream.Stream;

/**
 * Given a graph topo file => generate traffic
 * ==> diff btw node and link should be det'ed in loader
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
		loadNeiMap("g1000_5_node_nei"); 
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
					nodeMap.put(nodeId, new ArrayList<String>());
					Stream<String> lines = 	Files.lines(file);
					lines.forEach(line -> 
					{
						nodeMap.get(nodeId).add(line);
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
