#include "LandmarkTransformationToolbox.h"
#include "fCalMainWindow.h"
#include "vtkVisualizationController.h"
#include "LandmarkTransformationWidget.h"
#include "vtkFakeTracker.h"
LandmarkTransformationToolbox::LandmarkTransformationToolbox(fCalMainWindow* aParentMainWindow, Qt::WFlags aFlags)
: AbstractToolbox(aParentMainWindow)
, QWidget (aParentMainWindow, aFlags)
, m_CurrentLandmarkIndex(0)
, m_TransformWidget (0)
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
  connect (m_TransformWidget, SIGNAL(recorded()), this, SLOT(AddRecordedPoint()));
  connect (m_TransformWidget, SIGNAL(destroyed()), this, SLOT(CloseTransform()));
  m_TransformWidget->show();
 }


 void LandmarkTransformationToolbox::AddRecordedPoint() {
	 //double coords [] = {1.0*rand()/RAND_MAX,1.0*rand()/RAND_MAX,1.0*rand()/RAND_MAX};
	
  LOG_TRACE("PhantomRegistrationToolbox::RecordPoint"); 

  // If tracker is FakeTracker then set counter (trigger position change) and wait for it to apply the new position
  vtkDataCollector* dataCollector = m_ParentMainWindow->GetVisualizationController()->GetDataCollector();
  
  if (dataCollector)
  {
    for( DeviceCollectionConstIterator it = dataCollector->GetDeviceConstIteratorBegin(); it != dataCollector->GetDeviceConstIteratorEnd(); ++it )
    {
      vtkFakeTracker *fakeTracker = dynamic_cast<vtkFakeTracker*>(*it);
      if (fakeTracker != NULL)
      {
        fakeTracker->SetCounter(m_CurrentLandmarkIndex);
        fakeTracker->SetTransformRepository(m_ParentMainWindow->GetVisualizationController()->GetTransformRepository());
        vtkAccurateTimer::Delay(2.1 / fakeTracker->GetAcquisitionRate());
        break;
      }
    }
  }

  // Acquire point
  vtkSmartPointer<vtkMatrix4x4> stylusTipToReferenceTransformMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
  bool valid = false;
  if (m_ParentMainWindow->GetVisualizationController()->GetTransformMatrix("StylusTip", "Tracker", stylusTipToReferenceTransformMatrix, &valid) != PLUS_SUCCESS)
  {
    LOG_ERROR("No transform found between stylus and reference!");
    return;
  }

  // Add point to registration algorithm
  if (!valid)
  {
    LOG_WARNING("Invalid stylus tip to reference transform - cannot be added!");
    return;
  }

  double stylusTipPosition[3] = {stylusTipToReferenceTransformMatrix->GetElement(0,3), stylusTipToReferenceTransformMatrix->GetElement(1,3), stylusTipToReferenceTransformMatrix->GetElement(2,3) };









	 if (! m_TransformWidget) {
		 LOG_ERROR ("Nie moge zapisac punktu. Brak okna");
		 return;
	 }
	 m_TransformWidget->addPoint(stylusTipPosition);

 }


//delete b;
  //delete segmentationParamDialog;
 void LandmarkTransformationToolbox::CloseTransform()
 {
LOG_TRACE("LandmarkTransformationToolbox::CloseTransform");
  disconnect (m_TransformWidget, SIGNAL(destroyed()), this, SLOT(CloseTransform()));
  delete m_TransformWidget;
  m_TransformWidget = 0;
  // Re-connect realtime image to canvas
  if( m_ParentMainWindow->GetVisualizationController()->ConnectInput() != PLUS_SUCCESS )
  {
    LOG_WARNING("Unable to reconnect input. Image will no longer show in main window.");
  }
  this->SetDisplayAccordingToState();

 }