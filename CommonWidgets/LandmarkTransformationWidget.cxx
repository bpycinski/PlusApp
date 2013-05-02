#include "ui_LandmarkTransformationWidget.h"
#include "LandmarkTransformationWidget.h"
#include <QMessageBox>
#include <sstream>
#include <QList>
#include <vtkLandmarkTransform.h>
#include <vtkMatrix4x4.h>
#include <vtkPoints.h>
#include <vector>
#include <vtkSmartPointer.h>
#include <iomanip>

// Constructor
LandmarkTransformationWidget::LandmarkTransformationWidget(QWidget* aParent) 
: QWidget(aParent)
, m_currentRowIndex(0)
{
  ui.setupUi(this);
  connect (ui.pushButton_record, SIGNAL(clicked()), this, SLOT(recordPoint())); 
};

LandmarkTransformationWidget::~LandmarkTransformationWidget()
{
  // The smart pointers should clean up for up

}


void LandmarkTransformationWidget::changePointsAmount(int nr) {
	ui.tableWidget->setRowCount(nr);
}

void LandmarkTransformationWidget::recordPoint(void) {
	emit(recorded());
}

bool LandmarkTransformationWidget::addPoint(double coords [3]) {
	//TODO index <= rowCount ?
	QTableWidgetItem* item;
	for (int i=0; i<3; ++i) {
		item = ui.tableWidget->item(m_currentRowIndex, i+3);
		if (item) {
			item->setText(QString::number(coords[i],',',3));
		} else {
			ui.tableWidget->setItem(m_currentRowIndex,i+3,new QTableWidgetItem(QString::number(coords[i],',',3)));
		}
	} 
	++m_currentRowIndex;
	return PLUS_SUCCESS;

}


void LandmarkTransformationWidget::computeMatrix(void) {

//	if (ui->tableWidget->findItems(QString(""), Qt::MatchExactly).size() ) {
//    		QMessageBox msg(QMessageBox::Critical, QString("Wype³nij komórki!"), 
//			QString("W tablicy wspó³rzêdnych pozosta³y puste komórki. \nPodaj przynajmniej 3 pary punktów\n"),
//			QMessageBox::Ok, this) ;
//		return;
//	}
	bool result=true;
	bool global_result = true;

	for (int i=0; i<ui.tableWidget->rowCount(); ++i) {
		for (int j=0;j<6;++j) {
			result=true;
			global_result = true;
			QTableWidgetItem* item = ui.tableWidget->item(i,j);
			if (!item || item->text().isEmpty()) {
				global_result = false;
				if (! item) 
					ui.tableWidget->setItem(i,j,new QTableWidgetItem(""));
				ui.tableWidget->item(i,j)->setBackgroundColor(QColor("yellow"));
			}
			else {
				item ->text().toDouble(&result);
				if (!result) {
					ui.tableWidget->item(i,j)->setBackgroundColor(QColor("yellow"));
					global_result = false;
				} else {
					ui.tableWidget->item(i,j)->setBackgroundColor(QColor("white"));
				}
			}
		}
	}

	if (!global_result) {
		return;
	}
	//stworz kolekcje punktow na podstawie zczytanych
	m_PointsSource = vtkPoints::New();
	m_PointsTarget = vtkPoints::New();
	global_result = true;
	//sla kazdego wiersza
	for (int i=0; i<ui.tableWidget->rowCount(); ++i) {
		//wektory przechowujace wspolrzedne dwoch punktow
		double sourcePoint [3];
		double targetPoint[3];
		result = true;
		//pobierz parami kolejne 3 wspolrzedne
		for (int j=0;j<3;++j) {
			sourcePoint[j] = ui.tableWidget->item(i,j)->text().toDouble(&result);
			global_result &= result; //konrola bledu
			targetPoint[j] = ui.tableWidget->item(i,j+3)->text().toDouble(&result);
			global_result &= result;  //kontrola bledu
		}
		m_PointsSource->InsertNextPoint(sourcePoint);
		m_PointsTarget->InsertNextPoint(targetPoint);
	}
	if (! global_result) {
		return ;
	}

	vtkSmartPointer<vtkLandmarkTransform> transform = vtkLandmarkTransform::New(); 
	transform->SetSourceLandmarks(m_PointsSource); 
	transform->SetTargetLandmarks(m_PointsTarget); 
	transform->SetModeToSimilarity(); // transform mode similarity 

	transform->Update();
	transform->Modified();
	transform->MakeTransform();
	m_transformMatrix= transform->GetMatrix();

	m_transformMatrix->Print(std::cout);

	std::stringstream  matrix_ss;
	std::string resultstring;
	for (int i=0;i<4;++i) {
		for (int j=0;j<4;++j) {
			matrix_ss.width(15);
			matrix_ss  << m_transformMatrix->GetElement(i, j) << " ";

		} 
		matrix_ss << "\n";
	}
	resultstring = matrix_ss.str();
	std::cout << resultstring;
	ui.textEdit->setText(QString(resultstring.c_str()));

	//zapisz macierz na ekran

		
	
	//QList<QTableWidgetItem>* listaZlych = ui->tableWidget->findItems(QString("^[0-9]*([,\.][0-9]*)?$"),Qt::MatchRegExp);
	//if (listaZlych.size() != 
	m_PointsSource->Delete();
	m_PointsTarget->Delete();

}

void LandmarkTransformationWidget::clear() {
	m_currentRowIndex = 0;
	for (int i=0; i<ui.tableWidget->rowCount(); ++i) {
		for (int j=0; j<ui.tableWidget->columnCount(); ++j) {
			QTableWidgetItem* item = ui.tableWidget->item(i, j);
			if (item) {
				item->setText("");
				item->setBackgroundColor(QColor("white"));
			}
		}
	}
}