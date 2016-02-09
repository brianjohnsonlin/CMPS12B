import gpdraw.*;

public class EulerSpirals{
	private DrawingTool pen;
	private SketchPad canvas;
	private int turn = 0;
	
    public EulerSpirals() {
    	canvas=new SketchPad(500,500);
    	pen=new DrawingTool(canvas);
    	pen.setDirection(0);
    	for(int i = 0; i < 15000; i++){
    		draw();
    	}
    }
    
    public void draw(){
    	pen.move(10);
    	pen.turnLeft(turn);
    	turn+=17;
	}
	
	public static void main(String[] args){
		EulerSpirals es = new EulerSpirals();
	}
}