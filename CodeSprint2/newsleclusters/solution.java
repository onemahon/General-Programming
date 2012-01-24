import java.util.ArrayList;
import java.util.PriorityQueue;
import java.util.Scanner;

public class Solution {
	/*
	 * DEAR REVIEWER:
	 * 
	 * I would just like to note that due to the restricted time limit of this event, I know this will not be the neatest code I would normally write, especially when it comes to lack of comments or unclear variable names.
	 * Also, although I ensure that this is my own work, I would also mention that if I used methods I learned elsewhere, I have cited them in the comments.
	 * 
	 * Thanks!
	 */
	
	
	
	
	private static final float THRESHOLD = 0.5f; //the value needed for things to be considered related
	
	public static void main(String[] args){
		/*
		 * convert the JSON into an array list
		 */
		
		Scanner sn = new Scanner(System.in);
		String json = sn.nextLine();
				
		ArrayList<String[]> articles = new ArrayList<String[]>();
		String[] articlesStrings = json.split("[{|}]");
		
		int numarticle = 0;
		for(int i = 0; i < articlesStrings.length; i++){
			if(articlesStrings[i].equals("[")  || articlesStrings[i].equals("[ ")  || articlesStrings[i].equals("]") || articlesStrings[i].equals(",") || articlesStrings[i].equals(", ") || articlesStrings[i].equals(" ")){ continue; }
			
			//find the id first:
			int a = articlesStrings[i].indexOf("\"id\": ");
			int b = articlesStrings[i].indexOf(", \"", a);
			if (b == -1 ) b = articlesStrings[i].length();
			
			//add that substring as a piece
			String[] arr = new String[3];
			arr[0] = articlesStrings[i].substring(a+6, b);
			
			//find the title next:
			a = articlesStrings[i].indexOf("\"title\": \"");
			b = articlesStrings[i].indexOf("\", \"", a);
			if (b == -1 ) b = articlesStrings[i].length()-1;
			
			//add that substring as a piece
			arr[1] = articlesStrings[i].substring(a+10, b);
			
			//find the content next:
			a = articlesStrings[i].indexOf("\"content\": \"");
			b = articlesStrings[i].indexOf("\", \"", a);
			if (b == -1 ) b = articlesStrings[i].length()-1;
			
			//add that substring as a piece
			arr[2] = articlesStrings[i].substring(a+12, b);
			
			//now add this article to the arraylist
			numarticle++;
			articles.add(arr);	
		}
		//at this point, all the articles should exist organized into the array list
		
		
		/*
		 * now take the array list and determine their relation values
		 */
		
		/*
		 * create arraylist<linkedlist> called "clusters"
		 * 
		 * foreach article
		 * 	add a linkedlist<String[]> containing just this article in a new slot in "clusters"
		 * 	foreach OTHER article
		 * 		if relation between article x and article y is high enough, remove article y from "articles", and add it to the linkedlist within clusters
		 * 
		 */
		
		ArrayList<PriorityQueue> clusters = new ArrayList<PriorityQueue>();
		
		for(int i = 0; i < articles.size(); i++){
			PriorityQueue<Integer> slot = new PriorityQueue<Integer>();
			
			String[] t = articles.get(i);
			slot.add(Integer.parseInt(t[0]));
			
			for(int j = 0; j < articles.size(); j++){
				if(j == i) continue;
				if(compareArticles(articles.get(i), articles.get(j)) >= THRESHOLD){
					String[] u = articles.get(j);
					slot.add(Integer.parseInt(u[0]));
					articles.remove(j);
				}
			}
			
			clusters.add(slot);
		}
		
		//organize clusters now:
		for(int i = 1; i < clusters.size(); i++){
			PriorityQueue<Integer> later = clusters.get(i);
			PriorityQueue<Integer> earlier = clusters.get(i-1);
			if(later.peek() < earlier.peek()){
				//if the later one is less than an earlier one, go backwards through clusters until it is not, then insert it there
				boolean gotit = false;
				for(int j = i-1; j >= 0; j--){
					PriorityQueue<Integer> cmp = clusters.get(j);
					if(later.peek() > cmp.peek()){
						clusters.remove(i);
						clusters.add(j+1,later);
						gotit = true;
						break;
					}
				}
				if(!gotit){
					clusters.remove(i);
					clusters.add(0, later);
				}
			}
		}
		
		System.out.print("[");
		for(int i = 0; i < clusters.size(); i++){
			PriorityQueue s = clusters.get(i);
			System.out.print("[");
			while(s.peek() != null){
				System.out.print(s.poll());
				if(s.peek() != null) System.out.print(", ");
			}
			System.out.print("]");
			
			if(i != clusters.size()-1 ) System.out.print(", ");
		}
		System.out.print("]\n");
		
	}
	
	private static double compareArticles(String[] art1, String[] art2){
		double res = 0;
		res = 0.5*compareStrings(art1[1], art2[1]) + 0.5*compareStrings(art1[2], art2[2]);
		return res;
	}
	
	
	
	/*
	 * methods for determining the relationship between strings is from:
	 * http://www.catalysoft.com/articles/StrikeAMatch.html
	 */
	private static double compareStrings(String str1, String str2){
		ArrayList pairs1 = wordLetterPairs(str1.toUpperCase());
		ArrayList pairs2 = wordLetterPairs(str2.toUpperCase());
		int intersection = 0;
		int union = pairs1.size() + pairs2.size();
		for (int i=0; i<pairs1.size(); i++) {
			 Object pair1=pairs1.get(i);
			 for(int j=0; j<pairs2.size(); j++) {
				  Object pair2=pairs2.get(j);
				  if (pair1.equals(pair2)) {
						intersection++;
						pairs2.remove(j);
						break;
				  }
			 }
		}
		return (2.0*intersection)/union;
	}
	
	private static ArrayList wordLetterPairs(String str) {
		 ArrayList allPairs = new ArrayList();
		 // Tokenize the string and put the tokens/words into an array
		 String[] words = str.split("\\s");
		 // For each word
		 for (int w=0; w < words.length; w++) {
			  // Find the pairs of characters
			 if(words[w].equals("")) continue;
			  String[] pairsInWord = letterPairs(words[w]);
			  for (int p=0; p < pairsInWord.length; p++) {
					allPairs.add(pairsInWord[p]);
			  }
		 }
		 return allPairs;
	}
	
	private static String[] letterPairs(String str) {
		 int numPairs = str.length()-1;
		 String[] pairs = new String[numPairs];
		 for (int i=0; i<numPairs; i++) {
			  pairs[i] = str.substring(i,i+2);
		 }
		 return pairs;
	}
	

}