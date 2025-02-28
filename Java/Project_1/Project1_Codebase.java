import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.util.HashMap;
import java.util.Map;
import java.util.Random;

public class Project1_Codebase {
    //references to some variables we may want to access in a global context
    static int WIDTH = 500; //width of the image
    static int HEIGHT = 500; //height of the image
    static BufferedImage Display; //the image we are displaying
    static JFrame window; //the frame containing our window


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
                JLabel Render = new JLabel("Path Render:");
                Configuration.add(Render);
                ButtonGroup Renderers = new ButtonGroup();
                JRadioButton Black = new JRadioButton("Black");
                Black.setActionCommand("black");

                Black.setSelected(true);
                JRadioButton Gradient = new JRadioButton("Gradient");
                Gradient.setActionCommand("gradient");

                JLabel c1 = new JLabel("Gradient Start:");
                JTextField color1 = new JTextField("0000FF");
                JLabel c2 = new JLabel("Gradient End:");
                JTextField color2 = new JTextField("FFAA00");
                Renderers.add(Black);
                Renderers.add(Gradient);

                Configuration.add(Black);
                Configuration.add(Gradient);
                Configuration.add(c1);
                Configuration.add(color1);
                Configuration.add(c2);
                Configuration.add(color2);


                //=====THIS IS THE MOST RELEVANT SECTION FOR PROJECT 1=====
                //Create the Walk button
                JButton Walk = new JButton("Walk");

                //Assign a behavior to run when the Walk button is pressed
                Walk.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        int count = Integer.parseInt(StepCount.getText()); //gets the string from a TextField, and read it as an int
                        String walk_type = WalkerTypes.getSelection().getActionCommand();//gets the action command of which radio button is selected, a String describing the type of Walk
                        String geom_type = Geometries.getSelection().getActionCommand();
                        String render_type = Renderers.getSelection().getActionCommand();
                        int grad_start = (int)Long.parseLong("FF" + color1.getText(), 16);//Get the color, convert from hex string to int
                        int grad_end = (int)Long.parseLong("FF" + color2.getText(), 16);//Get the color, convert from hex string to int

                        int[] gradient = CreateGradient(grad_start, grad_end, count);
                        if(render_type.equals("black")){
                            for (int i = 0; i < gradient.length; i++) {
                                gradient[i] = 0xFF000000; // Black color with full opacity
                            }
                        }
                        //===Walk, Update Display, repaint===
                        //1. Generate a Buffered image in the specified style using the data from above
                        BufferedImage GeneratedImage = new BufferedImage(WIDTH,HEIGHT,BufferedImage.TYPE_INT_ARGB);

                        boolean skip = walk_type.equals("skippy");
                        boolean loop = geom_type.equals("toroidal");
                        WalkFill(count, GeneratedImage, skip, loop, gradient);
                        //GeneratedImage.setRGB(WIDTH/2, HEIGHT/2, 0xFF000000);

                        //2. Update the display with the generated image
                        UpdateDisplay(GeneratedImage);
                        window.repaint();
                    }
                });

                Configuration.add(Walk);
                window.add(Configuration,BorderLayout.EAST);
            }
        });
    }
    //functions!
    static public int RGBAtoInt(int r, int g, int b, int a){
        //get seperates things and combine into hex ARGB or operator!
        return ((a << 24) | (r << 16) | (g << 8) | (b));

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
    static public int[] CreateGradient(int GradientStartColor, int GradientEndColor, int steps){
        //we have data parsed from Int to RGBA funciton
        int[] initial = InttoRGBA(GradientStartColor);
        int[] ending = InttoRGBA(GradientEndColor);
        //get differeces
        float redDiff = (float)(ending[0] - initial[0])/(steps - 1);
        float greenDiff = (float)(ending[1] - initial[1]) / (steps - 1);
        float blueDiff = (float)(ending[2] - initial[2]) / (steps - 1);
        float alphaDiff = (float)(ending[3] - initial[3]) / (steps - 1);
        int[] gradient = new int[steps];
        //over X number of steps
        float alpha = initial[3];
        float red = initial[0];
        float green = initial[1];
        float blue = initial[2];
        for(int i = 0; i < steps; i++) {
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
    static void Case1(Map<String, Integer> map, BufferedImage img, boolean paint, int color) {
        if(paint) {
            img.setRGB(map.get("x") - 1, map.get("y") - 1, color);
        }
        map.put("x", map.get("x") - 1);
        map.put("y", map.get("y") - 1);

    }
    static void Case2(Map<String, Integer> map, BufferedImage img, boolean paint, int color) {
        if(paint) {
            img.setRGB(map.get("x"), map.get("y") - 1, color);
        }
        map.put("y", map.get("y") - 1);
    }
    static void Case3(Map<String, Integer> map, BufferedImage img, boolean paint, int color) {
        if(paint) {
            img.setRGB(map.get("x") + 1, map.get("y") - 1, color);
        }
        map.put("x", map.get("x") + 1);
        map.put("y", map.get("y") - 1);

    }
    static void Case4(Map<String, Integer> map, BufferedImage img, boolean paint, int color) {
        if(paint) {
            img.setRGB(map.get("x") - 1, map.get("y"), color);
        }
        map.put("x", map.get("x") - 1);

    }
    static void Case5(Map<String, Integer> map, BufferedImage img, boolean paint, int color) {
        if(paint) {
            img.setRGB(map.get("x") + 1, map.get("y"), color);
        }
        map.put("x", map.get("x") + 1);

    }
    static void Case6(Map<String, Integer> map, BufferedImage img, boolean paint, int color) {
        if(paint) {
            img.setRGB(map.get("x") - 1, map.get("y") + 1, color);
        }
        map.put("x", map.get("x") - 1);
        map.put("y", map.get("y") + 1);

    }
    static void Case7(Map<String, Integer> map, BufferedImage img, boolean paint, int color) {
        if(paint) {
            img.setRGB(map.get("x"), map.get("y") + 1, color);
        }
        map.put("y", map.get("y") + 1);

    }
    static void Case8(Map<String, Integer> map, BufferedImage img, boolean paint, int color) {
        if(paint) {
            img.setRGB(map.get("x") + 1, map.get("y") + 1, color);
        }
        map.put("x", map.get("x") + 1);
        map.put("y", map.get("y") + 1);

    }
    static void Case9(Map<String, Integer> map, BufferedImage img, boolean paint, int color) {
        if(paint) {
            img.setRGB(WIDTH - 1, map.get("y") - 1, color);
        }
        map.put("x", WIDTH - 1);
        map.put("y", map.get("y") - 1);

    }
    static void Case10(Map<String, Integer> map, BufferedImage img, boolean paint, int color) {
        if(paint) {
            img.setRGB(WIDTH - 1, HEIGHT - 1, color);
        }
        map.put("x", WIDTH - 1);
        map.put("y", HEIGHT - 1);

    }
    static void Case11(Map<String, Integer> map, BufferedImage img, boolean paint, int color) {
        if(paint) {
            img.setRGB(map.get("x"), HEIGHT - 1, color);
        }
        map.put("y", HEIGHT - 1);

    }
    static void Case12(Map<String, Integer> map, BufferedImage img, boolean paint, int color) {
        if(paint) {
            img.setRGB(0, map.get("y") - 1, color);
        }
        map.put("x", 0);
        map.put("y", map.get("y") - 1);

    }
    static void Case13(Map<String, Integer> map, BufferedImage img, boolean paint, int color) {
        if(paint) {
            img.setRGB(0, HEIGHT - 1, color);
        }
        map.put("x", 0);
        map.put("y", HEIGHT - 1);

    }
    static void Case14(Map<String, Integer> map, BufferedImage img, boolean paint, int color) {
        if(paint) {
            img.setRGB(WIDTH- 1, map.get("y"), color);
        }
        map.put("x", WIDTH - 1);
    }
    static void Case15(Map<String, Integer> map, BufferedImage img, boolean paint, int color) {
        if(paint) {
            img.setRGB(0, map.get("y"), color);
        }
        map.put("x", 0);
    }
    static void Case16(Map<String, Integer> map, BufferedImage img, boolean paint, int color) {
        if(paint) {
            img.setRGB(WIDTH- 1, map.get("y") + 1, color);
        }
        map.put("x", WIDTH - 1);
        map.put("y", map.get("y") + 1);
    }
    static void Case17(Map<String, Integer> map, BufferedImage img, boolean paint, int color) {
        if(paint) {
            img.setRGB(WIDTH- 1, 0, color);
        }
        map.put("x", WIDTH - 1);
        map.put("y", 0);
    }
    static void Case18(Map<String, Integer> map, BufferedImage img, boolean paint, int color) {
        if(paint) {
            img.setRGB(map.get("x"), 0, color);
        }
        map.put("y", 0);
    }
    static void Case19(Map<String, Integer> map, BufferedImage img, boolean paint, int color) {
        if(paint) {
            img.setRGB(0, map.get("y") + 1, color);
        }
        map.put("x", 0);
        map.put("y", map.get("y") + 1);
    }
    static void Case20(Map<String, Integer> map, BufferedImage img, boolean paint, int color) {
        if(paint) {
            img.setRGB(0, 0, color);
        }
        map.put("x", 0);
        map.put("y", 0);
    }
    static void WalkFill(int steps, BufferedImage img, boolean isSkipping, boolean isLooped, int[] gradient){
        Random random = new Random();
        Map<String, Integer> map = new HashMap<>();
        //check skippy
        boolean paint = true;
        int skipNum = 0;
        int min = 0;
        int max = 500;

        if(isSkipping){
            skipNum = random.nextInt((max-min) + 1) + min;
        }
        //normal walking first
        map.put("x", WIDTH/2);
        map.put("y", HEIGHT/2);
        img.setRGB(map.get("x"), map.get("y"), gradient[0]);
        for(int i = 1; i < steps; i++){
            if(skipNum == 0 && isSkipping){
                paint = !paint;
                skipNum = random.nextInt((max-min) + 1) + min;
            }
            //RANDOMLY pick between 8 spaces
            int rand = random.nextInt(8)+1;
            int rand = random.nextInt(8)+1;
            skipNum--;
            switch(rand){


                case 1:
                    //System.out.println(map.get("y"));
                    if(map.get("x") == 0 && map.get("y") != 0){
                        if(isLooped){
                            Case9(map, img, paint, gradient[i]);
                        }
                        else {
                            Case2(map, img, paint, gradient[i]);
                        }
                        break;
                    }
                    else if(map.get("y") == 0){
                        if(isLooped && map.get("x") == 0){
                            Case10(map, img, paint, gradient[i]);
                        }
                        break;
                    }
                    else {
                        Case1(map, img, paint, gradient[i]);
                        break;
                    }
                case 2:
                    if (map.get("y") != 0) {
                        Case2(map, img, paint, gradient[i]);
                    }
                    else{
                        if(isLooped){
                            Case11(map, img, paint, gradient[i]);
                        }
                    }
                    break;
                case 3:
                    if(map.get("x") == WIDTH - 1 && map.get("y") != 0){
                        if(isLooped) {
                            Case12(map,img, paint, gradient[i]);
                        }
                        else {
                            Case2(map, img, paint, gradient[i]);
                        }
                        break;
                    }
                    else if(map.get("y") == 0){
                        if(isLooped && map.get("x") == WIDTH - 1){
                            Case13(map, img, paint, gradient[i]);
                        }
                        break;
                    }
                    else {
                        Case3(map, img, paint, gradient[i]);
                        break;
                    }
                case 4:
                    if (map.get("x") != 0) {
                        Case4(map, img, paint, gradient[i]);
                    }else{
                        if(isLooped){
                            Case14(map, img, paint, gradient[i]);
                        }
                    }
                    break;
                case 5:
                    if (map.get("x") != WIDTH - 1) {
                        Case5(map, img, paint, gradient[i]);
                    }
                    else{
                        if(isLooped){
                            Case15(map, img, paint, gradient[i]);
                        }
                    }
                    break;
                case 6:
                    if(map.get("x") == 0 && map.get("y") != HEIGHT - 1){
                        if(isLooped){
                            Case16(map, img, paint, gradient[i]);
                        }
                        else {
                            Case7(map, img, paint, gradient[i]);
                        }
                        break;
                    }
                    else if(map.get("y") == HEIGHT - 1){
                        if(isLooped && map.get("x") == 0){
                            Case17(map, img, paint, gradient[i]);
                        }
                        break;
                    }
                    else{
                        Case6(map, img, paint, gradient[i]);
                        break;
                    }
                case 7:
                    if (map.get("y") != HEIGHT - 1) {
                        Case7(map, img, paint, gradient[i]);
                    }
                    else{
                        if(isLooped){
                            Case18(map, img, paint, gradient[i]);
                        }
                    }
                    break;
                case 8:
                    if(map.get("x") == WIDTH - 1 && map.get("y") != HEIGHT - 1){
                        if(isLooped){
                            Case19(map, img, paint, gradient[i]);
                        }
                        else {
                            Case7(map, img, paint, gradient[i]);
                        }
                        break;
                    }
                    else if(map.get("y") == HEIGHT - 1){
                        if(isLooped && map.get("x") == WIDTH - 1){
                            Case20(map, img, paint,gradient[i]);
                        }
                        break;
                    }
                    else{
                        Case8(map, img, paint, gradient[i]);
                        break;
                    }
            }
        }

    }


}
































