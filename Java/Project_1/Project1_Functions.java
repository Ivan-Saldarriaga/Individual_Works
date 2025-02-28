

public class Project1_Functions {
    //Required function
    static public int ARGBtoInt(int r, int g, int b, int a){
        //get seperates things and combine into hex ARGB or operator!
        return ((a << 24) | (r << 16) | (g << 8) | (b));

    }

    //Required function
    static public int[] InttoARGB(int color){
        //take a color and seperate its colors into individual channels!
        //hex is 0xARGB and we want to seperate into RGBA
        int red = (color >> 16) & 0xFF;
        int green = (color >> 8) & 0xFF;
        int blue = (color) & 0xFF;
        //3 should be alpha channel
        int alpha = (color >> 24) & 0xFF;
        return new int []{alpha, red, green, blue};
    }

    //Required function
    static public int[] CreateGradient(int GradientStartColor, int GradientEndColor, int steps){
        //we have data parsed from Int to RGBA funciton
        int[] initial = InttoARGB(GradientStartColor);
        int[] ending = InttoARGB(GradientEndColor);
        //get differeces
        float redDiff = (float)(ending[1] - initial[1])/(steps - 1);
        float greenDiff = (float)(ending[2] - initial[2]) / (steps - 1);
        float blueDiff = (float)(ending[3] - initial[3]) / (steps - 1);
        float alphaDiff = (float)(ending[0] - initial[0]) / (steps - 1);
        int[] gradient = new int[steps];
        //over X number of steps
        float alpha = initial[0];
        float red = initial[1];
        float green = initial[2];
        float blue = initial[3];
        for(int i = 0; i < steps; i++) {
            gradient[i] = ARGBtoInt((int)red, (int)green, (int)blue, (int)alpha);
            alpha += alphaDiff;
            red += redDiff;
            green += greenDiff;
            blue += blueDiff;
        }

        return gradient;
    }
    //FOR TESTING!!!
//    public static void main(String[] args) {
//        // Input start and end colors
//        int[] gradient = CreateGradient(0xFFAABBCC, 0xFF000000, 50);
//        for (int i : gradient){
//            System.out.printf("0x%08X, ", i);
//
//        }
//
//    }
}
