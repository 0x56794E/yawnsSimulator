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
	{	
	
	}
	

	private static void doGen(String dirName)
		throws IOException
	{
		//Load each node's neighbors
		try (Stream<Path> paths = Files.walk(Paths.get(dirName)))
		{
			paths.forEach(filePath -> {
				if (Files.isRegularFile(filePath))
				{
					filePath.forEach(line -> {
						System.out.println(line);
					});
				}		

				System.out.println("DONE WITH ONE FILE");
				System.exit(1);	
			});
		}
	}
	
}
