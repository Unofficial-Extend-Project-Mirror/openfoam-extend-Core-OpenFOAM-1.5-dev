package PatchTool.Util;

import javax.media.j3d.*;
import javax.vecmath.*;
import com.sun.j3d.utils.behaviors.mouse.*;

public class VBZMouseZoom extends MouseZoom{
  private static final double DEFAULT_SCALE = 20.0;
  private static double currentScale = DEFAULT_SCALE;
  private Vector3d translation = new Vector3d();
//  private VBZLogFrame logFrame = null;
  
  public void transformChanged(Transform3D transform) {
    transformX.get(translation);
    currentScale += translation.z;
  }

//  public void setLogFrame(VBZLogFrame logFrame){
//    this.logFrame = logFrame;
//  }

  public void setScale(double scale){
    translation.z = - currentScale + scale;
    currentScale = scale;
    transformX.set(translation);
    transformGroup.getTransform(currXform);

    if(invert){
      currXform.mul(currXform, transformX);
    }else {
      currXform.mul(transformX, currXform);
    }
    transformGroup.setTransform(currXform);
    
//    if(logFrame != null){
//      logFrame.printLog("scale:"+ currentScale);
//    }
  }

  public void resetValue(){
    currentScale = DEFAULT_SCALE;
    setScale(21.0);
  }

  public double getScale(){
    return currentScale;
  }
}
