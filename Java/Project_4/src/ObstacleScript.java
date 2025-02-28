public class ObstacleScript extends ScriptableBehavior {
    //private GameObject gameObject; // Ensure this is not null
    private float speedMultiplier = 0f;
    public ObstacleScript(float speedMultiplier, GameObject gameObject) {
        super(gameObject);
        //this.gameObject = gameObject; // Set the gameObject reference
        this.speedMultiplier = Application.speedMultiplier;
        // Other initialization logic
    }
    @Override
    public void Start(){

    }
    @Override
    public void Update() {
        if(GatorEngine.currentState == GameState.GAME_OVER){
            return;
        }
        if (gameObject == null) {

            System.out.println("GameObject is null in ObstacleScript!");
            return; // Prevent further operations if gameObject is null
        }

        // Logic for obstacle movement
        gameObject.Translate(-Application.speedMultiplier, 0);
    }
    public boolean isStar(){
        return false;
    }
}
