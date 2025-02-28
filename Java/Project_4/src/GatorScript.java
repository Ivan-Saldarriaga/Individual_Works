public class GatorScript extends ScriptableBehavior {
        //An example script; this one will look for character input,
// and translate whatever game object it is on
        int x,y,width,height;
        //It is also customizable! Speed can change.
    float v = 0;
    float g = 2f;
    float jump = -20f;
    float maxFall = 20f;
    GatorScript(int x, int y, int width, int height, GameObject g) {
            super(g);
            this.x = x;
            this.y = y;
            this.width = width;
            this.height = height;
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
            if(Input.GetKeyPressed(' ') && GatorEngine.currentState == GameState.PLAYING){
                v = jump;
            }
            //apply gravity!
            v += g;
            if(v > maxFall){
                v = maxFall;
            }
            gameObject.Translate(0, (int) v);
            if(gameObject.GetObjectTransform().getTranslateY() > 800){
                gameObject.MoveTo((float)gameObject.GetObjectTransform().getTranslateX(), 800);
                v = 0;
            }


        }






}
