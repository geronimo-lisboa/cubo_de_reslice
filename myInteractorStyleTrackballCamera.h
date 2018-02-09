
#ifndef myInteractorStyleTrackballCamera_h
#define myInteractorStyleTrackballCamera_h

#include "vtkInteractionStyleModule.h" // For export macro
#include "vtkInteractorStyle.h"

class VTKINTERACTIONSTYLE_EXPORT myInteractorStyleTrackballCamera : public vtkInteractorStyle
{
public:
  static myInteractorStyleTrackballCamera *New();
  vtkTypeMacro(myInteractorStyleTrackballCamera,vtkInteractorStyle);
  void PrintSelf(ostream& os, vtkIndent indent) override;

  //@{
  /**
   * Event bindings controlling the effects of pressing mouse buttons
   * or moving the mouse.
   */
  void OnMouseMove() override;
  void OnLeftButtonDown() override;
  void OnLeftButtonUp() override;
  void OnMiddleButtonDown() override;
  void OnMiddleButtonUp() override;
  void OnRightButtonDown() override;
  void OnRightButtonUp() override;
  void OnMouseWheelForward() override;
  void OnMouseWheelBackward() override;
  //@}

  // These methods for the different interactions in different modes
  // are overridden in subclasses to perform the correct motion. Since
  // they are called by OnTimer, they do not have mouse coord parameters
  // (use interactor's GetEventPosition and GetLastEventPosition)
  void Rotate() override;
  void Spin() override;
  void Pan() override;
  void Dolly() override;

  //@{
  /**
   * Set the apparent sensitivity of the interactor style to mouse motion.
   */
  vtkSetMacro(MotionFactor,double);
  vtkGetMacro(MotionFactor,double);
  //@}

protected:
  myInteractorStyleTrackballCamera();
  ~myInteractorStyleTrackballCamera() override;

  double MotionFactor;

  virtual void Dolly(double factor);

private:
  myInteractorStyleTrackballCamera(const myInteractorStyleTrackballCamera&) = delete;
  void operator=(const myInteractorStyleTrackballCamera&) = delete;
};

#endif
