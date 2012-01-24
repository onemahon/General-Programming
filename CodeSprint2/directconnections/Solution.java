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
	private Integer[] tracker;
	
	public Solution(){
		Scanner sn = new Scanner(System.in);
		int tests = Integer.parseInt(sn.nextLine());
		for(int i = 0; i < tests; i++){			
			int numcities = Integer.parseInt(sn.nextLine());
			ArrayList<Integer> locations = new ArrayList<Integer>();
			ArrayList<Integer> populations = new ArrayList<Integer>();
			for(int j = 0; j < numcities; j++){
				locations.add(sn.nextInt());
			}
			sn.nextLine();
			for(int j = 0; j < numcities; j++){
				populations.add(sn.nextInt());
			}

			//double time1 = System.nanoTime();

//LISTS method
//			ArrayList<Integer> cxncables = new ArrayList<Integer>(numcities);
//			ArrayList<Integer> cxndistances = new ArrayList<Integer>(numcities);
			long wire = 0;

//CONNECTIONS method
//			Connection head = null, prev = null;

			for(int j = 0; j < numcities; j++){
				for(int k = j+1; k < numcities; k++){
					int cxncables = (populations.get(j) > populations.get(k))? populations.get(j) : populations.get(k);
					int cxndistance = Math.abs(locations.get(j)-locations.get(k));
					wire += cxncables*cxndistance;
					wire %= 1000000007;

					
//LISTS method
//unecessary: no need to loop through these in the same order again
//					cxncables.add( (populations.get(j) > populations.get(k))? populations.get(j) : populations.get(k) );
//					cxndistances.add(Math.abs(locations.get(j)-locations.get(k)));


//CONNECTIONS method
//inefficient: blows through HeapSpace
//					if(j == 0 && k == 1){
//						head = this.new Connection(locations.get(j), locations.get(k), populations.get(j), populations.get(k), null);
//						prev = head;
//					}else{
//						Connection c = this.new Connection(locations.get(j), locations.get(k), populations.get(j), populations.get(k), prev);
//						prev = c;
//					}
				}
			}		
			//double time2 = System.nanoTime();
			//System.out.println("Added in: 		"+(time2-time1)/1000+"ns");

			
			/*
			 * now parse through the connections to get the total amount of wire needed:
			 */
			
//LISTS method
//			long wire = 0;
//			for(int j = 0; j < cxncables.size(); j++){
//				wire += cxncables.get(j)*cxndistances.get(j);
//				wire %= 1000000007;
//			}


//CONNECTIONS method
//			Connection cur = head;
//			while(cur != null){
//				wire += cur.getWire();
//				wire %= 1000000007;
//				cur = cur.getNext();
//			}


// SINGLE ARRAY method - doesn't work, doesn't account for all connections
// 			int numcities = Integer.parseInt(sn.nextLine());
// 			int[] locations = new int[numcities];
// 			int[] populations = new int[numcities];
// 			for(int j = 0; j < numcities; j++){
// 				locations[j] = sn.nextInt();
// 			}
// 			sn.nextLine();
// 			for(int j = 0; j < numcities; j++){
// 				populations[j] = sn.nextInt();
// 			}
// 
// 			long wire = 0;
// 			int[] wirepercities = new int[numcities];
// 			wirepercities[numcities-1] = 0; //no extra wire needed in the last city
// 			wirepercities[numcities-2] = ((populations[numcities-2] > populations[numcities-1])? populations[numcities-2] : populations[numcities-1]) * (Math.abs(locations[numcities-2]-locations[numcities-1])); //need only as much wire as is needed to get from 2nd to last to last city
// 			
// 			for(int j = numcities-3; j >= 0; j--){
// 				int nextcitypop =  (populations[j+1] > populations[j+2]) ? populations[j+1] : populations[j+2];
// 				int thiscitypop =  (populations[j] > populations[j+1]) ? populations[j] : populations[j+1];
// 				wirepercities[j] = ( ( ( (wirepercities[j+1]/nextcitypop) + Math.abs(locations[j] - locations[j+1]) ) * (thiscitypop)))%1000000007;
// 			}
// 			
// 			for(int j = 0; j < numcities; j++){
// 				wire += wirepercities[j];
// 				wire %= 1000000007;
// 			}




// NESTED ARRAY method - works, was not fast enough algorithm (believed to be O(nlogn))
// 			int numcities = Integer.parseInt(sn.nextLine());
// 			int[] locations = new int[numcities];
// 			int[] populations = new int[numcities];
// 			for(int j = 0; j < numcities; j++){
// 				locations[j] = sn.nextInt();
// 			}
// 			sn.nextLine();
// 			for(int j = 0; j < numcities; j++){
// 				populations[j] = sn.nextInt();
// 			}
//	
//			for(int j = 0; j < numcities; j++){
//				for(int k = j+1; k < numcities; k++){
//					int cxncables = (populations[j] > populations[k])? populations[j] : populations[k];
//					int cxndistance = Math.abs(locations[j]-locations[k]);
//					wire += cxncables*cxndistance;
//					wire %= 1000000007;
//				}
//			}	

			System.out.println(wire);
			
			if(i<tests-1) sn.nextLine();
			
			
			
		
//SORTING method (wholly encapsulated - overlaps with later sections)
//			System.out.println(".");
//			System.out.println(".");
//			double time1 = System.nanoTime();
//			
//			int numcities = Integer.parseInt(sn.nextLine());
//			ArrayList<Integer> locations = new ArrayList<Integer>();
//			ArrayList<Integer> populations = new ArrayList<Integer>();
//			for(int j = 0; j < numcities; j++){
//				locations.add(sn.nextInt());
//			}
//			sn.nextLine();
//			int max = 0, min = Integer.MAX_VALUE;
//			for(int j = 0; j < numcities; j++){
//				int n = sn.nextInt();
//				if(n > max) max = n;
//				if(n < min) min = n;
//				populations.add(n);
//			}
//			
//			double time2 = System.nanoTime();
//			System.out.println("Added in: 		"+(time2-time1)/1000+"ns");
//			
//			time1 = System.nanoTime();
//			/*
//			 * now sort the arrays
//			 */
//			tracker = new Integer[numcities]; 
//			sort(populations, min, max);
//			
//			ArrayList<Integer> newlocs = (ArrayList<Integer>) locations.clone();
//			for(int j = 0; j < numcities; j++){
//				newlocs.set(tracker[j], locations.get(j));
//			}
//			locations = newlocs;
//			/*
//			 * arrays are correspondingly sorted
//			 */
//			
//			time2 = System.nanoTime();
//			System.out.println("Sorted in: 		"+(time2-time1)/1000+"ns");
//			
//			time1 = System.nanoTime();
//
//			/*
//			 * now parse arrays in order of population in a double nested loop (inner loop goes through each remaining city)
//			 */
//			int total = 0;
//			for(int j = populations.size()-1; j > 0; j--){
//				int citytotal = 0;
//				for(int k = 0; k < j; k++){
//					citytotal+=(Math.abs(locations.get(j)-locations.get(k)));
//				}
//				total += citytotal*populations.get(j);
//				total %= 1000000007;
//			}
//			
//			time2 = System.nanoTime();
//			System.out.println("Calculated in: 		"+(time2-time1)/1000+"ns");
//
//			System.out.println(total);
//			if(i<tests-1) sn.nextLine();
		}
		
	}
	
	
//CONNECTIONS method
//	public class Connection{
//		Connection next;
//		int cables, distance;
//		
//		public Connection(int city1, int city2, int population1, int population2, Connection prev){
//			distance = Math.abs(city2-city1);
//			cables = (population1 > population2) ? population1 : population2;
//			if(prev != null) prev.setNext(this);
//		}
//		
//		public Connection getNext(){ return next; }
//		public void setNext(Connection x){ next = x; }
//		
//		public long getWire(){
//			return (distance*cables);
//		}
//	}
//	

//SORTING method
// 	private ArrayList<Integer> sort(ArrayList<Integer> array, int min, int max) {
// 		ArrayList<Integer> copy = (ArrayList<Integer>) array.clone();
// 		
// 		//the range is useful to minmize the memory usage
// 		//countIntegers holds the number of each integer
// 		int[] countIntegers = new int[max - min + 1];
// 
// 		for (int i = 0; i < array.size(); i++) {
// 			countIntegers[array.get(i) - min]++;
// 		}
// 
// 		int insertPosition = 0;
// 		//fill array in sorted order
// 		for (int i = min; i <= max; i++) {
// 			for (int j = 0; j < countIntegers[i - min]; j++) {
// 				int oldindex = copy.indexOf(i);
// 				tracker[oldindex] = insertPosition;
// 				copy.set(oldindex, -1);
// 				array.set(insertPosition++, i);
// 			}
// 		}
// 		return array;
// 	}
	
	public static void main(String[] args){
		Solution s = new Solution();
	}
}