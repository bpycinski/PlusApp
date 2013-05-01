#include "LandmarkTransformationToolbox.h"
#include "fCalMainWindow.h"

LandmarkTransformationToolbox::LandmarkTransformationToolbox(fCalMainWindow* aParentMainWindow, Qt::WFlags aFlags)
: AbstractToolbox(aParentMainWindow)
, QWidget (aParentMainWindow, aFlags)
{
	ui.setupUi(this);
}

LandmarkTransformationToolbox::~LandmarkTransformationToolbox(void)
{
}


 void LandmarkTransformationToolbox::OnActivated() {}

 void LandmarkTransformationToolbox::OnDeactivated() {}

 void LandmarkTransformationToolbox::Reset() {}

 void LandmarkTransformationToolbox::RefreshContent() {}

 void LandmarkTransformationToolbox::SetDisplayAccordingToState() {}
