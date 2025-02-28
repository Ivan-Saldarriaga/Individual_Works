public class ScrollingBackgroundScript extends ScriptableBehavior {
    private float speedMultiplier; // Speed of scrolling
    private GameObject gameObject;

    public ScrollingBackgroundScript(float speedMultiplier, GameObject gameObject) {
        super(gameObject);
        this.speedMultiplier = Application.speedMultiplier;
        this.gameObject = gameObject;
    }
    @Override
    public void Start() {

    }

    @Override
    public void Update() {

        if (gameObject != null) {
            if(GatorEngine.currentState == GameState.GAME_OVER){
                return;
            }
            gameObject.Translate(-Application.speedMultiplier, 0);
            // If the background has moved completely off-screen to the left, reset its position
            if (gameObject.GetObjectTransform().getTranslateX() + 1200 < 0) {
                gameObject.MoveTo(0, 0); // Reposition to the right side
            }
        }
    }
}
