import javax.swing.*;
import javax.swing.event.ChangeEvent;
import javax.swing.event.ChangeListener;
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.AffineTransform;
import java.awt.image.BufferedImage;
import java.util.HashMap;
import java.util.Stack;
import java.awt.event.MouseEvent;

public class Project3_Codebase {

    static Graphics2D DisplayGraphics;
    static String finalLSystem ="";
    static String currentSegment;
    static float segmentLength;
    static float segmentWidth;
    static float rotationAngle;
    static boolean drawTurtle;
    //UI Variables
    static int WIDTH = 500;
    static int HEIGHT = 500;
    static BufferedImage Display;
    static RulePanel r1;
    static RulePanel r2;
    static JTextField width,length, angle, start, iteration;

    //Panning and Zooming Variables
    static float TurtleScale = 1.0f;
    static float XOffset = 0;
    static float YOffset = 0;

    //Animation Variables
    static Timer AnimationTimer;
    static int TurtleStepCount = 0;

    //LSystem
    static String LSystemString = "";

    //Rules, made by extending the JPanel class to create a new component
    static class RulePanel extends JPanel {
        String name;
        JLabel nameLabel;
        JTextField word;
        JTextField rule;

        RulePanel(String name){
            this.setLayout(new BoxLayout(this,BoxLayout.PAGE_AXIS));

            nameLabel = new JLabel(name);
            word = new JTextField("A");
            rule = new JTextField("AB");
            rule.setPreferredSize(new Dimension(100,25));

            this.add(nameLabel);
            this.add(word);
            this.add(rule);
        }

        public String GetWord(){
            return word.getText();
        }

        public String GetRule(){
            return rule.getText();
        }
    }

    public static void main(String[] args) {

        //make our UI on EDT using invokeLater
        //have an action listener on a button that will make and start a new thread for calucation/image drawing
        //when that thread makes the image, repaint the window on the EDT using invoke later
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                //Creating the  window
                JFrame window = new JFrame("L-System");
                window.setPreferredSize(new Dimension(WIDTH + 100, HEIGHT + 50));
                window.pack();
                window.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
                window.setVisible(true);

                //Display panel containing the BufferedImage
                JPanel DisplayPanel = new JPanel();
                DisplayPanel.setBackground(Color.ORANGE);
                Display = new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_ARGB);
                Display.setRGB(250, 250, Color.BLACK.getRGB());
                DisplayGraphics = (Graphics2D) Display.getGraphics();
                DisplayPanel.add(new JLabel(new ImageIcon(Display)));
                window.add(DisplayPanel, BorderLayout.CENTER);

                //==============Menu Bar===============
                //sets up some default configurations for different L-systems
                JMenuBar MenuBar = new JMenuBar();
                window.setJMenuBar(MenuBar);

                JMenu Configure = new JMenu("Load Configuration");
                MenuBar.add(Configure);
                JMenuItem bin_tree = new JMenuItem("Binary Tree");
                bin_tree.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        angle.setText("45");
                        start.setText("0");
                        width.setText("1");
                        r1.word.setText("0");
                        r1.rule.setText("1[*<0]>0");
                        r2.word.setText("1");
                        r2.rule.setText("11");
                    }
                });
                Configure.add(bin_tree);

                JMenuItem Sierpinksi = new JMenuItem("Sierpinksi Triangle");
                Sierpinksi.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        angle.setText("120");
                        start.setText("F>F>F");
                        width.setText("1");
                        r1.word.setText("F");
                        r1.rule.setText("F>G<F<G>F");
                        r2.word.setText("G");
                        r2.rule.setText("GG");
                    }
                });
                Configure.add(Sierpinksi);

                JMenuItem Dragon = new JMenuItem("Dragon Curve");
                Dragon.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        angle.setText("90");
                        start.setText("F");
                        width.setText("1");
                        r1.word.setText("F");
                        r1.rule.setText("F<G");
                        r2.word.setText("G");
                        r2.rule.setText("F>G");
                    }
                });
                Configure.add(Dragon);

                JMenuItem Fern = new JMenuItem("Barnsley Fern");
                Fern.addActionListener(new ActionListener() {
                    @Override
                    public void actionPerformed(ActionEvent e) {
                        angle.setText("25");
                        start.setText("X");
                        width.setText("10");
                        r1.word.setText("X");
                        r1.rule.setText("F>--[[X]<X]<F[>FX]>X");
                        r2.word.setText("F");
                        r2.rule.setText("FF");
                    }
                });
                Configure.add(Fern);

                //==========Config panel=============
                JPanel Configuration = new JPanel();
                Configuration.setBackground(new Color(230, 230, 230));
                Configuration.setPreferredSize(new Dimension(250, 500));
                Configuration.setLayout(new FlowLayout());

                //iterations
                JPanel Iterations = new JPanel();
                Iterations.add(new JLabel("Number of Iterations"));
                iteration = new JTextField("5");
                iteration.setPreferredSize(new Dimension(50, 20));
                Iterations.add(iteration);
                Configuration.add(Iterations);

                //Angle
                JPanel Angle = new JPanel();
                Angle.add(new JLabel("Turn Angle"));
                angle = new JTextField("45");
                angle.setPreferredSize(new Dimension(50, 20));
                Angle.add(angle);
                Configuration.add(Angle);

                //Length
                JPanel Length = new JPanel();
                Length.add(new JLabel("Segment Length"));
                length = new JTextField("10");
                length.setPreferredSize(new Dimension(50, 20));
                Length.add(length);
                Configuration.add(Length);

                //Width
                JPanel Width = new JPanel();
                Width.add(new JLabel("Segment Width"));
                width = new JTextField("1");
                width.setPreferredSize(new Dimension(50, 20));
                Width.add(width);
                Configuration.add(Width);

                //Start
                JPanel Start = new JPanel();
                Start.add(new JLabel("Start"));
                start = new JTextField("A");
                start.setPreferredSize(new Dimension(50, 20));
                Start.add(start);
                Start.setPreferredSize(new Dimension(150, 25));
                Configuration.add(Start);

                r1 = new RulePanel("Rule 1");
                Configuration.add(r1);

                r2 = new RulePanel("Rule 2");
                Configuration.add(r2);

                JCheckBox DrawTurtleCheck = new JCheckBox("Draw Turtle");
                Configuration.add(DrawTurtleCheck);

                JPanel SpeedPanel = new JPanel();
                SpeedPanel.add(new JLabel("Speed:"));
                JSlider Speed = new JSlider(0,1000);
                Speed.setValue(50);
                Speed.addChangeListener(new ChangeListener() {
                    @Override
                    public void stateChanged(ChangeEvent e) {
                        //TODO: Adjust the initial delay of the timer when the slider moves
                        //int sliderValue = Speed.getValue();
                        //int newDelay = 10 + (100 - sliderValue) * 10;
                        int sliderValue = Speed.getValue()+1;
                        //System.out.println(sliderValue);
                        //int newDelay = 1 + (sliderValue * 10);
                        // Set the delay of the AnimationTimer
                        AnimationTimer.setDelay(sliderValue);
                    }
                });
                SpeedPanel.add(Speed);
                Configuration.add(SpeedPanel);

                //Run Button
                JButton Run = new JButton("Generate");
                Run.setPreferredSize(new Dimension(200,50));
                Run.addActionListener(new ActionListener() {

                    @Override
                    public void actionPerformed(ActionEvent e) {
                        HashMap<Character,String> rulemap = new HashMap<Character, String>();
                        if(r1.GetWord().length()!=0)
                            rulemap.put(r1.GetWord().charAt(0),r1.GetRule());
                        if(r2.GetWord().length()!=0)
                            rulemap.put(r2.GetWord().charAt(0),r2.GetRule());

                        int iterations = Integer.parseInt(iteration.getText());
                        String starting_axiom = start.getText();

                        //TODO: Generate the L-system string, then draw it. Later, draw it in a Timer to animate it.
                        //Generate String
                        StringBuilder currentString = new StringBuilder(starting_axiom);
                        for(int i = 0; i < iterations; i++){
                            //creating a string for the new mapping/system
                            StringBuilder tempString = new StringBuilder();
                            //looping and assessing each characater
                            for(int j = 0; j < currentString.length(); j++){
                                char temp = currentString.charAt(j);
                                //put in the rune from the hash!
                                if(rulemap.containsKey(temp)){
                                    //System.out.println(temp + "to " + rulemap.get(temp));
                                    tempString.append(rulemap.get(temp));
                                }else{
                                    //System.out.println(temp);
                                    tempString.append(temp);
                                }
                            }
                            //System.out.println(currentString);
                            currentString = tempString;
                        }
                        //draw system
                        finalLSystem = currentString.toString();
                        segmentLength = Float.parseFloat(length.getText());
                        segmentWidth = Float.parseFloat(width.getText());
                        rotationAngle = Float.parseFloat(angle.getText());
                        drawTurtle = DrawTurtleCheck.isSelected();
                        TurtleStepCount = 0;
                        AnimationTimer.restart();
                    }
                });
                Configuration.add(Run);
                window.add(Configuration,BorderLayout.EAST);

                //==========EVENTS==============
                DisplayPanel.setFocusable(true);
                DisplayPanel.addMouseWheelListener(new MouseWheelListener() {
                    @Override
                    public void mouseWheelMoved(MouseWheelEvent e) {
                        //TODO: Use the MouseWheelEvent to modify the current scaling factor, then redraw the L-system at the new scale and repaint the UI
                        //System.out.println("scaling");
                        int rotation = e.getWheelRotation();
                        double scaleFactor = 0.1;
                        if(rotation < 0){
                            TurtleScale += scaleFactor;
                        }else{
                            TurtleScale -= scaleFactor;
                            if(TurtleScale < 0.1){
                                TurtleScale = 0.1f;
                            }
                        }
                        //DisplayGraphics.scale(TurtleScale, TurtleScale);
                        UpdateDisplay(Draw_LSystem(currentSegment, segmentLength, segmentWidth, rotationAngle, drawTurtle));

                        DisplayPanel.repaint();

                        //window.repaint();
                        // Repaint the display panel
                        //DisplayPanel.repaint();
                    }
                });

                DisplayPanel.addMouseMotionListener(new MouseMotionListener() {
                    private int lastMouseX;
                    private int lastMouseY;
                    private boolean isDragging = false;
                    @Override
                    public void mouseDragged(MouseEvent e) {

                        //TODO: Use the MouseEvent to modify the current X and Y offsets, then redraw the L-system at the new scale on its own thread, then repaint the UI on the EDT. You should account for the difference between the coordinate systems of the turtle and the JPanel
                        //get the difference from mouse position (initial) to mouse position end
                        if (!isDragging) {
                            // Start drag: initialize last mouse position
                            lastMouseX = e.getX();
                            lastMouseY = e.getY();
                            isDragging = true;
                        }

                        // Calculate the incremental change
                        int deltaX = e.getX() - lastMouseX;
                        int deltaY = e.getY() - lastMouseY;

                        // Apply this incremental change to the offsets
                        XOffset += deltaX;
                        YOffset += deltaY;

                        // Update the last known mouse position for the next drag event
                        lastMouseX = e.getX();
                        lastMouseY = e.getY();
                        new Thread(() -> {
                            UpdateDisplay(Draw_LSystem(currentSegment, segmentLength, segmentWidth, rotationAngle, drawTurtle));
                            SwingUtilities.invokeLater(() -> DisplayPanel.repaint());
                        }).start();

                        //window.repaint();

                    }

                    @Override
                    public void mouseMoved(MouseEvent e) {
                        //TODO: Keep track of the mouse position even if the mouse isn't clicked
                        lastMouseX = e.getX();
                        lastMouseY = e.getY();
                        isDragging = false;
                    }
                });


                AnimationTimer = new Timer(100, new ActionListener() {

                    @Override
                    public void actionPerformed(ActionEvent e) {
                        //TODO: Progressively draw more and more of the current L-system each time the Timer starts over. Repaint the UI at the end of the Timer, and restart it.
                        //if(!finalLSystem.isEmpty()) {
                            Thread drawThread = new Thread(new Runnable() {
                                @Override
                                public void run() {

                                    //TODO: Run your code
                                    //looping through a stack of the string
                                    if (TurtleStepCount > finalLSystem.length()) {
                                        AnimationTimer.stop();
                                        return;
                                    }
                                    currentSegment = finalLSystem.substring(0, TurtleStepCount);
                                    UpdateDisplay(Draw_LSystem(currentSegment, segmentLength, segmentWidth, rotationAngle, drawTurtle));
                                    TurtleStepCount++;

                                    SwingUtilities.invokeLater(new Runnable() {
                                        @Override
                                        public void run() {
                                            DisplayPanel.repaint(); // Repaint to display the latest segment
                                            //window.repaint();
                                        }
                                    });
                                }
                            });

                            drawThread.start();
                        }
                    //}
                });
                AnimationTimer.start();
            }
        });
    }



    //Recommended function... not required to use this but this might be helpful.
    //You can give it a known system string and implement the drawing part without needed to generate your own strings, for testing.
    //You can also use in you timer to draw the system while animating.
    static BufferedImage Draw_LSystem(String system, float segment_length, float stroke, float rotation_angle, boolean drawturtle){
        BufferedImage img = new BufferedImage(WIDTH, HEIGHT, BufferedImage.TYPE_INT_ARGB);
        Graphics2D g2d;
        g2d = img.createGraphics();
        g2d.translate(WIDTH/2, HEIGHT/2);
        g2d.scale(TurtleScale, TurtleScale);
        g2d.translate(XOffset, YOffset);
        //DisplayGraphics.fillOval(-5,-5,10,10);
        float x = 0;
        float y = 0;
        float currentAngle = 90;
        g2d.setColor(Color.black);
        g2d.setStroke(new BasicStroke(stroke));
        //stack for saving position
        Stack<Float> strk = new Stack<>();
        Stack<AffineTransform> stack = new Stack<>();
        stack.push(g2d.getTransform());
        strk.push(stroke);
        for(int i = 0; i < system.length(); i++){
            char command = system.charAt(i);
            //System.out.println(command + " " + segment_length);
            switch (command){
                case '<':
                    g2d.rotate(Math.toRadians(-rotation_angle));
                    //currentAngle += rotation_angle;
                    break;
                case '>':
                    g2d.rotate(Math.toRadians(rotation_angle));
                    //currentAngle -= rotation_angle;
                    break;
                case '+':
                    //System.out.println(2);
                    stroke += 1;
                    g2d.setStroke(new BasicStroke(stroke));
                    break;
                case '-':
                    //System.out.println( 3);
                    stroke -= 1;
                    g2d.setStroke(new BasicStroke(stroke));
                    break;
                case '[':
                    //System.out.println(4);
                    //positionSaved.push(new float[]{x, y, currentAngle, stroke});
                    strk.push(stroke);
                    stack.push(g2d.getTransform());
                    break;
                case ']': // Restore previous state
                    //System.out.println(5);
                    g2d.setTransform(stack.pop());
                    stroke = strk.pop();
                    g2d.setStroke(new BasicStroke(stroke));

                    //float[] state = positionSaved.pop();
                    //x = state[0];
                    //y = state[1];
                    //currentAngle = state[2];
                    //DisplayGraphics.setStroke(new BasicStroke(stroke));
                    break;
                case '*':
                    //System.out.println(6);
                    g2d.setColor(Color.RED);
                    g2d.fillOval((int) (x - segment_length / 4), (int) (y - segment_length / 4), (int) segment_length / 2, (int) segment_length / 2);
                    g2d.setColor(Color.BLACK);
                    break;
                default:
                    //System.out.println(1);

                    float endX = x + (float) Math.cos(Math.toRadians(currentAngle)) * segment_length;
                    float endY = y - (float) Math.sin(Math.toRadians(currentAngle)) * segment_length;
                    g2d.drawLine((int)x, (int) y, (int)endX, (int) endY);
                    g2d.translate(endX-x, endY - y);
                    break;
            }

        }
        //DisplayGraphics.dispose();
        if (drawturtle) {
            DrawTurtle(g2d);
        }
        g2d.setTransform(stack.pop());
        g2d.dispose();
        //finalLSystem = "";
        return img; //return value of an empty image included to allow the function to compile.
    }


    //==========PROVIDED FUNCTIONS===========
    static void UpdateDisplay(BufferedImage img){
        Graphics2D g = (Graphics2D) Display.getGraphics();
        g.setColor(Color.WHITE);
        g.fillRect(0,0,WIDTH,HEIGHT);
        g.drawImage(img,0,0,null);
    }
    //Draws the Turtle at its current spot/orientation.
    static void DrawTurtle(Graphics2D pen){
        pen.rotate(Math.toRadians(180));
        //body (centered at current X,Y location)
        pen.setColor(new Color(255,255,100));
        pen.fillOval(-5,-5,10,10);

        //right arm (facing in +X direction)
        pen.setColor(new Color(150,50,50));
        pen.drawLine(0,0,10,0);

        //head (facing in +Y direction)
        pen.setColor(new Color(50,100,50));
        pen.drawLine(0,0,0,10);
        pen.rotate(Math.toRadians(-180));
    }
}


