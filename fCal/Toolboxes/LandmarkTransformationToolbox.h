#ifndef LANDMARKTRANSFORMATIONTOOLBOX_H
#define LANDMARKTRANSFORMATIONTOOLBOX_H

#include "ui_LandmarkTransformationToolbox.h"

#include "AbstractToolbox.h"
#include "PlusConfigure.h"


#include <QWidget>

class LandmarkTransformationWidget;

/*! \class LandmarkTransformationToolbbox 
* \brief Landmark Transformation toolbox class
* \ingroup PlusAppFCal
*/
class LandmarkTransformationToolbox :
	 public QWidget, public AbstractToolbox
{
	Q_OBJECT

public:
	LandmarkTransformationToolbox(fCalMainWindow* aParentMainWindow, Qt::WFlags aFlags = 0);
   ~LandmarkTransformationToolbox();


  virtual void OnActivated();
  virtual void OnDeactivated();
  virtual void Reset();
  virtual void RefreshContent();
  virtual void SetDisplayAccordingToState();


protected:
	/*! core window of the module */ 
	LandmarkTransformationWidget* m_TransformWidget;
	QPushButton* but;
	/*! Index of current landmark */
	int m_CurrentLandmarkIndex;

protected slots:
    void OpenTransform();
	void CloseTransform();
    void AddRecordedPoint();

protected:
  Ui::LandmarkTransformationToolbox ui;

};

#endif
