import java.awt.*;
import java.awt.geom.Ellipse2D;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;

public class Application {
    //This is where we can implement applications in the Gator Engine.
    private static List<GameObject> obstacles = new ArrayList<>();
    public static GameObject gatorHead;
    public static GameObject bubble;
    public static int count = 0;
    public static int gameState = 1;
    public static int lastGameState = 1;
    //We don't have an "editor mode", so we can only manually set up our application.
    public static float minSpawnInterval = 3000; // 2 seconds in milliseconds
    public static float maxSpawnInterval = 5000; // 6 seconds in milliseconds
    public static long lastSpawnTime = System.currentTimeMillis();
    public static long nextSpawnInterval = getRandomSpawnInterval(); // Initialize with a random value
    public static boolean isProtecteed = false;
    //Materials
    public static Material backgroundMaterial = new Material("resources/background.png");
    public static Material title = new Material("resources/title.png");
    public static Material transparent = new Material(new Color(0,0,0,0), new Color(0,0,0,0), 0);
    public static Material bubbleMaterial = new Material("resources/Bubble.png");
    public static Material swimmerMaterial = new Material("resources/Swimmer.png");
    public static Material garbageMaterial = new Material("resources/Garbage.png");
    public static Material rockMaterial = new Material("resources/Rock.png");

    //Here is an example of creating a GameObject and giving it a behavior (moving with WASD).
    public static void CreateStartScreen() {
        //System.out.println("STARTED!");
        GameObject g = new GameObject(0,0);//create a position
        g.SetShape(new Rectangle(0,0,1200,800));
        g.SetObjectMaterial(new Material("resources/background.png"));
        GatorEngine.Create(g);


        GameObject g2 = new GameObject(0,0);//create a position
        g2.SetShape(new Rectangle(0,0,1200,800));
        g2.SetObjectMaterial(title);
        //g2.AddScript(new ClickingScript(g2));
        GatorEngine.Create(g2);
        GameObject g3 = new GameObject(707,400);//create a position
        g3.SetShape(new Rectangle(0,0,298,140));
        g3.SetObjectMaterial(new Material(new Color(0,0,0,0), new Color(0,0,0,0), 0));
        g3.AddScript(new ClickingScript(707,400,298,140, g3));
        GatorEngine.Create(g3);

    }
    //spawner variabels
    public static float speedMultiplier = 5.0f;
    public static float globalSpeed;


    public static void UpdatePlaying(){
        speedMultiplier += .5f; // Gradually increase speed
        minSpawnInterval = Math.max(1500, minSpawnInterval - 35); // Adjust spawn intervals moderately
        maxSpawnInterval = Math.max(2500, maxSpawnInterval - 75);
        globalSpeed = speedMultiplier;
        SpawnObstacle();
        nextSpawnInterval = getRandomSpawnInterval();

        // Increase the spawn speed and multiplier after each spawn
        //spawnInterval = Math.max(0.5f, spawnInterval - 0.1f); // Ensure a minimum of 0.5 seconds interval
    }
    public static void SpawnObstacle(){
        count++;
        int[] positions = {100,400,700};
        int yPosition = positions[(int) (Math.random() * positions.length)];
        GameObject obstacle;
        if(count == 5){
            count = 0;
            //System.out.println("star appeared?");
            obstacle = new GameObject(1200,0, true);
            obstacle.SetShape(new Ellipse2D.Float(0,0,100,100));
            obstacle.SetObjectMaterial(bubbleMaterial);
            obstacle.AddScript(new ObstacleScript(speedMultiplier, obstacle));
            GatorEngine.Create(obstacle);
            obstacles.add(obstacle);
            return;
        }
        else {
            if (yPosition == 100) { // Top
                obstacle = new GameObject(1200, 0);//create a position
                obstacle.SetShape(new Ellipse2D.Float(0, 0, 363, 260));
                obstacle.SetObjectMaterial(swimmerMaterial);
            } else if (yPosition == 400) { // Middle
                obstacle = new GameObject(1200, 400 - (176 / 2));//create a position
                obstacle.SetShape(new Ellipse2D.Float(0, 0, 137, 176));
                obstacle.SetObjectMaterial(garbageMaterial);

            } else { // Bottom
                obstacle = new GameObject(1200, 800 - 255);//create a position
                obstacle.SetShape(new Ellipse2D.Float(0, 0, 155, 255));
                obstacle.SetObjectMaterial(rockMaterial);
            }
            //obstacle.MoveTo(1200, yPosition);
            obstacle.AddScript(new ObstacleScript(speedMultiplier, obstacle));
            GatorEngine.Create(obstacle);
            obstacles.add(obstacle);
        }
    }
    public static boolean CheckCollision() {
        if(gatorHead.GetObjectTransform().getTranslateY() >= 730){
            GatorEngine.currentState = GameState.GAME_OVER;
            gameState = 3;
            return true;
        }
        for (int i = 0; i < obstacles.size(); i++) {
            GameObject obstacle = obstacles.get(i);

            if (gatorHead != null && obstacle != null) {
                // Check for collision between gator head and the obstacle
                if (gatorHead.CollidesWith(obstacle)) {
                    //System.out.println(obstacle.isStar);
                    if (obstacle.isStar) {
                        // If the obstacle is a "star," remove it and delete it
                       // System.out.println("Star collected!");
                        obstacles.remove(i); // Remove from the list
                        GatorEngine.Delete(obstacle); // Delete the object from the engine
                        bubble.SetObjectMaterial(bubbleMaterial);
                        isProtecteed = true;
                        i--; // Adjust the index after removal
                    } else {
                        // Regular obstacle collision, game over
                        if(isProtecteed){
                            isProtecteed = false;
                            bubble.SetObjectMaterial(transparent);
                            obstacles.remove(i);
                            GatorEngine.Delete(obstacle);
                            i--;
                        }else {
                            //System.out.println("Collision detected! Game over.");
                            GatorEngine.currentState = GameState.GAME_OVER;
                            gameState = 3;
                            return true; // Exit as the game is over
                        }
                    }
                }
            }
        }
        return false; // No collision
    }
    public static void PlayingScreen(){
      //  System.out.println("PLAYING!");
        GameObject g = new GameObject(0,0);//create a position
        g.SetShape(new Rectangle(0,0,1200*2,800));
        g.SetObjectMaterial(new Material("resources/background.png"));
        g.AddScript(new ScrollingBackgroundScript(speedMultiplier, g));
        GatorEngine.Create(g);
        GameObject g2 = new GameObject(0,400-115);
        g2.SetShape(new Rectangle(0,0, 380,115));
        g2.SetObjectMaterial(new Material("resources/Gator.png"));
        g2.AddScript(new GatorScript(0,400-115, 380, 115,g2));
        GatorEngine.Create(g2);
        bubble = new GameObject(280, 280);
        bubble.SetShape(new Rectangle(0,0, 120,120));
        bubble.SetObjectMaterial(new Material(new Color(255, 255, 255, 0),new Color(255,255,255,0),0));
        bubble.AddScript(new GatorScript(0,400-115, 380, 115,bubble));
        GatorEngine.Create(bubble);
        //do collision object
        //UpdatePlaying();
        //StartObstacleSpawner();
        //UpdatePlaying();


    }
    public static void ClearScreen() {
        for (GameObject e : GatorEngine.OBJECTLIST) {
            GatorEngine.Delete(e);
        }
    }
    public static void GameOverScreen(){
//        System.out.println("GAMEOVER!");
//        GameObject g = new GameObject(0,0);//create a position
//        g.SetShape(new Rectangle(0,0,1200,800));
//        g.SetObjectMaterial(new Material("resources/Background.png"));
//        GatorEngine.Create(g);


        GameObject g2 = new GameObject(0,0);//create a position
        g2.SetShape(new Rectangle(0,0,1200,800));
        g2.SetObjectMaterial(new Material("resources/End.png"));
        //g2.AddScript(new ClickingScript(g2));
        GatorEngine.Create(g2);
        GameObject g3 = new GameObject(707,515);//create a position
        g3.SetShape(new Rectangle(0,0,298,140));
        g3.SetObjectMaterial(new Material(new Color(0,0,0,0), new Color(0,0,0,0), 0));
        g3.AddScript(new ClickingScript(707,515,298,140, g3));
        GatorEngine.Create(g3);
    }
    public static void Start(){
        if(GatorEngine.currentState == GameState.START_SCREEN){
            ClearScreen();
            CreateStartScreen();
            //start screen can always be 1

        } else if (GatorEngine.currentState == GameState.PLAYING) {
            ClearScreen();
            //playing can always be 2
            //lastUpdateTime = System.currentTimeMillis();
            gatorHead = new GameObject(300, 330); // Example position
            gatorHead.SetShape(new Ellipse2D.Float(0, 0, 80, 10) {
            });
            gatorHead.SetObjectMaterial(new Material(Color.BLACK, Color.BLACK, 25));
            gatorHead.AddScript(new GatorScript(300, 330, 80, 10, gatorHead));
            GatorEngine.Create(gatorHead);
            PlayingScreen();
            gameState = 2;

        } else if (GatorEngine.currentState == GameState.GAME_OVER) {
            obstacles.clear();
            count = 0;
            isProtecteed = false;
            speedMultiplier = 5.0f;
            minSpawnInterval = 3000; // 2 seconds in milliseconds
            maxSpawnInterval = 5000;
            GameOverScreen();
            //gameover can always be 3
            gameState = 3;
        }

    }
    public static long getRandomSpawnInterval() {
        Random random = new Random();
        float scalingFactor = Math.max(1.2f, 15.0f / speedMultiplier);
        return (long)(minSpawnInterval + random.nextFloat() * (maxSpawnInterval - minSpawnInterval) / scalingFactor);}
    public static void Update() {
        if(gameState != lastGameState){
            lastGameState = gameState;
            //ClearScreen();
            Start();
        }
        if (gameState == 2) {
            long currentTime = System.currentTimeMillis();
            if (currentTime - lastSpawnTime >= nextSpawnInterval) {
                // Call UpdatePlaying() and set a new random interval
                UpdatePlaying();
                lastSpawnTime = currentTime; // Reset the last spawn time
            }
            if (CheckCollision()) {
                GatorEngine.currentState = GameState.GAME_OVER;
                //System.out.println("Game Over! Collision detected.");
            }
        }


    }
}
