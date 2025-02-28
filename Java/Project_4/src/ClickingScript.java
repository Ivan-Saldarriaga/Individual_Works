import java.awt.*;

public class ClickingScript  extends ScriptableBehavior {
//An example script; this one will look for character input,
// and translate whatever game object it is on
    int x,y,width,height;
    Color initial;
    Color secondary = new Color(0,255,25,50);
    //It is also customizable! Speed can change.
    boolean selected = false;//try changing this!
    ClickingScript(int x, int y, int width, int height, GameObject g) {
        super(g);
        this.x = x;
        this.y = y;
        this.width = width;
        this.height = height;
        initial = g.GetObjectMaterial().GetFill();
    }

    @Override
    public void Start() {

    }

    @Override
    public void Update(){
        //(Input.GetKeyHeld('a'))
            //gameObject.Translate(-speed,0);
        //if mouse i sclicked on start page
        //System.out.println(Input.GetMouseX() + " " + Input.GetMouseY());
        if(!selected) {
            if(Input.GetMouseX()> x && Input.GetMouseX() < x+width && Input.GetMouseY() > y && Input.GetMouseY() < y+height){
                gameObject.GetObjectMaterial().SetFill(secondary);
                if (Input.IsMouseDown()){
                    if(GatorEngine.currentState == GameState.GAME_OVER){
                        Application.lastGameState = 3;
                        Application.gameState = 1;
                        GatorEngine.currentState = GameState.START_SCREEN;
                    }else {
                        Application.lastGameState = 1;
                        Application.gameState = 2;
                        GatorEngine.currentState = GameState.PLAYING;
                    }
                }
            }else{
                gameObject.GetObjectMaterial().SetFill(initial);
            }
        }
        if(selected){
            System.out.println("Clicked start!");
        }


    }



}

