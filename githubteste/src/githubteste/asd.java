// im developing
package githubteste;
import java.util.Scanner;
import java.util.*;

public class asd {

	public static void main(String[] args) {
		int opcao;
		System.out.println("\n\nWelcome!!:");
		do {
			System.out.println("What do you want to do?\n");
			System.out.println("	[1] - Read a programmer joke");
			System.out.println("	[2] - Exit");
			opcao = lerInt();
			if(opcao!=2)showJoke();
		} while(opcao !=2);
	}

	public static int lerInt() {
		
		int valor=0;
		boolean valida = false;
		
		Scanner scan = new Scanner(System.in);
		
		do {
			try {
				valida = scan.hasNextInt();
				valor = scan.nextInt();
				
				if(valor<0)
					System.out.print("The value must be greater or equal to 0\n: ");
			}
			catch (InputMismatchException e) {
				System.out.print("The value must be a positive integer\n: ");
				scan.next();
			}
		} while (!valida || valor<0);
		
		return valor;
	}

	/*src: https://stackoverflow.com/questions/234075/what-is-your-best-programmer-joke*/
	public static void showJoke(){

		String[]  jokes={"Q: How do you tell an introverted computer scientist from an extroverted computer scientist?\n"
						+"A: An extroverted computer scientist looks at your shoes when he talks to you.",
					    
					    "Q: Why do programmers always mix up Halloween and Christmas?\n"
					    +"A: Because Oct 31 == Dec 25!",

					    "Knock, knock.\n"
						+"Who’s there?\n"
						+"very long pause...\n"
						+"Java.\n"
						+":-o"
					};

		int random=randInt(0,jokes.length-1);
		System.out.println("\n"+jokes[random]+"\n");

	}

	/**
	 * @source https://stackoverflow.com/questions/363681/generating-random-numbers-in-a-range-with-java/363692#363692
	 * Returns a pseudo-random number between min and max, inclusive.
	 * The difference between min and max can be at most
	 * <code>Integer.MAX_VALUE - 1</code>.
	 *
	 * @param min Minimum value
	 * @param max Maximum value.  Must be greater than min.
	 * @return Integer between min and max, inclusive.
	 * @see java.util.Random#nextInt(int)
	 */
	public static int randInt(int min, int max) {

	    // Usually this can be a field rather than a method variable
	    Random rand = new Random();

	    // nextInt is normally exclusive of the top value,
	    // so add 1 to make it inclusive
	    int randomNum = rand.nextInt((max - min) + 1) + min;

	    return randomNum;
	}
}
