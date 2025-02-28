import java.util.HashMap;

public class Project3_Functions {

    static String Generate_LSystem(int iterations, String start, HashMap<Character,String> rules){
        //TODO: Implement the L-System generator. Apply the rules to the starting string, and loop some number of times.
        StringBuilder currentString = new StringBuilder(start);
        for(int i = 0; i < iterations; i++){
            //creating a string for the new mapping/system
            StringBuilder tempString = new StringBuilder();
            //looping and assessing each characater
            for(int j = 0; j < currentString.length(); j++){
                char temp = currentString.charAt(j);
                //put in the rune from the hash!
                if(rules.containsKey(temp)){
                    //System.out.println(temp + "to " + rulemap.get(temp));
                    tempString.append(rules.get(temp));
                }else{
                    //System.out.println(temp);
                    tempString.append(temp);
                }
            }
            //System.out.println(currentString);
            currentString = tempString;
        }
        //draw system

        return currentString.toString(); //Return value included so code compiles.
    }

//    public static void main(String[] args) {
//        String start = "*A*>B*";
//        HashMap<Character, String> rules = new HashMap<>();
//        rules.put('A', "A*>B");
//        rules.put('B', "BB");
//
//        // Test the L-System with 1, 2, and 3 iterations
//        int iterations = 4;
//
//        String result = Generate_LSystem(iterations, start, rules);
//        System.out.println(result);
//        String expected = "*A*>B*>BB*>BBBB*>BBBBBBBB*>BBBBBBBBBBBBBBBB*";
//        if(result.equals(expected)){
//            System.out.println(true);
//        }else{
//            System.out.println("false");
//        }
//    }
}
