import java.util.ArrayList;

public class Input {
    //Contains characters that were pressed this frame
    static private ArrayList<Character> PressedList = new ArrayList<>();
    //contains characters that are pressed
    static private ArrayList<Character> HeldList = new ArrayList<>();
    //Contains characters that were released this frame
    static private ArrayList<Character> ReleasedList = new ArrayList<>();

    private static int MouseX; //the current mouse X position on the DISPLAY BufferedImage
    private static int MouseY; //the current mouse Y position on the DISPLAY BufferedImage
    private static boolean MouseDown; //true if the mouse is currently down
    private static boolean MouseClicked; //true if the mouse was clicked this frame




    //"Received" values directly from the UI.
    // These may arrive at uneven times, possibly mid-frame, which can cause inputs to be missed by some scripts.
    // These will be used to modify the above fields at the START of each frame.
    static private ArrayList<Character> Received_PressedList = new ArrayList<>();
    static private ArrayList<Character> Received_HeldList = new ArrayList<>();
    static private ArrayList<Character> Received_ReleasedList = new ArrayList<>();
    private static int Received_MouseX;
    private static int Received_MouseY;
    private static boolean Received_MouseDown;
    private static boolean Received_MouseClicked;

    //TODO: This function should be called at the START of the frame
    // it uses values received from event listeners (operating at a different speed than our FRAMETIMER)
    // to update the respective field. Then, the received lists are emptied/returned to a default state.
    static void ReceiveInputs(){
        //Update the actual list
        PressedList.addAll(Received_PressedList);
        HeldList.addAll(Received_HeldList);
        ReleasedList.addAll(Received_ReleasedList);
        MouseX = Received_MouseX;
        MouseY = Received_MouseY;
        MouseDown = Received_MouseDown;
        MouseClicked = Received_MouseClicked;
        //clear all
        Received_HeldList.clear();
        Received_PressedList.clear();
        Received_ReleasedList.clear();
        Received_MouseClicked = false;
    }


    //TODO: This function should called at the END of every frame and
    // clear any values in the Input class that need to be removed,
    // e.g., if a key was pressed on a frame, it should be removed from the pressed list the next frame
    static void ValidateInputs(){
        ArrayList<Character> to_remove = new ArrayList<>();
        for(Character c: HeldList){
            if(ReleasedList.contains(c))
                to_remove.add(c);
        }
        for (Character c : to_remove){
            HeldList.remove(c);
        }
        PressedList.clear();
        ReleasedList.clear();
        //held is never cleared because it always continues onto the next frame?
        // Reset the MouseClicked state for the next frame
        MouseClicked = false;
    }

    //TODO: Add the character to the list of received pressed keys if it is not already in the actual set of pressed keys.
    static public void AddKeyPressed(char c){
        //and held?
        if(!PressedList.contains(c) && !HeldList.contains(c)){
            Received_PressedList.add(c);
        }
    }

    //TODO: Add the character to the list of received held keys if it is not already in the actual set of held keys.
    static public void AddKeyHeld(char c){
        if(!HeldList.contains(c)){
            Received_HeldList.add(c);
        }
    }

    //TODO: Add the character to the list of received released keys if it is not already in the actual set of released keys.
    static public void AddKeyReleased(char c){
        if(!ReleasedList.contains(c)){
            Received_ReleasedList.add(c);
        }
    }

    //TODO: return true if c is in the pressed list
    static boolean GetKeyPressed(char c){
        return PressedList.contains(c);
    }

    //TODO: return true if c is in the held list
    static boolean GetKeyHeld(char c){
        return HeldList.contains(c);
    }

    //TODO: return true if c is in the released list
    static boolean GetKeyReleased(char c){
        return ReleasedList.contains(c);
    }


    //Getters and Setters
    //TODO: Done for you!
    public static int GetMouseX() {
        return MouseX;
    }

    public static void SetMouseX(int mouseX) {
        Received_MouseX = mouseX;
    }

    public static int GetMouseY() {
        return MouseY;
    }

    public static void SetMouseY(int mouseY) {
        Received_MouseY = mouseY;
    }

    public static boolean IsMouseDown() {
        return MouseDown;
    }

    public static void SetMouseDown(boolean mousePressed) {
        Received_MouseDown = mousePressed;
    }

    public static boolean IsMouseClicked() {
        return MouseClicked;
    }

    public static void SetMouseClicked(boolean mouseClicked) {
        Received_MouseClicked = mouseClicked;
    }
}
