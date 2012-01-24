import java.util.ArrayList;
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
	
	public static void main(String[] args){		
		Scanner sn = new Scanner(System.in);
		int tests = Integer.parseInt(sn.nextLine());
		for(int i = 0; i < tests; i++){
			ArrayList<Integer> cards = new ArrayList<Integer>();
			int n = Integer.parseInt(sn.nextLine());
			int next;
			
//			long time1 = System.nanoTime();
			
			int max = -1, min = -1;
			for(int j = 0; j < n; j++){
				next = sn.nextInt();
				if(max == -1){
					max = next;
					min = next;
				}else{
					if(next > max) max = next;
					if(next < min) min = next;
				}
				//System.out.println("Adding "+j+"th number, "+next);
				add(cards, next, 0, j);
				//System.out.println(cards.toString());
			}
			
			sort(cards, min, max);
			
////			long time2 = System.nanoTime();
//
//			
//			System.out.println("adding elements took: "+(time2-time1)+"ns");
//
//			long time3 = System.nanoTime();

			/*
			 * check that cards *can* be solved by checking each of the elements in the array - if you ever have one that is greater than its index, no can do.
			 */
			boolean nocando = false;
			for(int j = 1; j < cards.size(); j++){
				if(cards.get(j) > j) {
					nocando = true;
				}
			}
			
			if(nocando) 
				System.out.println("0");
			else{
				System.out.println(solve(cards));
			}
			if(i<tests-1) sn.nextLine();
			
//			long time4 = System.nanoTime();
//			
//			System.out.println("solving took: "+(time4-time3)+"ns");

		}
	}
	
	
	
	private static void add(ArrayList<Integer> cards, Integer newEntry, int beginindex, int endindex){
		//if(newEntry >= 9947) System.out.println("begin: "+beginindex+", end: "+endindex);
		cards.add(newEntry);
		/*
		int diff = endindex - beginindex;
		if(diff == 0){
			cards.add(beginindex, newEntry);
			return;
		}
		if(diff == 1){
			if(cards.get(beginindex) >= newEntry){
				cards.add(beginindex, newEntry);
			}else{
				cards.add(endindex, newEntry);				
			}
			return;
		}
		int avg = diff/2+beginindex;
		int check = cards.get(avg);
		
		if(check == newEntry){
			cards.add(avg, newEntry);
		}else if(newEntry > check ){
			add(cards, newEntry, avg+1, endindex);
		}else{
			add(cards, newEntry, beginindex, avg);
		}
		return;
		*/
	}
	
	
	
	
	public static long solve(ArrayList<Integer> cards){
		/*
		 * base cases:
		 * if cards is only 1 element long, it must be 0
		 * if the last element in cards is greater than the size of cards, return 0 (impossible)
		 */
		
		long result = 1;
		for(int i = cards.size(); i > 0; i--){
			if(cards.size() == 1) {
				if(cards.get(0) == 0) 
					result *= 1;
				else 
					result *= 0;
				break;
			}
			int size = cards.size();
			int diff = size - cards.remove(size-1);
			result = diff*result;
			result %= 1000000007;
		}
		return result;
//		
//				
//		if(cards.size() == 1)
//			if(cards.get(0) == 0) return 1; 
//			else return 0;
//		
//		int size = cards.size();
//		int diff = size - cards.remove(size-1);
//		long x = solve(cards)%1000000007;
//
//		return diff*solve(cards);
	}
	

	 private static ArrayList<Integer> sort(ArrayList<Integer> array, int min, int max) {
	   //the range is useful to minmize the memory usage
	   //countIntegers holds the number of each integer
	   int[] countIntegers = new int[max - min + 1];

	   for (int i = 0; i < array.size(); i++) {
	     countIntegers[array.get(i) - min]++;
	   }

	   int insertPosition = 0;
	   //fill array in sorted order
	   for (int i = min; i <= max; i++) {
	     for (int j = 0; j < countIntegers[i - min]; j++) {
	    	 array.set(insertPosition++, i);
	     }
	   }
	   return array;
	 }
	
}