import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.io.IOException;
import java.util.stream.Stream;

/**
 * Given a graph topo file => generate traffic
 * ==> diff btw node and link should be det'ed in loader
 * Line output: <first stop's node id> <second stop's node id> <arrival time> <stop/hops count>
 *
 * Generate traffic for all links/nodes on all procs 
 *
 * @author: Vy Thuy Nguyen
 */
public class UniversalTrafficGenerator
{
	public static void main(String[] args)
		throws IOException
	{	
		doGen("g1000_5_node_nei/"); 
	}
	

	private static void doGen(String dirName)
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

					Stream<String> lines = 	Files.lines(file);
					lines.forEach(line -> 
					{
						System.out.println(line);
					});
				}
				catch (IOException ioe)
				{
					System.out.printf("IOE for file %s\n", file);
				}
			}
		});
	}
	
}
