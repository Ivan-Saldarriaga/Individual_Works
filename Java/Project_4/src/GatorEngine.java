import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import java.awt.geom.Ellipse2D;
import java.awt.image.BufferedImage;
import java.util.ArrayList;

/*
This class implements three main features for the engine:
1. The UI code: setting up the display window that contains the "view" of the application implemented by the engine.
2. The GatorEngine logic: GameObject management (creation, deletion, starting, updating, etc.)
3. Input management: tracking key/mouse presses, releases, movement, clicks, etc.
*/
enum GameState{
    START_SCREEN,
    PLAYING,
    GAME_OVER
}
public class GatorEngine {
    //TODO: implementation stuff start below here
    //==========vvv Engine Implementation vvv==========

    //UI Components (things that are related to the UI/Display)
    static JFrame WINDOW;
    static JPanel DISPLAY_CONTAINER;
    static JLabel DISPLAY_LABEL;
    static BufferedImage DISPLAY;
    static int WIDTH=1200, HEIGHT=800;
    static GameState currentState = GameState.START_SCREEN;
    //Engine Components (things that are related to the engine structures)
    static Graphics2D RENDERER; //A Graphics2D used to draw all GameObjects
    static ArrayList<GameObject> OBJECTLIST = new ArrayList<>(); //list of GameObjects in the scene
    static ArrayList<GameObject> CREATELIST = new ArrayList<>(); //list of GameObjects marked to be added at the end of the frame
    static ArrayList<GameObject> DELETELIST = new ArrayList<>(); //list of GameObjects marked to be deleted at the end of the frame
    static float FRAMERATE = 60; //target frames per second;
    static float FRAMEDELAY = 1000/FRAMERATE; //target delay between frames
    static Timer FRAMETIMER; //Timer controlling the update loop


    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            @Override
            public void run() {
                CreateEngineWindow();
            }
        });
    }

    //Creates the application window, sets up FRAMETIMER and EventListeners
    static void CreateEngineWindow(){
        //Sets up the GUI
        WINDOW = new JFrame("Gator Engine");
        WINDOW.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        WINDOW.setVisible(true);

        DISPLAY = new BufferedImage(WIDTH,HEIGHT,BufferedImage.TYPE_INT_ARGB);
        RENDERER = (Graphics2D) DISPLAY.getGraphics();
        DISPLAY_CONTAINER = new JPanel();
        DISPLAY_CONTAINER.setFocusable(true);
        DISPLAY_LABEL = new JLabel(new ImageIcon(DISPLAY));
        DISPLAY_CONTAINER.add(DISPLAY_LABEL);
        WINDOW.add(DISPLAY_CONTAINER);
        WINDOW.pack();


        //TODO: This Timer runs at the target FPS. You need to implement the Update loop.
        FRAMETIMER = new Timer((int)FRAMEDELAY, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                Thread FRAMETHREAD = new Thread(new Runnable() {
                    @Override
                    public void run() {
                        //TODO: Update loop
                        // Use Inputs received in the middle of the previous timer execution to update the Input class (Input.ReceiveInputs())
                        // Execute the implemented UpdateEngine() to Update() all GameObjects in the Engine.
                        // Clear any inputs that need to be removed between frames (e.g., MouseClicked, KeyPressed, etc.) with Input.ValidateInputs()
                        // Ensure OBJECTLIST is up to date
                        SwingUtilities.invokeLater(new Runnable() {
                            @Override
                            public void run() {
                                FRAMETIMER.stop();
                            }
                        });
                        Input.ReceiveInputs();
                        UpdateEngine();
                        Input.ValidateInputs();
                        ValidateGameObjectLists();
                        //System.out.println("debug");
                        SwingUtilities.invokeLater(new Runnable() {
                            @Override
                            public void run() {
                                //TODO:
                                DISPLAY_LABEL.repaint();
                                FRAMETIMER.restart();
                                // Repaint the GUI and restart the FRAMETIMER back on the EDT.
                            }
                        });
                    }
                });
                FRAMETHREAD.start();
            }
        });
        FRAMETIMER.start();
        StartEngine();

        //===================INPUT=========================
        //Key Events
        //TODO: Use the events to manage the various lists and data in the Input class.
        DISPLAY_CONTAINER.addKeyListener(new KeyListener() {
            @Override
            public void keyTyped(KeyEvent e) {

            }

            @Override
            public void keyPressed(KeyEvent e) {
                Input.AddKeyPressed(e.getKeyChar());
                Input.AddKeyHeld(e.getKeyChar());
            }

            @Override
            public void keyReleased(KeyEvent e) {
                Input.AddKeyReleased(e.getKeyChar());
            }
        });

        //Mouse Events
        DISPLAY_CONTAINER.addMouseListener(new MouseListener() {
            @Override
            public void mouseClicked(MouseEvent e) {

            }

            @Override
            public void mousePressed(MouseEvent e) {
                Input.SetMouseClicked(true);
                Input.SetMouseDown(true);
            }

            @Override
            public void mouseReleased(MouseEvent e) {
                Input.SetMouseDown(false);
            }

            @Override
            public void mouseEntered(MouseEvent e) {

            }

            @Override
            public void mouseExited(MouseEvent e) {

            }
        });

        DISPLAY_CONTAINER.addMouseMotionListener(new MouseMotionListener() {
            @Override
            public void mouseDragged(MouseEvent e) {
                Input.SetMouseX(e.getX() - DISPLAY_LABEL.getLocation().x);
                Input.SetMouseY(e.getY() - DISPLAY_LABEL.getLocation().y);
            }

            @Override
            public void mouseMoved(MouseEvent e) {
                Input.SetMouseX(e.getX() - DISPLAY_LABEL.getLocation().x);
                Input.SetMouseY(e.getY() - DISPLAY_LABEL.getLocation().y);
            }
        });
    }



    //=========Engine Functions

    //TODO: Mark an object for creation in the CREATELIST and Start() it.
    static void Create(GameObject g){
        CREATELIST.add(g);
        g.Start();
    }


    //TODO: Marks an object for deletion in the DELETELIST.
    static void Delete(GameObject g){
        DELETELIST.add(g);
    }

    //TODO: If an object was marked for deletion this frame, remove it from the OBJECTLIST.
    //TODO: If and object was marked for creation this frame, add it to the OBJECTLIST.
    //TODO: Remove objects marked for deletion/creation this frame from their repective list.
    static void ValidateGameObjectLists(){
        for(GameObject g: DELETELIST){
            OBJECTLIST.remove(g);
        }
        OBJECTLIST.addAll(CREATELIST);
        CREATELIST.clear();
        //deleting
        DELETELIST.clear();
    }

    //TODO: Start() the Application, and then Start() all objects in OBJECTLIST
    static void StartEngine(){
        Application.Start();
        //System.out.println(1);
        for(GameObject obj: OBJECTLIST){
            obj.Start();
        }
    }

    //TODO: Redraw the Background(), then Update() and Draw() all GameObjects in OBJECTLIST
    static void UpdateEngine(){
        Background();
        for(GameObject obj: OBJECTLIST){
            obj.Update();
            obj.Draw(RENDERER);
//            for (GameObject object: OBJECTLIST){
//                if (obj != object){
//                    if(obj.CollidesWith(object)){
//                        System.out.println("COLLISION");
//                    }
//                }
//            }
        }
        Application.Update();

        //DISPLAY_LABEL.repaint();
    }

    //Draws a background on the Renderer. right now it is solid, but we could load an image
    //TODO: Nothing! Done for you!
    static void Background(){
        RENDERER.setColor(Color.WHITE);
        RENDERER.fillRect(0,0,WIDTH,HEIGHT);

    }
}






















