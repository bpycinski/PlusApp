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
#include <QDateTime>
#include <QFileDialog>
#include <stdexcept>
#include <QMessageBox>

/** Constructor. Napisany przez ludzi z plusa */
LandmarkTransformationWidget::LandmarkTransformationWidget(QWidget* aParent) 
: QWidget(aParent)
, m_currentRowIndex(0)
, m_transformMatrix(0)
{
  ui.setupUi(this);
  connect (ui.pushButton_record, SIGNAL(clicked()), this, SLOT(recordPoint()));
  connect (ui.tableWidget, SIGNAL(cellClicked ( int , int  )),
	  this, SLOT (activeItemChanged(int , int )) );
};
/** Pusty destruktor*/
LandmarkTransformationWidget::~LandmarkTransformationWidget()
{
  // The smart pointers should clean up for up

}

/** Zdarzenie prze��czenia si� do innego elementu siatki.*/
void LandmarkTransformationWidget::activeItemChanged(int row, int column) {
	m_currentRowIndex = row;
}
/** Zdarzenie zwi�kszenia lub zmniejszenia liczby wierszy */
void LandmarkTransformationWidget::changePointsAmount(int nr) {
	ui.tableWidget->setRowCount(nr);
}
/**Zdarzenie naci�ni�cia przycisku.

Funkcja jest wywo�ywana, gdy zostanie naci�ni�ty przycisk. Implementacja �ci�gni�ta z zakladki PhantomRegistration, nie wiem, dlaczego akurat tak ma by�.
W odpowiedzi na emisj� sygna�u wywo�ywana jest funkcje addPoint (nie wiem, gdzie jest to po��czenie zrobione).
*/
void LandmarkTransformationWidget::recordPoint(void) {
	addPoint(new double [3]);
	return;
	emit(recorded());
}
/** Funkcja wywo�ywana po naci�ni�ciu przycisku. Wype�nia jeden wiersz tabeli warto�ciami pozycji z Polarisa. Wybierany jest aktualnie aktywny wiersz - mo�e zamaza� dane! 

@todo zaimplementowa� problem, gdy zlikwidowano wiersz, kt�rego kom�rka by�a aktywna i nie zaznaczono innej 
*/
bool LandmarkTransformationWidget::addPoint(double coords [3]) {
	//TODO index <= rowCount ?
	QTableWidgetItem* item;
	//dla ka�dej wsp�rz�dnej
	for (int i=0; i<3; ++i) {
		//sprawd�, czy kom�rka jest utworzona
		item = ui.tableWidget->item(m_currentRowIndex, i);
		//je�li nie, to j� utw�rz
		if (! item) {
			item = new QTableWidgetItem();
			ui.tableWidget->setItem(m_currentRowIndex, i, item);
		}
		//wpisz zawarto�� (liczba, separator, precyzja)
		item->setText(QString::number(coords[i],',',4));
	} 
	//ustaw aktywn� kom�rk� jeden rz�d ni�ej
	++m_currentRowIndex;
	ui.tableWidget->setCurrentCell(m_currentRowIndex, 0);
	return PLUS_SUCCESS;

}

/** Funkcja wyznaczaj�ca macierz transformacji. Ta funkcjonalno�� zostanie przeniesiona do nowej aplikacji */

void LandmarkTransformationWidget::computeMatrix(void) {
/*
//	if (ui->tableWidget->findItems(QString(""), Qt::MatchExactly).size() ) {
//    		QMessageBox msg(QMessageBox::Critical, QString("Wype�nij kom�rki!"), 
//			QString("W tablicy wsp�rz�dnych pozosta�y puste kom�rki. \nPodaj przynajmniej 3 pary punkt�w\n"),
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
*/
}

/** Wyczyszcza ca�� tablic� - tekst i kolory oraz pole tekstowe */
void LandmarkTransformationWidget::clear() {
	m_currentRowIndex = 0;
	m_transformMatrix = 0;
	for (int i=0; i<ui.tableWidget->rowCount(); ++i) {
		for (int j=0; j<ui.tableWidget->columnCount(); ++j) {
			QTableWidgetItem* item = ui.tableWidget->item(i, j);
			if (item) {
				item->setText("");
				item->setBackgroundColor(QColor("white"));
			}
		}
	}
	ui.textEdit->clear();
}
/** Zapisuje do pliku zawarto�� okna. Dopisuje tekst do wskazanego pliku */
void LandmarkTransformationWidget::save() {
	
	//Wybierz plik
	QString s = QFileDialog::getOpenFileName("./", "All Files (*.*)", this,				"open file dialog", "Choose a file..." );
	//Zrezygnowano z wybrania pliku
	if (s.isEmpty()) {
		QMessageBox::warning(this, tr("Uwaga"), tr("Nie wybrano pliku! Zapis mo�e zosta� utracony!"));
		return ;
	}
	//Pomocniczy potok, do kt�rego b�dzie wszystko zapisywane
	std::ostringstream oss;

	oss << "# ****************************************\n# "  
		<< QDateTime::currentDateTime().toString().toStdString() 
		<< std::endl;
	
	if (m_transformMatrix) {
		oss << "# Transformation Matrix\n";
		m_transformMatrix->Print (oss );
		oss <<"\n";
	}

	//Wypisz ca�� zawarto�� tabeli (uwaga na pute kom�rki - w tym wypadku wpisz 5 spacji
	oss <<"#MATRIX BEGIN\n";
	for (int i=0; i<ui.tableWidget->rowCount(); ++i) {
		for (int j=0; j<ui.tableWidget->columnCount(); ++j) {
			QTableWidgetItem* item = ui.tableWidget->item(i, j);
			oss << ((!item||item->text().isEmpty()) 
				    ? "     "
					: item ->text().toStdString() )  <<" ";
		}
		oss << "\n";
	}
	oss <<"#MATRIX END\n";

    //Wypisz zawarto�� pola tekstowego
	oss << ui.textEdit->toPlainText().toStdString();
	oss	<< std::endl;;
	
	//przerzu� ca�� zawarto�� z pomocniczego potoku do pliku
	//uwaga: oss jest tylko ostringstream, wi�c mo�na tylko do niego
	//zapisywa� (output). �eby z niego odczyta�, trzeba wywo�a� metod�
	//str().

	try {
		std::ofstream of (s.toStdString().c_str(), std::ios::app );
		of << oss.str() << std::endl ;
		of.close();
	} catch (std::ios_base::failure ex) {
		QMessageBox::critical (this, tr("Zapis si� nie powi�d�"), tr("Nie uda�o si� zapisa� danych. \nNieznany b��d Wej�cia/Wyj�cia. \nWykonaj printscreen i zapisz!"));
		return;
	}
	QMessageBox::information(this, tr("Zapisano"), tr("Dopisano wynik do pliku\n")+s);

}