#ifndef LandmarkTransformationWidget_H
#define LandmarkTransformationWidget_H

#include "PlusConfigure.h"
#include "ui_LandmarkTransformationWidget.h"
//#include "vtkSmartPointer.h"    // Required for smart pointer internal ivars.
#include <QWidget>

// Forward Qt class declarations
class Ui_SimpleView;

// Forward VTK class declarations
//class vtkQtTableView;

class vtkPoints;
class vtkMatrix4x4;


class LandmarkTransformationWidget : public QWidget
{
  Q_OBJECT

public:

  // Constructor/Destructor
  LandmarkTransformationWidget(QWidget* aParent=0) ; 
  ~LandmarkTransformationWidget();

public slots:

//  virtual void slotOpenFile();
//  virtual void slotExit();

protected:
   
protected slots:
    void changePointsAmount(int nr);
    void computeMatrix(void);
	void clear();
private:

	vtkPoints* m_PointsSource;
	vtkPoints* m_PointsTarget;
	vtkMatrix4x4* m_transformMatrix; 
//  vtkSmartPointer<vtkQtTableView>         TableView;
    
  // Designer form
	Ui::LandmarkTransformationWidget ui;
};

#endif // LandmarkTransformationWidget
