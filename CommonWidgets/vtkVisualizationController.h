/*=Plus=header=begin======================================================
Program: Plus
Copyright (c) Laboratory for Percutaneous Surgery. All rights reserved.
See License.txt for details.
=========================================================Plus=header=end*/ 

#ifndef __vtkVisualizationController_h
#define __vtkVisualizationController_h

#include "PlusCommon.h"
#include "PlusVideoFrame.h"
#include "vtkCamera.h"
#include "vtkDataCollector.h"
#include "vtkImageActor.h"
#include "vtkImageVisualizer.h"
#include "vtkObject.h"
#include "vtk3DObjectVisualizer.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderer.h"
#include "vtkTransformRepository.h"

#include <QObject>

class vtkMatrix4x4;
class vtkSTLReader;
class vtkTransform;
class vtkXMLDataElement;

class QTimer;
class QResizeEvent;
class QVTKWidget;

//-----------------------------------------------------------------------------

/*! \class vtkVisualizationController
\brief Class that is responsible for managing a connection with tracked data and managing the visualization of said data

Usage: Instantiate, set the QVTKCanvas that is to be managed by this visualizer the call Initialize function. Updating the visualization is done by attaching Update() to a QTimer (self-managed). 
Before calling this, force the data collector to provide new data by calling GetDataCollector()->Modified() function.

It has three modes, DISPLAY_MODE_2D, DISPLAY_MODE_3D and DISPLAY_MODE_NONE. In DISPLAY_MODE_2D it shows only the video input in the whole window. In DISPLAY_MODE_3D, all the devices and 
the image is visible (that are defined in the device set configuration file's Rendering element). In DISPLAY_MODE_NONE the canvas is hidden and all renderers are detached.

Devices and objects can be shown and hidden (HideAll(), ShowAllObjects(), ShowObjectsByCoordinateFrame(), ShowInput(), ShowResult()). Internally, this class forwards these requests to the underlying visualization classes.

\ingroup PlusAppCommonWidgets
*/
class vtkVisualizationController : public QObject, public vtkObject
{
  Q_OBJECT

public:
  enum DISPLAY_MODE {
    DISPLAY_MODE_2D,
    DISPLAY_MODE_3D,
    DISPLAY_MODE_NONE
  };

  /*! New */
  static vtkVisualizationController *New();

  /*! Start data collection */
  PlusStatus StartDataCollection();

  /* Stop data collection and disconnect collector */
  PlusStatus StopAndDisconnectDataCollector();

  /*!
  * Hide all tools, other models and the image from main canvas
  */
  PlusStatus HideAll();

  /*! Hide the renderer */
  PlusStatus HideRenderer();

  /*!
  * Forward the request to the both visualizers
  */
  PlusStatus AssignDataCollector(vtkDataCollector* aCollector);

  /*!
  * Forward the request to the 3D visualizer
  * \param aOn Show if true, else hide
  */
  PlusStatus ShowInput(bool aOn);

  /*!
  * Show or hide result points
  * \param aOn Show if true, else hide
  */
  PlusStatus ShowResult(bool aOn);

  /*!
  * Forward the ShowObjectById request to the 3D visualizer
  * \param aModelId Model ID to operate on
  * \param aOn Show if true, else hide
  */
  PlusStatus ShowObjectById(const char* aModelId, bool aOn);

  /*!
  * Forward the GetObjectById request to the 3D visualizer
  * \param aModelId Model ID to operate on
  */
  vtkDisplayableObject* GetObjectById( const char* aId );

  /*!
  * Forward the ShowAllObjects request to the 3D visualizer
  * \param aShow Show if true, else hide
  */
  PlusStatus ShowAllObjects( bool aShow );

  /*!
  * Enable/disable image mode
  * \param aOn Image mode flag - true: show only the image and interactions are off - false: show all toola and the image and interactions are on
  */
  PlusStatus SetVisualizationMode(DISPLAY_MODE aMode);

  /*!
  * Show or hide the MF orientation markers
  * \param aEnable Show/Hide
  */
  PlusStatus ShowOrientationMarkers( bool aShow );

  /*!
  * Dump video and tracker buffers to a given directory
  * \param aDirectory Destination directory
  */
  PlusStatus DumpBuffersToDirectory(const char* aDirectory);

  /*!
  * Return acquisition timer (to be able to connect actions to it)
  * \return Acquisition timer object
  */
  QTimer* GetAcquisitionTimer() { return this->AcquisitionTimer; };

  /*!
  Acquire transform matrix from tracking and provide string containing the translation part
  /param aTransformTranslationString Out parameter for the position string
  /param aValid True if the transform is valid, false otherwise (optional parameter)
  */
  PlusStatus GetTransformTranslationString(const char* aTransformFrom, const char* aTransformTo, std::string &aTransformTranslationString, bool* aValid = NULL);
  /*!
  Acquire transform matrix from tracking and provide string containing the translation part
  /param aTransformTranslationString Out parameter for the position string
  /param aValid True if the transform is valid, false otherwise (optional parameter)
  */
  PlusStatus GetTransformTranslationString(PlusTransformName aTransform, std::string &aTransformTranslationString, bool* aValid = NULL);

  /*!
  Acquire transform matrix from tracking
  /param aOutputMatrix Out parameter for the transform matrix
  /param aValid True if the transform is valid, false otherwise (optional parameter)
  */
  PlusStatus GetTransformMatrix(const char* aTransformFrom, const char* aTransformTo, vtkMatrix4x4* aOutputMatrix, bool* aValid = NULL);
  /*!
  Acquire transform matrix from tracking
  /param aOutputMatrix Out parameter for the transform matrix
  /param aValid True if the transform is valid, false otherwise (optional parameter)
  */
  PlusStatus GetTransformMatrix(PlusTransformName aTransform, vtkMatrix4x4* aOutputMatrix, bool* aValid = NULL);

  /*!
  Check if a transform exists in transform repository
  /param aUseLatestTrackedFrame Get latest tracked frame and set its transforms to transform repository before checking
  */
  PlusStatus IsExistingTransform(const char* aTransformFrom, const char* aTransformTo, bool aUseLatestTrackedFrame = true);

  /*! Function to handle resize events */
  void resizeEvent( QResizeEvent* aEvent );

  /*!
  * Set MF orientation in 2D mode
  * \param aNewOrientation The new MF orientation
  */
  PlusStatus SetScreenRightDownAxesOrientation(US_IMAGE_ORIENTATION aOrientation = US_IMG_ORIENT_MF);

  /*!
  * Read from the active configuration to initialize any config based data dependency
  */
  PlusStatus ReadConfiguration(vtkXMLDataElement* aXMLElement);

  /*!
  * Save the current device set configuration into XML
  */
  PlusStatus WriteConfiguration(vtkXMLDataElement* aXMLElement);

  /*!
  * Set the image visualizer ROI region
  * \param xMin min x bounds of ROI
  * \param xMax max x bounds of ROI
  * \param yMin min y bounds of ROI
  * \param yMax max y bounds of ROI
  */
  PlusStatus SetROIBounds(int xMin, int xMax, int yMin, int yMax);

  /*!
  * Enable/disable the image visualizer ROI visualization
  * \param aEnable enable/disable flag
  */
  PlusStatus EnableROI(bool aEnable);

  /*!
  * Pass the request to the image visualizer
  * \param aPointList x/y location for labels
  */
  PlusStatus SetWireLabelPositions(vtkPoints* aPointList);

  /*!
  * Enable/disable wire label visualization
  * \param aEnable enable/disable flag
  */
  PlusStatus EnableWireLabels(bool aEnable);

  /*!
  * Enable or disable the volume actor in 3D mode
  * \param aEnable enable/disable flag
  */
  PlusStatus EnableVolumeActor( bool aEnable );

  /*!
  * Set the volume actor mapper
  * \param aContourMapper new mapper to use
  */
  PlusStatus SetVolumeMapper( vtkPolyDataMapper* aContourMapper );

  /*!
  * Set the volume actor colour
  * \param r red value
  * \param g green value
  * \param b blue value
  */
  PlusStatus SetVolumeColor( double r, double g, double b );

  /*! Disconnect the image input */
  PlusStatus DisconnectInput();

  /*! Connect the image input to the DataCollector output */
  PlusStatus ConnectInput();

  /*! Clear the transform repository */
  PlusStatus ClearTransformRepository();

  /*! Reset the visualization */
  PlusStatus Reset();

  /*! Set the selected channel */
  void SetSelectedChannel(vtkPlusChannel* aChannel);

protected slots:
  /*!
  * Forward any updates to members that require it
  */
  PlusStatus Update();

public:
  // Set/Get macros for member variables
  PlusStatus SetAcquisitionFrameRate(int aFrameRate); 
  vtkGetMacro(AcquisitionFrameRate, int); 
  vtkGetObjectMacro(DataCollector, vtkDataCollector);
  vtkGetObjectMacro(InputPolyData, vtkPolyData);
  vtkGetObjectMacro(ResultPolyData, vtkPolyData);
  virtual vtkSmartPointer<vtkTransformRepository> GetTransformRepository();

  vtkSetObjectMacro(DataCollector, vtkDataCollector);   

  vtkRenderer* GetCanvasRenderer();

  bool Is2DMode();
  bool Is3DMode();

  void SetCanvas(QVTKWidget* aCanvas);

  void SetInput( vtkImageData * input );

protected:
  vtkGetObjectMacro(ImageVisualizer, vtkImageVisualizer);
  vtkGetObjectMacro(PerspectiveVisualizer, vtk3DObjectVisualizer);
  vtkSetObjectMacro(ImageVisualizer, vtkImageVisualizer);
  vtkSetObjectMacro(PerspectiveVisualizer, vtk3DObjectVisualizer);
  vtkSetObjectMacro(InputPolyData, vtkPolyData);
  vtkSetObjectMacro(ResultPolyData, vtkPolyData);
  virtual void SetTransformRepository(vtkSmartPointer<vtkTransformRepository> aRepo);

  vtkImageActor* GetImageActor();

  QVTKWidget* GetCanvas() const { return Canvas; }

protected:
  /*!
  * Constructor
  */
  vtkVisualizationController();

  /*!
  * Destructor
  */
  virtual ~vtkVisualizationController();	

protected:
  /*! 2D visualizer */
  vtkImageVisualizer* ImageVisualizer;

  /*! 3D visualizer */
  vtk3DObjectVisualizer* PerspectiveVisualizer;

  /*! Reference to the canvas */
  QVTKWidget* Canvas;

  /*! Data collector object */
  vtkDataCollector*	DataCollector;

  /*! Timer for acquisition */
  QTimer*	AcquisitionTimer;

  /*! Polydata holding the result points (eg. stylus tip, segmented points) */
  vtkPolyData* ResultPolyData;

  /*! Polydata holding the input points */
  vtkPolyData* InputPolyData;

  /*! Flag indicating if the visualization is in image mode (show only the image and interactions are off) or device display mode (show all tools and the image and interactions are on) or none */
  DISPLAY_MODE CurrentMode;

  /*! Desired frame rate of synchronized recording */
  int AcquisitionFrameRate;

  /*! Transform repository to store and handle all transforms */
  vtkSmartPointer<vtkTransformRepository> TransformRepository;

  vtkPlusChannel* SelectedChannel;
};

#endif  // __vtkVisualizationController_h
