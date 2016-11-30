#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "kakurasu.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: white;");

    QPalette pal;
    pal.setColor(QPalette::Background, Qt::white);

    QFont fnt;
    fnt.setPointSize(20);
    fnt.setFamily("Arial");
    ui->tableWidget->setFont(fnt);
    ui->tableWidget_row->setFont(fnt);
    ui->tableWidget_col->setFont(fnt);

    ui->tableWidget->setVisible(false);
    ui->tableWidget_row->setVisible(false);
    ui->tableWidget_col->setVisible(false);

    QSize size = this->size();
    ui->textEdit->setFixedWidth(size.width() + 70);
    ui->textEdit->setFixedHeight(size.height() + 5);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "4x4")
    {
        arrangeTable(4);
    }
    else if(arg1 == "5x5")
    {
        arrangeTable(5);
    }
    else if(arg1 == "6x6")
    {
        arrangeTable(6);
    }
    else if(arg1 == "7x7")
    {
        arrangeTable(7);
    }
    else if(arg1 == "8x8")
    {
        arrangeTable(8);
    }
    else if(arg1 == "9x9")
    {
        arrangeTable(9);
    }
}

void MainWindow::arrangeTable(int index)
{
    ui->tableWidget->setVisible(true);
    ui->tableWidget->setRowCount(index);
    ui->tableWidget->setColumnCount(index);
    ui->tableWidget->horizontalHeader()->setDefaultSectionSize(40);
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(40);
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setVisible(false);
    ui->tableWidget->resize(ui->tableWidget->horizontalHeader()->length() + 2,
                            ui->tableWidget->verticalHeader()->length() + 2);

    ui->tableWidget_row->setVisible(true);
    ui->tableWidget_row->setRowCount(1);
    ui->tableWidget_row->setColumnCount(index);
    ui->tableWidget_row->horizontalHeader()->setDefaultSectionSize(40);
    ui->tableWidget_row->verticalHeader()->setDefaultSectionSize(40);
    ui->tableWidget_row->verticalHeader()->setVisible(false);
    ui->tableWidget_row->horizontalHeader()->setVisible(false);
    ui->tableWidget_row->resize(ui->tableWidget_row->horizontalHeader()->length() + 2,
                            ui->tableWidget_row->verticalHeader()->length() + 2);
    ui->tableWidget_row->move(ui->tableWidget->x(),
                              ui->tableWidget->y() + ui->tableWidget->verticalHeader()->length() + 10);

    ui->tableWidget_col->setVisible(true);
    ui->tableWidget_col->setRowCount(index);
    ui->tableWidget_col->setColumnCount(1);
    ui->tableWidget_col->horizontalHeader()->setDefaultSectionSize(40);
    ui->tableWidget_col->verticalHeader()->setDefaultSectionSize(40);
    ui->tableWidget_col->verticalHeader()->setVisible(false);
    ui->tableWidget_col->horizontalHeader()->setVisible(false);
    ui->tableWidget_col->resize(ui->tableWidget_col->horizontalHeader()->length() + 2,
                            ui->tableWidget_col->verticalHeader()->length() + 2);
    ui->tableWidget_col->move(ui->tableWidget->x() + ui->tableWidget_row->horizontalHeader()->length() + 10,
                              ui->tableWidget->y());
}

void MainWindow::on_pushButton_clicked()
{
    int index = ui->comboBox->currentIndex() + 3;
    ofstream out;
    out.open("tempFile.txt");
    out<<to_string(index) + "\n";
    for(int i = 0; i < ui->tableWidget->rowCount(); i++)
    {
        if(!ui->tableWidget_row->item(0,i)->text().toStdString().compare("?") && !ui->tableWidget_col->item(i,0)->text().toStdString().compare("?"))
            out<<"-1,-1\n";

        else if(!ui->tableWidget_row->item(0,i)->text().toStdString().compare("?") && ui->tableWidget_col->item(i,0)->text().toStdString().compare("?"))
            out<<ui->tableWidget_col->item(i,0)->text().toStdString() + ",-1" + "\n";

        else if(ui->tableWidget_row->item(0,i)->text().toStdString().compare("?") && !ui->tableWidget_col->item(i,0)->text().toStdString().compare("?"))
            out<<"-1," + ui->tableWidget_row->item(0,i)->text().toStdString() + "\n";

        else
            out<<ui->tableWidget_col->item(i,0)->text().toStdString() + "," +
                 ui->tableWidget_row->item(0,i)->text().toStdString() + "\n";
    }
    out.close();

    Kakurasu *kakurasu;
    kakurasu = new Kakurasu("tempFile.txt", ui->textEdit);

    kakurasu->printSolMatrix();
    kakurasu->getAllCombinations();
    kakurasu->ACAlgorithm((kakurasu->rows), (kakurasu->columns), kakurasu->checkMatrix);
    kakurasu -> UnidentifiedVariables = kakurasu->unidentifiedVariables(kakurasu->checkMatrix, kakurasu->fMatrix);

    ui->textEdit->insertPlainText("\n\n In main : number of Unidentified Variables are : " + QString::number(kakurasu -> UnidentifiedVariables) + "\n\n");
    if(kakurasu->UnidentifiedVariables == 0)
    {
        ui->textEdit->insertPlainText("\n Problem is Solved Using AC Algorithm");
    }
    else
    {
        kakurasu->stm(kakurasu->fValueList, kakurasu->checkMatrix, kakurasu->fMatrix);
        fflush(stdout);
        kakurasu->searchalgorithm(kakurasu->rows, kakurasu->columns, kakurasu->checkMatrix, kakurasu->fMatrix, kakurasu->fValueList);
    }

    for(int i = 1; i<=kakurasu->sizeOfTheProblem; i++)
    {
        for(int j = 1; j<=kakurasu->sizeOfTheProblem; j++)
        {
            //cout<<"\n"<<kakurasu->checkMatrix[i][j];
            QString str = QString::number(kakurasu->checkMatrix[i][j]);
            QTableWidgetItem *item = new QTableWidgetItem(str);
            ui->tableWidget->setItem(i - 1, j - 1, item);
        }
    }
}
