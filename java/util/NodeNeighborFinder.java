import java.nio.file.Files;
import java.nio.file.Paths;
import java.io.IOException;
import java.io.File;

import java.util.stream.Stream;
import java.util.Map;
import java.util.HashMap;
import java.util.Set;
import java.util.HashSet;

/**
 * WILL REFACTOR and GROUP WITH NeighborFinder!!!
 * @author Vy Thuy Nguyen
 */
public class NodeNeighborFinder
{
	private final String fileName;
	private final String delim;
	
	//Key: node ID, value: IDs of nodes sharing a link with key
	private Map<String, Set<String>> neiMap = new HashMap<>();
	
	private NodeNeighborFinder(String fileName, String delim)
	{
		this.fileName = fileName;
		this.delim = delim;
	}
	
	public static void main (String[] args)
				throws Exception
	{
		if (args.length < 2)
        {
            System.out.println("Usage: java NodeNeighborFinder <graph file name> <delim>");
            System.exit(1);                
        }
		
		NodeNeighborFinder finder = new NodeNeighborFinder(args[0], args[1]);
		finder.exec();
				
	}
	
	private void exec() throws IOException
	{
        Stream<String> lines = Files.lines(Paths.get(fileName));
        lines.forEach(line -> process(line));
        writeToFile();
	}
	
	/**
	 * Ea line: <node A> <node B>
	 */
	private void process (String line)
	{
		String toks[] = line.split(delim);
		
		//Add node B as neighbor of node A
		if (!neiMap.containsKey(toks[0]))
			neiMap.put(toks[0], new HashSet<String>());
		
		neiMap.get(toks[0]).add(toks[1]);
		
		
		//Add node A as neighbor of node B
		if (!neiMap.containsKey(toks[1]))
			neiMap.put(toks[1], new HashSet<String>());
		
		neiMap.get(toks[1]).add(toks[0]);
	}
	
	/**
	 * Write neighbor set to file.
	 * Ea file contains the neighbors of ONE LP
	 * File name format: <input_file_name>_<node_id>.txt
	 * Line format: <nei/node_id>
	 */
	private void writeToFile() throws IOException
	{
		//Make dir to store these files
		String dirName = fileName +  "_node_nei";
		new File(dirName).mkdir();
		
		for (Map.Entry<String, Set<String>> entry : neiMap.entrySet())
		{  
			Files.write(Paths.get(dirName + "/" + entry.getKey() + ".txt"), 
					    entry.getValue());
	    }
	}
}