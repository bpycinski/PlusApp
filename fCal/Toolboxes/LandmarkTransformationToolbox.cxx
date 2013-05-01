#include "LandmarkTransformationToolbox.h"
#include "fCalMainWindow.h"
#include "vtkVisualizationController.h"
#include "LandmarkTransformationWidget.h"
LandmarkTransformationToolbox::LandmarkTransformationToolbox(fCalMainWindow* aParentMainWindow, Qt::WFlags aFlags)
: AbstractToolbox(aParentMainWindow)
, QWidget (aParentMainWindow, aFlags)
, m_CurrentLandmarkIndex(0)
{
	ui.setupUi(this);
	connect (ui.pushButton_Start, SIGNAL(clicked()), this, SLOT(OpenTransform()));
}

LandmarkTransformationToolbox::~LandmarkTransformationToolbox(void)
{
}


 void LandmarkTransformationToolbox::OnActivated() {
   LOG_TRACE("LandmarkTransformationToolbox::OnActivated"); 
 }

 void LandmarkTransformationToolbox::OnDeactivated() {}

 void LandmarkTransformationToolbox::Reset() {}

 void LandmarkTransformationToolbox::RefreshContent() {}

 void LandmarkTransformationToolbox::SetDisplayAccordingToState() {}

 void LandmarkTransformationToolbox::OpenTransform() {
    LOG_TRACE("LandmarkTransformationToolbox::OpenTransform");

	// Disconnect realtime image from main canvas
  m_ParentMainWindow->GetVisualizationController()->SetVisualizationMode(vtkVisualizationController::DISPLAY_MODE_NONE);
  if( m_ParentMainWindow->GetVisualizationController()->DisconnectInput() != PLUS_SUCCESS )
  {
    LOG_ERROR("Unable to disconnect input. Cannot show input in SegmentationParameterDialog.");
    return;
  }

  // Show segmentation parameter dialog
  //SegmentationParameterDialog* segmentationParamDialog = new SegmentationParameterDialog(this, m_ParentMainWindow->GetVisualizationController()->GetDataCollector(), m_ParentMainWindow->GetSelectedChannel());
  //segmentationParamDialog->exec();

  m_TransformWidget = new LandmarkTransformationWidget(0);
  connect (m_TransformWidget, SIGNAL(destroyed()), this, SLOT(CloseTransform()));
  m_TransformWidget->show();
 }
  //TODO zatrzymac tutaj wykonanie biezacego watku, az zostanie zamkniete okno
  //lub cos w tym stylu, zeby potem delete widget

//delete b;
  //delete segmentationParamDialog;
 void LandmarkTransformationToolbox::CloseTransform()
 {
LOG_TRACE("LandmarkTransformationToolbox::CloseTransform");
  disconnect (m_TransformWidget, SIGNAL(destroyed()), this, SLOT(CloseTransform()));
  delete m_TransformWidget;
  // Re-connect realtime image to canvas
  if( m_ParentMainWindow->GetVisualizationController()->ConnectInput() != PLUS_SUCCESS )
  {
    LOG_WARNING("Unable to reconnect input. Image will no longer show in main window.");
  }
  this->SetDisplayAccordingToState();

 }