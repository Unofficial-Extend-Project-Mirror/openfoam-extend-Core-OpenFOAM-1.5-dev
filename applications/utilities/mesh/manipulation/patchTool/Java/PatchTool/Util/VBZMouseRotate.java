package PatchTool.Util;

import javax.media.j3d.*;
import javax.vecmath.*;
import com.sun.j3d.utils.behaviors.mouse.*;
import java.lang.Math;

public class VBZMouseRotate extends MouseRotate{
//  private VBZLogFrame logFrame = null;
  private static double current_alpha;
  private static double current_beta;
  private static double old_alpha;
  private static double old_beta;
  
  public void resetValue(){
    transformGroup.setTransform(new Transform3D());
    current_alpha = 0.0;
    current_beta = 0.0;
    old_alpha = 0.0;
    old_beta = 0.0;
  }

//  public void setLogFrame(VBZLogFrame logFrame){
//    this.logFrame = logFrame;
//  }

  public void setDirection(double alpha, double beta){
    /*
    if((old_alpha - alpha == 0) && (old_beta - beta == 0)){
      return;
    }
    transformX.rotX(-current_alpha * Math.PI / 180);
    transformY.rotY(-current_beta * Math.PI / 180);
    */
    transformY.rotY((beta - old_beta) * Math.PI / 180);
    transformX.rotX((alpha - old_alpha) * Math.PI / 180);

    transformGroup.getTransform(currXform);
                    
    //Vector3d translation = new Vector3d();
    //Matrix3f rotation = new Matrix3f();
    Matrix4d mat = new Matrix4d();
                    
    // Remember old matrix
    currXform.get(mat);
                    
    // Translate to origin
    currXform.setTranslation(new Vector3d(0.0,0.0,0.0));
    if (invert) {
      currXform.mul(currXform, transformX);
      currXform.mul(currXform, transformY);
    } else {
      currXform.mul(transformX, currXform);
      currXform.mul(transformY, currXform);
    }
                    
    // Set old translation back
    Vector3d translation = new 
      Vector3d(mat.m03, mat.m13, mat.m23);
    currXform.setTranslation(translation);
                    
    // Update xform
    transformGroup.setTransform(currXform);

    transformChanged( currXform );

    old_alpha = alpha;
    old_beta = beta;
    current_alpha += alpha;
    current_beta += beta;
  }

  public double getAlpha(){
    return current_alpha;
  }

  public double getBeta(){
    return current_beta;
  }
}
