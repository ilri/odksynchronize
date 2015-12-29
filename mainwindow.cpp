#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <QFile>
#include <QDomDocument>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#if defined(Q_OS_ANDROID)
    #include "androidinterface.h"
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Actualizador de listas");
    this->setWindowState(Qt::WindowMaximized);
    //ui->lblerror->setVisible(false);
    ui->lbldone->setVisible(false);    

#if defined(Q_OS_ANDROID)
    androidInterface andInt;
    appPath = andInt.getExternalStorageDirectory();    
#else
    appPath = "/storage/emulated/0";
#endif

    dataPath = appPath + "/Android/data";

    QDir dir;
    if (!dir.exists(dataPath + "/updatelists.ilri.org/temp"))
    {
        if (!dir.mkpath(dataPath + "/updatelists.ilri.org/temp"))
        {
            procError("Cannot create temporary directory: " + dataPath + "/updatelists.ilri.org/temp");
        }
    }
    else
    {
        dir.setPath(dataPath + "/updatelists.ilri.org/temp");
        foreach (QString fileName, dir.entryList(QDir::Files))
        {
            QFile::remove(dir.absolutePath() + "/" + fileName);
        }
    }
    ui->cmdact->setEnabled(false);
    ui->cmdupd->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::procError(QString text)
{
    ui->listWidget->addItem("Error:" + text);
    /*//if (ui->lblerror->isVisible() == false)
    //{
        ui->lblerror->setVisible(true);
        ui->lblerror->setText(text);
        ui->lblerror->setWordWrap(true);
        ui->cmdactxml->setEnabled(false);
        ui->cmdact->setEnabled(false);
        QFont font;
        font.setBold(true);
        font.setPointSize(18);
        ui->lblerror->setFont(font);

        QPalette palette = ui->lblerror->palette();
        palette.setColor(ui->lblerror->backgroundRole(), Qt::black);
        palette.setColor(ui->lblerror->foregroundRole(), Qt::red);

        ui->lblerror->setPalette(palette);

    //}*/
}

void MainWindow::on_cmdsalir_clicked()
{
    this->close();
}

void MainWindow::on_cmdact_clicked()
{
    //Delete all files in the temp directory
    QDir dir;
    dir.setPath(dataPath + "/updatelists.ilri.org/temp");
    foreach (QString fileName, dir.entryList(QDir::Files))
    {
        QFile::remove(dir.absolutePath() + "/" + fileName);
    }

    //Read the XML file to obtain the lists
    QFile xmlfile(dataPath + "/updatelists.ilri.org/lists.xml");
    if (!xmlfile.open(QIODevice::ReadOnly))
    {
        procError("Cannot open XML file");
        return;
    }
    QDomDocument doc("ODKDocument");
    if (!doc.setContent(&xmlfile))
    {
        procError("Cannot read XML files");
        return;
    }
    xmlfile.close();

    QDomNodeList lists;
    lists = doc.elementsByTagName("list");
    for (int nlist = 0; nlist < lists.count();nlist++)
    {
        QString url = lists.item(nlist).toElement().attribute("url","NONE");
        QString fileName = lists.item(nlist).toElement().attribute("file","NONE");
        if ((url != "NONE") && (fileName != "NONE"))
        {
            QUrl fileUrl(url);
            FileDownloader *fileCnt = new FileDownloader(fileUrl,fileName,this);
            connect(fileCnt, SIGNAL(downloaded(QString,QByteArray)), this, SLOT(fileLoaded(QString,QByteArray)));
            files.append(fileCnt);
        }
    }

    for (int nfiles=0; nfiles < files.count();nfiles++)
    {
        if (files[nfiles]->finished == false)
            files[nfiles]->startDownload();
    }

    this->ui->cmdact->setEnabled(false);
}

void MainWindow::fileLoaded(QString fileName, QByteArray data)
{

    //Copy the file to the temporary directory first
    QFile file(dataPath + "/updatelists.ilri.org/temp/"+ fileName);
    file.open(QIODevice::WriteOnly); 
    file.write(data);
    file.close();

    //Checks if all downloaded files finished
    bool allFinished;
    allFinished = true;
    for (int nfiles=0; nfiles < files.count();nfiles++)
    {
        if (files[nfiles]->finished == false)
            allFinished = false;
    }
    if (allFinished)
    {
        ui->cmdupd->setEnabled(true);
    }
}

void MainWindow::replaceFiles()
{
    ui->cmdupd->setEnabled(false);
    bool allFinished;
    allFinished = true;
    if (allFinished)
    {
        //Now that we have all the files move them to the forms
        //Read the XML file to obtain the lists
        QFile xmlfile(dataPath + "/updatelists.ilri.org/lists.xml");
        if (!xmlfile.open(QIODevice::ReadOnly))
        {
            procError("No se puede abrir el archivo XML");
            return;
        }
        QDomDocument doc("ODKDocument");
        if (!doc.setContent(&xmlfile))
        {
            procError("No se puede leer el archivo XML");
            return;
        }
        xmlfile.close();

        QDomNodeList lists;
        lists = doc.elementsByTagName("list"); //Get the lists
        bool updateError;
        updateError = false;
        QSqlDatabase source = QSqlDatabase::addDatabase("QSQLITE","SourceDB");
        QSqlDatabase target = QSqlDatabase::addDatabase("QSQLITE","TargetDB");
        for (int nlist = 0; nlist < lists.count();nlist++)
        {
            QString targetFileName = lists.item(nlist).toElement().attribute("file","NONE");
            if (targetFileName != "NONE")
            {
                source.close();
                source.setDatabaseName(dataPath + "/updatelists.ilri.org/temp/" + targetFileName);


                QDomNodeList columns;
                QDomNodeList forms;
                columns = lists.item(nlist).toElement().elementsByTagName("column"); //Get the files of a list
                forms = lists.item(nlist).toElement().elementsByTagName("form"); //Get the forms of a list
                //For each form
                for (int nforms = 0; nforms < forms.count(); nforms++)
                {
                    QString formDir;
                    formDir = forms.item(nforms).toElement().text();
                    //For each file
                    if (QFile::exists(appPath + "/odk/forms/" + formDir + "/" + targetFileName)) //If the old database exists
                    {
                        if (QFile::exists(dataPath + "/updatelists.ilri.org/temp/" + targetFileName)) //If the new database exists
                        {                            
                            target.close();                            
                            ui->listWidget->addItem("Updating: " + formDir + " for file: " + targetFileName);
                            ui->listWidget->update();

                            target.setDatabaseName(appPath + "/odk/forms/" + formDir + "/" + targetFileName);
                            if (source.open() && target.open())
                            {
                                QSqlQuery squery(source);
                                QSqlQuery tquery(target);
                                tquery.exec("BEGIN TRANSACTION;");
                                QString sql;
                                int clm;
                                if (tquery.exec("DELETE FROM externalData") == true)
                                {
                                    sql = "SELECT ";
                                    for (clm = 0; clm < columns.count();clm++)
                                    {
                                         sql = sql + columns.item(clm).toElement().text() + ",";
                                    }
                                    sql = sql.left(sql.length()-1);
                                    sql = sql + " FROM externalData";
                                    if (squery.exec(sql))
                                    {
                                        while (squery.next())
                                        {
                                            sql = "INSERT INTO externalData (";
                                            for (clm = 0; clm < columns.count();clm++)
                                            {
                                                sql = sql + columns.item(clm).toElement().text() + ",";
                                            }
                                            sql = sql.left(sql.length()-1);
                                            sql = sql + ") VALUES (";
                                            for (clm = 0; clm < columns.count();clm++)
                                            {
                                                if (columns.item(clm).toElement().attribute("type","string") == "string")
                                                    sql = sql + "'" + squery.value(clm).toString() + "',";
                                                else
                                                    sql = sql + squery.value(clm).toString() + ",";
                                            }
                                            sql = sql.left(sql.length()-1);
                                            sql = sql + ")";

                                            if (!tquery.exec(sql))
                                            {
                                                ui->listWidget->addItem("Error insertando datos en " + formDir + " db: " + targetFileName + " :" + tquery.lastError().databaseText());
                                                updateError = true;
                                                break;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        ui->listWidget->addItem("Error en consulta para " + formDir + " db: " + targetFileName + " :" + squery.lastError().databaseText());
                                        updateError = true;
                                        break;
                                    }
                                }
                                else
                                {
                                    ui->listWidget->addItem("No se pueden borrar los datos existentes");
                                    updateError = true;
                                    break;
                                }
                                tquery.exec("END TRANSACTION;");
                            }
                            else
                            {
                                ui->listWidget->addItem("No se puede abrir la base origin or destino");
                                updateError = true;
                                break;
                            }
                        }
                        else
                        {
                            ui->listWidget->addItem("La base de datos de origen no existe");
                            updateError = true;
                            break;
                        }
                    }
                    else
                    {
                        ui->listWidget->addItem("Nota: Las bases de datos para el formulario " +  formDir + " no existen.");
                    }
                }
            }
            else
            {
                ui->listWidget->addItem("El codigo de lista en el XML esta en blanco");
                updateError = true;
                break;
            }
        }
        if (updateError == false)
            ui->lbldone->setVisible(true);
    }
}

void MainWindow::loadXML(QString fileName, QByteArray data)
{
    //Copy the file to the temporary directory first
    QFile file(dataPath + "/updatelists.ilri.org/"+ fileName);    
    file.open(QIODevice::WriteOnly);
    file.write(data);
    file.close();

    //Load the XML

    ui->cmdact->setEnabled(true);
    ui->cmdactxml->setEnabled(false);
}

void MainWindow::on_cmdactxml_clicked()
{
    //Obtain the villages file
    QUrl xmlUrl("http://data.ilri.org/portal/rdm/projects/adanicaragua/lists.xml");
    //QUrl xmlUrl("http://192.168.1.102:5000/rdm/projects/adanicaragua/lists.xml");
    m_XMLFile = new FileDownloader(xmlUrl,"lists.xml",this);
    connect(m_XMLFile, SIGNAL(downloaded(QString,QByteArray)), this, SLOT(loadXML(QString,QByteArray)));
    m_XMLFile->startDownload();
}

void MainWindow::on_cmdupd_clicked()
{
    replaceFiles();
}
