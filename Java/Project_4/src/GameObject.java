import java.awt.*;
import java.awt.geom.AffineTransform;
import java.awt.geom.Area;
import java.awt.geom.Point2D;
import java.awt.geom.Rectangle2D;
import java.awt.image.BufferedImage;
import java.util.ArrayList;

public class GameObject {
    private AffineTransform ObjectTransform; //the location/scale/rotation of our object
    private Shape ObjectShape; //the collider/rendered shape of this object
    private Material ObjectMaterial; //data about the fill color, border color, and border thickness
    private ArrayList<ScriptableBehavior> ObjectScripts = new ArrayList<>(); //all scripts attached to the object
    private boolean IsActive = true; //whether this gets Updated() and Draw()
    public boolean isStar = false;
    //TODO: Create the default GameObject using:
    //TODO: a default AffineTransform
    //TODO: a default Material
    //TODO: a 10x10 pixel Rectangle2D.Float Shape at coordinate (0,0)
    public GameObject(){
        ObjectTransform = new AffineTransform();
        ObjectMaterial = new Material();
        ObjectShape = new Rectangle2D.Float(0,0,10,10);
    }

    //TODO: Create the default GameObject, but with its AffineTransform translated to the coordinate (x,y)
    public GameObject(int x, int y){
        ObjectTransform = new AffineTransform();
        ObjectTransform.translate(x,y);
        ObjectMaterial = new Material();
        ObjectShape = new Rectangle2D.Float(0,0,10,10);
    }
    public GameObject(int x, int y, boolean star){
        ObjectTransform = new AffineTransform();
        ObjectTransform.translate(x,y);
        ObjectMaterial = new Material();
        ObjectShape = new Rectangle2D.Float(0,0,10,10);
        isStar = true;
    }

    //Engine Methods

    //TODO: Start all scripts on the object
    public void Start(){
        for (ScriptableBehavior objectScript : ObjectScripts) {
            objectScript.Start();
        }
    }

    //TODO: Update all scripts on the object
    public void Update(){
        if(!IsActive) return;
        for (ScriptableBehavior objectScript : ObjectScripts) {
            objectScript.Update();
        }
    }

    //TODO: Adds a new scripted behavior at runtime.
    // Start the script and add it to the list.
    public void AddScript(ScriptableBehavior NewScript){
        ObjectScripts.add(NewScript);
        NewScript.Start();
    }


    //TODO: Draw the object by
    // 1) saving the renderer's old transform.
    // 2) transforming the renderer based on the GameObject's transform.
    // 3) based on the type of material, drawing either the styled shape or the image scaled to the bounds of the shape.
    // 4) returning the old transform to the renderer.
    public void Draw(Graphics2D renderer){
        //1
        if(!IsActive)return;
        AffineTransform saved = renderer.getTransform();
        //2
        renderer.setTransform(ObjectTransform);

        if(ObjectMaterial.IsShape) {
            //styled shape?
            renderer.setColor(ObjectMaterial.Fill);
            renderer.fill(ObjectShape);
            renderer.setColor(ObjectMaterial.Stroke);
            renderer.setStroke(new BasicStroke(ObjectMaterial.StrokeWidth));
            renderer.draw(ObjectShape);
        }else{
            BufferedImage texture = ObjectMaterial.GetMaterialImage();
            if(texture != null){
                double shapeWidth = ObjectShape.getBounds2D().getWidth();
                double shapeHeight = ObjectShape.getBounds2D().getHeight();
                double imageWidth = texture.getWidth();
                double imageHeight = texture.getHeight();

                double scaleX = shapeWidth / imageWidth;
                double scaleY = shapeHeight / imageHeight;

                AffineTransform scaled = new AffineTransform();

                scaled.scale(scaleX, scaleY);
                renderer.transform(scaled);
                renderer.drawImage(texture, 0, 0, null);
                //THIS NEXT LINE I ADDED AFTER
                renderer.scale(1/scaleX, 1/scaleY);
            }
        }


        renderer.setTransform(saved);

    }

    //Movement/Collision Methods
    //TODO: Move the GameObject's transform by the provided values.
    public void Translate(float dX, float dY){
        //added the devide to account for scale and translate behaviors weird
        ObjectTransform.translate(dX/ObjectTransform.getScaleX(), dY/ObjectTransform.getScaleY());
    }

    //TODO:
    public void MoveTo(float x, float y){
        double currentX = ObjectTransform.getTranslateX();
        double currentY = ObjectTransform.getTranslateY();

        double deltaX = x - currentX;
        double deltaY = y - currentY;

        Translate((float) deltaX, (float)deltaY);
    }

    //TODO: Scale the GameObject's transform around the CENTER of its transformed shape.
    public void Scale(float sX, float sY){
        //get the center of the shape
        Rectangle2D bounds = ObjectShape.getBounds2D();
        double centerX = bounds.getCenterX();
        double centerY = bounds.getCenterY();
        AffineTransform scaleTransform = new AffineTransform();
        //transalte to origin
        scaleTransform.translate(centerX, centerY);
        //scale about origin (center)
        scaleTransform.scale(sX, sY);
        //transalte back to its original spot after scale and it should be good!
        scaleTransform.translate(-centerX, -centerY);
        //combine all those transformations
        ObjectTransform.concatenate(scaleTransform);
    }

    //TODO: Returns true if the two objects are touching
    // i.e., the intersection of their Areas is not empty)
    public boolean CollidesWith(GameObject other){

        Area intersection1 = new Area(ObjectShape);
        Area intersection2 = new Area(other.ObjectShape);

        intersection1.transform(ObjectTransform);
        intersection2.transform(other.ObjectTransform);
        intersection1.intersect(intersection2);
        return !intersection1.isEmpty();//added so the project compiles
    }

    //TODO:Should return true if the transformed shape contains the point
    public boolean Contains(Point2D point){
        Area a1 = new Area(ObjectShape);
        a1.transform(ObjectTransform);
        return a1.contains(point);
       //added so the project compiles
    }


    public void setStar(boolean val){
        isStar = val;
    }
    //Getters and Setters
    //TODO: Done for you!
    public AffineTransform GetObjectTransform() {
        return ObjectTransform;
    }

    public void SetObjectTransform(AffineTransform objectTransform) {
        this.ObjectTransform = objectTransform;
    }

    public Shape GetShape() {
        return ObjectShape;
    }

    public void SetShape(Shape shape) {
        this.ObjectShape = shape;
    }

    public Material GetObjectMaterial() {
        return ObjectMaterial;
    }

    public void SetObjectMaterial(Material objectMaterial) {
        this.ObjectMaterial = objectMaterial;
    }

    public ArrayList<ScriptableBehavior> GetObjectScripts() {
        return ObjectScripts;
    }

    public void SetObjectScripts(ArrayList<ScriptableBehavior> objectScripts) {
        this.ObjectScripts = objectScripts;
    }

    public boolean IsActive() {
        return IsActive;
    }

    public void SetActive(boolean active) {
        this.IsActive = active;
    }

}
