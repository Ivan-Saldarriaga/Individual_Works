import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.util.*;

public class Project2_Codebase {
    //references to some variables we may want to access in a global context
    static int WIDTH = 500; //width of the image
    static int HEIGHT = 500; //height of the image
    static BufferedImage Display; //the image we are displaying
    static JFrame window; //the frame containing our window
    static int[][] globalArray;
    static int dimensions = 0;
    static Graphics2D DisplayGraphics;
    static JTextField dim;
    public static void main(String[] args) {
        //run the GUI on the special event dispatch thread (EDT)

        SwingUtilities.invokeLater(new Runnable() {
            public void run() {

                //Create the window and set options
                //The window
                window = new JFrame("RandomWalker");
                window.setPreferredSize(new Dimension(WIDTH+100,HEIGHT+50)); //sets the "ideal" window size
                window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE); //terminate program when "x" is clicked
                window.setVisible(true); //show the window
                window.pack(); //make window the preferred size


                //Display panel/image
                JPanel DisplayPanel = new JPanel();
                Display = new BufferedImage(WIDTH,HEIGHT,BufferedImage.TYPE_INT_ARGB);
                DisplayPanel.add(new JLabel(new ImageIcon(Display)));
                window.add(DisplayPanel,BorderLayout.CENTER);

                //Config panel
                JPanel Configuration = new JPanel();
                Configuration.setBackground(new Color(230,230,230));
                Configuration.setPreferredSize(new Dimension(100,500));
                Configuration.setLayout(new FlowLayout());

                //Step count input
                JLabel StepCountLabel = new JLabel("Step Count:");
                Configuration.add(StepCountLabel);

                JTextField StepCount = new JTextField("500");
                StepCount.setPreferredSize(new Dimension(100,25));
                Configuration.add(StepCount);

                //Walker type input
                JLabel WalkerType = new JLabel("Walker Type:");
                Configuration.add(WalkerType);

                ButtonGroup WalkerTypes = new ButtonGroup();//group of buttons
                JRadioButton Standard = new JRadioButton("Standard");//creates a radio button. in a ButtonGroup, only one can be selected at a time
                Standard.setActionCommand("standard");//can be grabbed to see which button is active
                Standard.setSelected(true);//set this one as selected by default
                JRadioButton Skippy = new JRadioButton("Skippy");
                Skippy.setActionCommand("skippy");
                WalkerTypes.add(Standard);//set as part of group
                WalkerTypes.add(Skippy);
                Configuration.add(Standard);//add to panel
                Configuration.add(Skippy);

                //Walker type input
                JLabel Geometry = new JLabel("World Geometry:");
                Configuration.add(Geometry);
                ButtonGroup Geometries = new ButtonGroup();
                JRadioButton Bounded = new JRadioButton("Bounded");
                Bounded.setActionCommand("bounded");
                Bounded.setSelected(true);
                JRadioButton Toroidal = new JRadioButton("Toroidal");
                Toroidal.setActionCommand("toroidal");
                Geometries.add(Bounded);
                Geometries.add(Toroidal);
                Configuration.add(Bounded);
                Configuration.add(Toroidal);

                //Path Rendering input
                JLabel Render = new JLabel("Render Style:");
                Configuration.add(Render);
                ButtonGroup Renderers = new ButtonGroup();
                JRadioButton Satellite = new JRadioButton("Satellite");
                Satellite.setActionCommand("satellite");

                Satellite.setSelected(true);
                JRadioButton Height = new JRadioButton("Height");
                Height.setActionCommand("height");


                Renderers.add(Satellite);
                Renderers.add(Height);
                Configuration.add(Satellite);
                Configuration.add(Height);


                JLabel Dimension = new JLabel("Dimension:");
                Configuration.add(Dimension);

                JTextField dim = new JTextField("50");
                dim.setPreferredSize(new Dimension(100,25));
                Configuration.add(dim);

                JButton Walk = new JButton("Walk");
                //Create Divide button
                JButton Divide = new JButton("Divide");
                Height.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        //try {
                            //dimensions = Integer.parseInt(dim.getText()); // Get the dimension from text field
                            int count = Integer.parseInt(StepCount.getText()); // Get the step count from text field
                            String render_type = Renderers.getSelection().getActionCommand();
                            generate(render_type, count);
                    }
                });
                Satellite.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        //try {
                            //dimensions = Integer.parseInt(dim.getText()); // Get the dimension from text field
                            int count = Integer.parseInt(StepCount.getText()); // Get the step count from text field
                            String render_type = Renderers.getSelection().getActionCommand();
                            // Generate the BufferedImage
                            generate(render_type, count);
                    }
                });

                Walk.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        int count = Integer.parseInt(StepCount.getText()); //gets the string from a TextField, and read it as an int
                        String walk_type = WalkerTypes.getSelection().getActionCommand();//gets the action command of which radio button is selected, a String describing the type of Walk
                        String geom_type = Geometries.getSelection().getActionCommand();
                        String render_type = Renderers.getSelection().getActionCommand();
                        dimensions = (int)Long.parseLong(dim.getText());//Get the color, convert from hex string to int

                        BufferedImage GeneratedImage = new BufferedImage(WIDTH,HEIGHT,BufferedImage.TYPE_INT_ARGB);

                        boolean skip = walk_type.equals("skippy");
                        boolean loop = geom_type.equals("toroidal");
                        //System.out.println("Before WalkFill: " + Arrays.deepToString(globalArray));
                        globalArray = WalkFill(count, GeneratedImage, skip, loop, dimensions);

                        generate(render_type, count);
                    }
                });
                Divide.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        int count = Integer.parseInt(StepCount.getText()); //gets the string from a TextField, and read it as an int
                        String walk_type = WalkerTypes.getSelection().getActionCommand();//gets the action command of which radio button is selected, a String describing the type of Walk
                        String geom_type = Geometries.getSelection().getActionCommand();
                        String render_type = Renderers.getSelection().getActionCommand();
                        divideTerrain();
                        BufferedImage GeneratedImage = new BufferedImage(WIDTH,HEIGHT,BufferedImage.TYPE_INT_ARGB);
                        dimensions *=2;
                        dim.setText(String.valueOf(dimensions));
                        KernelProcess();
                        generate(render_type, count);
                    }
                });

                Configuration.add(Walk);
                Configuration.add(Divide);
                window.add(Configuration,BorderLayout.EAST);
            }
        });
    }
    static public int[] InttoRGBA(int color){
        //take a color and seperate its colors into individual channels!
        //hex is 0xARGB and we want to seperate into RGBA
        int red = (color >> 16) & 0xFF;
        int green = (color >> 8) & 0xFF;
        int blue = (color) & 0xFF;
        //3 should be alpha channel
        int alpha = (color >> 24) & 0xFF;
        return new int []{red, green, blue, alpha};
    }
    static public int RGBAtoInt(int r, int g, int b, int a){
        //get seperates things and combine into hex ARGB or operator!
        return ((a << 24) | (r << 16) | (g << 8) | (b));

    }
    static public int[] CreateGradient(int max){
        //we have data parsed from Int to RGBA funciton
        int[] initial = InttoRGBA(0xFF000000);
        int[] ending = InttoRGBA(0xFFFFFFFF);
        //get differeces
        float redDiff = (float)(ending[0] - initial[0])/(max - 1);
        float greenDiff = (float)(ending[1] - initial[1]) / (max - 1);
        float blueDiff = (float)(ending[2] - initial[2]) / (max - 1);
        float alphaDiff = (float)(ending[3] - initial[3]) / (max - 1);
        int[] gradient = new int[max];
        //over X number of steps
        float alpha = initial[3];
        float red = initial[0];
        float green = initial[1];
        float blue = initial[2];
        for(int i = 0; i < max; i++) {
            gradient[i] = RGBAtoInt((int)red, (int)green, (int)blue, (int)alpha);
            alpha += alphaDiff;
            red += redDiff;
            green += greenDiff;
            blue += blueDiff;
        }

        return gradient;
    }
    //A method to update the display image to match one generated by you
    static void UpdateDisplay(BufferedImage img){
        //Below 4 lines draws the input image on the display image
        Graphics2D g = (Graphics2D) Display.getGraphics();
        g.setColor(Color.WHITE);
        g.fillRect(0,0,WIDTH,HEIGHT); //draw over the old image with white
        g.drawImage(img,0,0,null); //draw over the old image with the one you generated from the walk.

        //forces the window to redraw its components (i.e., the image)
        window.repaint();
    }
    static public void generate(String render_type, int count){
        BufferedImage GeneratedImage = new BufferedImage(WIDTH,HEIGHT,BufferedImage.TYPE_INT_ARGB);

        int[][] coloredPix = setColor(render_type, dimensions, count);
        //create listener for different colors

        //GeneratedImage.setRGB(WIDTH/2, HEIGHT/2, 0xFF000000);
        //terrain rendering
        float squareSize = WIDTH / (float)dimensions; // Size of each square

        DisplayGraphics = GeneratedImage.createGraphics();
        for (int x = 0; x < dimensions; x++) {
            for (int y = 0; y < dimensions; y++) {
                // Calculate the color for the current cell
                int color = coloredPix[x][y];
                DisplayGraphics.setColor(new Color(color));

                // Draw the filled square
                float squareX = x * squareSize; // Calculate the top-left corner x
                float squareY = y * squareSize; // Calculate the top-left corner y
                DisplayGraphics.fillRect((int)Math.floor(squareX), (int)Math.floor(squareY),(int)Math.ceil(squareSize), (int)Math.ceil(squareSize));
            }
        }
        //2. Update the display with the generated image
        DisplayGraphics.dispose();
        UpdateDisplay(GeneratedImage);
        window.repaint();

    }
    static public int[][] setColor( String render_type, int dimensions, int steps){
        boolean colored = !render_type.equals("height");

        int[][] output = new int[dimensions][dimensions];
        int maxVal = -1;
        for(int i = 0; i < dimensions; i++){
            for(int j = 0; j < dimensions; j++){
                if(globalArray[i][j] > maxVal){
                    maxVal = globalArray[i][j];
                }
            }
        }
        int[] grays = CreateGradient(maxVal +1);
        //System.out.println("After COLOR: " + Arrays.deepToString(globalArray));
        for(int i = 0; i < dimensions; i++){
            for(int j = 0; j < dimensions; j++){
                if(colored) {
                    //System.out.println(globalArray[i][j]);
                    if (globalArray[i][j] > maxVal * .75) {
                        output[i][j] = 0xFFE6E6FF;
                    } else if (globalArray[i][j] > maxVal * .5) {
                        output[i][j] = 0xFF644B1E;
                    } else if (globalArray[i][j] > maxVal * .1) {
                        output[i][j] = 0xFF1E961E;
                    } else if (globalArray[i][j] > maxVal * .05) {
                        output[i][j] = 0xFFFFFFC3;
                    } else {
                        output[i][j] = 0xFF1BE4FF;
                    }
                } else {
                    int num  = globalArray[i][j];
                    //System.out.println(globalArray[i][j]);
                    output[i][j] = grays[num];
                }
            }
        }
        return output;
    }
    static public void divideTerrain(){
        //first dimensions of terrain array are doubled in size
        int size = globalArray.length * 2;
        //System.out.println(globalArray.length);
        int[][] output = new int[size][size];
        for(int x = 0; x < globalArray.length; x++){
            for(int y = 0; y < globalArray.length; y++){
                //place original pixel at 2i 2j and do nearest neighbor for remaing (just keep filling basically for the quad
                output[2 * x][2 * y] = globalArray[x][y];
                //
                output[(2 * x) + 1][2 * y] = globalArray[x][y];
                output[(2 * x)][(2 * y) + 1] = globalArray[x][y];
                output[(2 * x) + 1][(2 * y) + 1] = globalArray[x][y];
            }
        }
        //nearest neighbor (basically neighbor at spot top left in quad?
        globalArray = output;
    }
    static int[][] WalkFill(int steps, BufferedImage img, boolean isSkipping, boolean isLooped, int dimensions){
        Random random = new Random();
        Map<String, Integer> map = new HashMap<>();
        //check skippy
        boolean paint = true;
        int skipNum = 0;
        int min = 0;
        int max = 500;
        // create 2d array
        int[][] pixelArray = new int[dimensions][dimensions];

        if(isSkipping){
            skipNum = random.nextInt((max-min) + 1) + min;
        }

        //initialize 2d array
        int x = dimensions/2, y = dimensions/2;
        pixelArray[x][y] = 1;

        //DO FOR ALL PIXELS AROUND BEFORE SUBMISSION
        int maxVal = 0;
        int rand = 0;
        for(int i = 1; i < steps; i++){
            if(skipNum == 0 && isSkipping){
                paint = !paint;
                skipNum = random.nextInt((max-min) + 1) + min;
            }
            //skipping
            switch (rand) {
                case 0: break;
                case 1: x++; break; // right
                case 2: y++; break; // down
                case 3: x--; break; // left
                case 4: x++; y--; break; // up-right
                case 5: x++; y++; break; // down-right
                case 6: x--; y++; break; // down-left
                case 7: x--; y--; break; // up-left
                case 8: y--; break; //up
            }
            if(x < 0){
                if(!isLooped) x = 0;
                else x = dimensions - 1;
            }
            if (x >= dimensions){
                if(!isLooped) x = dimensions - 1;
                else x = 0;
            }
            if(y < 0){
                if(!isLooped) y = 0;
                else y = dimensions -1;
            }
            if(y >= dimensions){
                if(!isLooped) y = dimensions - 1;
                else y = 0;
            }
            if(paint){
                if (x >= 0 && x < dimensions && y >= 0 && y < dimensions) {
                    pixelArray[x][y]++;
                    if (pixelArray[x][y] > maxVal) {
                        maxVal = pixelArray[x][y];
                    }
                }
                // Top-left pixel
                if (x - 1 >= 0 && y - 1 >= 0) {
                    pixelArray[x - 1][y - 1]++;
                    if (pixelArray[x - 1][y - 1] > maxVal) {
                        maxVal = pixelArray[x - 1][y - 1];
                    }
                } else if (x - 1 < 0 && y - 1 < 0 && isLooped) {
                    pixelArray[dimensions - 1][dimensions - 1]++;
                    if (pixelArray[dimensions - 1][dimensions - 1] > maxVal) {
                        maxVal = pixelArray[dimensions - 1][dimensions - 1];
                    }
                }
                // Top pixel
                if (y - 1 >= 0) {
                    pixelArray[x][y - 1]++;
                    if (pixelArray[x][y - 1] > maxVal) {
                        maxVal = pixelArray[x][y - 1];
                    }
                }else if (y - 1 < 0 && isLooped) {
                    pixelArray[x][dimensions - 1]++;
                    if (pixelArray[x][dimensions - 1] > maxVal) {
                        maxVal = pixelArray[x][dimensions - 1];
                    }
                }
                // Top-right pixel
                if (x + 1 < dimensions && y - 1 >= 0) {
                    pixelArray[x + 1][y - 1]++;
                    if (pixelArray[x + 1][y - 1] > maxVal) {
                        maxVal = pixelArray[x + 1][y - 1];
                    }
                }
                else if (x + 1 == dimensions && y - 1 < 0 &&isLooped) {
                    pixelArray[0][dimensions - 1]++;
                    if (pixelArray[0][dimensions - 1] > maxVal) {
                        maxVal = pixelArray[0][dimensions - 1];
                    }
                }
                // Right pixel
                if (x + 1 < dimensions) {
                    pixelArray[x + 1][y]++;
                    if (pixelArray[x + 1][y] > maxVal) {
                        maxVal = pixelArray[x + 1][y];
                    }
                }else if (x + 1 == dimensions && isLooped) {
                    pixelArray[0][y]++;
                    if (pixelArray[0][y] > maxVal) {
                        maxVal = pixelArray[0][y];
                    }
                }
                // Bottom-right pixel
                if (x + 1 < dimensions && y + 1 < dimensions) {
                    pixelArray[x + 1][y + 1]++;
                    if (pixelArray[x + 1][y + 1] > maxVal) {
                        maxVal = pixelArray[x + 1][y + 1];
                    }
                }else if ((x + 1 == dimensions) && (y + 1 == dimensions) && isLooped) {
                    pixelArray[0][0]++;
                    if (pixelArray[0][0] > maxVal) {
                        maxVal = pixelArray[0][0];
                    }
                }
                // Bottom pixel
                if (y + 1 < dimensions) {
                    pixelArray[x][y + 1]++;
                    if (pixelArray[x][y + 1] > maxVal) {
                        maxVal = pixelArray[x][y + 1];
                    }
                }else if (y + 1 == dimensions && isLooped) {
                    pixelArray[x][0]++;
                    if (pixelArray[x][0] > maxVal) {
                        maxVal = pixelArray[x][0];
                    }
                }
                // Bottom-left pixel
                if (x - 1 >= 0 && y + 1 < dimensions) {
                    pixelArray[x - 1][y + 1]++;
                    if (pixelArray[x - 1][y + 1] > maxVal) {
                        maxVal = pixelArray[x - 1][y + 1];
                    }
                }else if (x - 1 < 0 && y + 1 == dimensions && isLooped) {
                    pixelArray[dimensions - 1][0]++;
                    if (pixelArray[dimensions - 1][0] > maxVal) {
                        maxVal = pixelArray[dimensions - 1][0];
                    }
                }
                // Left pixel
                if (x - 1 >= 0) {
                    pixelArray[x - 1][y]++;
                    if (pixelArray[x - 1][y] > maxVal) {
                        maxVal = pixelArray[x - 1][y];
                    }
                } else if (x - 1 < 0 && isLooped) {
                    pixelArray[dimensions - 1][y]++;
                    if (pixelArray[dimensions - 1][y] > maxVal) {
                        maxVal = pixelArray[dimensions - 1][y];
                    }
                }

            }
            rand = random.nextInt(8) + 1;
            skipNum--;
        }

        return pixelArray;
    }
    static public void KernelProcess(){
        float[][] kernel = {
                {0.0625f, 0.125f,  0.0625f},
                {0.125f,  0.25f,   0.125f },
                {0.0625f, 0.125f,  0.0625f}
        };
        int originalRows = globalArray.length;
        int originalCols = globalArray[0].length;
        int kernelRows = kernel.length;
        int kernelCols = kernel[0].length;
        //considering its always going to be 3x3 kernel, you can hard code this to be 2
        int padY = kernelRows/2;
        int padX = kernelCols/2;
        int extendedRows = originalRows + 2 * padY;
        int extendedCols = originalCols + 2 * padX;
        //make new 2d array that is the size of the original PLUS edges of padding
        float[][] extendedInput = new float[extendedRows][extendedCols];
        // output array does not include the padding, it was mainly for the convolution part that is coming up
        float[][] output = new float[originalRows][originalCols];
        // fill in all the original values in the new array, skipping the padding
        for (int i = 0; i < originalRows; i++) {
            for (int j = 0; j < originalCols; j++) {
                extendedInput[i + padY][j + padX] = globalArray[i][j];
            }
        }
        // EDGE EXTENSIONS
        // Left and right side by copying the value of the inner section edge to the 2 first and 2 lasat spaces in the extended array (The padding)
        for (int i = 0; i < extendedRows; i++) {
            for (int j = 0; j < padX; j++) {
                extendedInput[i][j] = extendedInput[i][padX];
                extendedInput[i][extendedCols - j - 1] = extendedInput[i][extendedCols - padX - 1];
            }
        }
        // Top and bottom side by copying the value of the inner section edge to the 2 first and 2 lasat spaces in the extended array (The padding)
        for (int i = 0; i < padY; i++) {
            for (int j = 0; j < extendedCols; j++) {
                extendedInput[i][j] = extendedInput[padY][j];
                extendedInput[extendedRows - i - 1][j] = extendedInput[extendedRows - padY - 1][j];
            }
        }
        // do kernel process by multiplying kernel to each value centered aorund an original pixel, and adding them up to insert into the output array
        //loop through each center pixel (original array)
        for (int i = 0; i < originalRows; i++) {
            for (int j = 0; j < originalCols; j++) {
                float sum = 0;
                //loop tho=rough the kernel values to multiply each overlap with the extended array (with padding) and then add to the sum for the final output
                for (int kerneli = 0; kerneli < kernelRows; kerneli++) {
                    for (int kernelj = 0; kernelj < kernelCols; kernelj++) {
                        int xi = i + kerneli;
                        int yj = j + kernelj;
                        sum += (float)extendedInput[xi][yj] * kernel[kerneli][kernelj];
                    }
                }
                globalArray[i][j] = (int)sum;
            }
        }
        //return output;
    }

}