//An example script; this one will look for character input,
// and translate whatever game object it is on

//It is also customizable! Speed can change.
public class MovementScript  extends ScriptableBehavior {
    float speed = 0;//try changing this!
    MovementScript(GameObject g, float speed) {
        super(g);
        this.speed = speed;
    }

    @Override
    public void Start() {

    }

    @Override
    public void Update(){
        if(Input.GetKeyHeld('a'))
            gameObject.Translate(-speed,0);
        if(Input.GetKeyHeld('d'))
            gameObject.Translate(speed,0);
        if(Input.GetKeyHeld('w'))
            gameObject.Translate(0, -speed);
        if(Input.GetKeyHeld('s'))
            gameObject.Translate(0,speed);

        //SELF ADDED
        if(Input.GetKeyHeld('+'))
            gameObject.Scale(1.1f, 1.1f);
        if(Input.GetKeyHeld('-'))
            gameObject.Scale(0.9f, 0.9f);
        if(Input.IsMouseDown()){
            gameObject.Translate(Input.GetMouseX(), Input.GetMouseY());
        }
        if(Input.GetKeyPressed('m')){
            gameObject.MoveTo(0,0);
        }
        //END OF SELF ADDED
        if(Input.GetKeyPressed('c')){
            GatorEngine.Create(new GameObject());
            System.out.println(GatorEngine.OBJECTLIST.size());
        }

        if(Input.GetKeyPressed('r')){
            GatorEngine.Delete(this.gameObject);
            System.out.println(GatorEngine.OBJECTLIST.size());
        }


    }


}
