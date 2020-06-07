#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDirIterator>
#include <QProcess>

        QProcess process2;

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Set progress bar
    ui->progressBar->setValue(0);
    ui->progressBar->setAlignment(Qt::AlignRight | Qt::AlignVCenter); // alignment
    ui->progressBar->setFormat(QString::fromUtf8("No task currently: %1%").arg(QString::number(0,'f', 1)));

    //Apply style
    QFile stylesheet("./Resource/themes/qdarkstyle/qdarkstyle.qss");
    stylesheet.open(QFile::ReadOnly);
    this->setStyleSheet(stylesheet.readAll());
    stylesheet.close();
#ifdef linux
    QProcess process;
    process.start( "v4l2-ctl --list-devices" );
   // process.start("sh", QStringList()<<"-c"<<"udevadm info --name=/dev/sdd | grep ID_SERIAL");
    process.waitForFinished(-1); // will wait forever until finished
  //  process.kill();
    QString stdout = process.readAll();
  //  QString stdout = process2.readAllStandardOutput();
 //  QStringList test = stdout.split(":");
      QStringList test = stdout.split("\n");
              //test << stdout.split("\t");
QStringList list;
int iter;
   foreach (QString pstring,test){
   qDebug() << test.at(iter).toLatin1();
   list << test.at(iter).toLatin1();
           iter++;
        //iflinecontains /dev/video

   }
QStringList List2;

   foreach (QString fstr, list) {
      if (fstr.contains("/dev/video")){
             List2 = fstr.split("\t");
      qDebug() << "found video" << List2.at(1).toLatin1();
      ui->cameras->addItem(List2.at(1).toLatin1());
      }
           }

#endif

//   QStringList test2 = test.at(1).split("=");

}

MainWindow::~MainWindow()
{
    if (streaming){
                QProcess::startDetached("killall", {"ffmpeg","-9"});
    }

    delete ui;

}

/************************************************* **********************************************
*Function name: initFTP
*Function description: Initially record URL address, username, password, and check the legality of URL address
*Function parameters: None
*Function return value: bool
*Author: sakuya
*Function creation date: 2019/1/24
*Function modification date: 2019/1/28
*Modified by: sakuya
*Reason for modification: Added detection of the legality of URL addresses
*Version: 1.1
*Historical version: 1.0
************************************************** *********************************************/
bool MainWindow::initFTP()
{
    //Judging whether the URL address, username and password are filled in
//    if (ui->lineEdit_ftp->text().isEmpty()) {
//        QMessageBox::critical(NULL, tr("Error"), "URL address cannot be empty");
//        return false;
//    }
//    else if (ui->lineEdit_user->text().isEmpty()) {
//        QMessageBox::critical(NULL, tr("Error"), "user cannot be empty");
//        return false;
//    }
//    else if (ui->lineEdit_user->text().isEmpty()) {
//        QMessageBox::critical(NULL, tr("Error"), "Password cannot be empty");
//        return false;
//    }
  //  else {
        ftpPath = ui->lineEdit_ftp->text();
        user = ui->lineEdit_user->text();
        password = ui->lineEdit_password->text();
        //Check if the URL address is legal
        QUrl url = QUrl(ftpPath);
        if (!url.isValid()) {
            QMessageBox::critical(NULL, tr("Error"), "URL address is invalid!");
            return false;
        }
        else if (url.scheme() != "ftp") {
            QMessageBox::critical(NULL, tr("Error"), "URL address must start with ftp!");
            return false;
        }
        return true;
  //  }
}

/************************************************* **********************************************
*Function name: replyFinished
*Function description: delete pointer, update and close file
*Function parameters: None
*Function return value: None
*Author: sakuya
*Function creation date: 2019/1/25
*Function modification date:
*Modified by:
*Modify reason :
*Version: 1.0
*Historical version: None
************************************************** *********************************************/
void MainWindow::replyFinished(QNetworkReply*)
{
    if (reply->error() == QNetworkReply::NoError) {
            reply->deleteLater();
            file->flush();
            file->close();
        }
        else {
            QMessageBox::critical(NULL, tr("Error"), "Error!!!");
        }
}

/************************************************* **********************************************
*Function name: loadProgress
*Function description: Update progress bar
*Function parameter: qint64
*Function return value: None
*Author: sakuya
*Function creation date: 2019/1/25
*Function modification date:
*Modified by:
*Modify reason :
*Version: 1.0
*Historical version: None
************************************************** *********************************************/
void MainWindow::loadProgress(qint64 bytesSent, qint64 bytesTotal)
{
    ui->progressBar->setValue(0);
    qDebug() << "loaded" << bytesSent << "of" << bytesTotal;
    ui->progressBar->setMaximum(bytesTotal); //Maximum
    ui->progressBar->setValue(bytesSent); //current value
    double currentProgress = (bytesSent-ui->progressBar->minimum()) * 100.0 / (bytesTotal-ui->progressBar->minimum());
    ui->progressBar->setFormat(QString::fromUtf8("Current progress is: %1%").arg(QString::number(currentProgress,'f', 1)));
}

/************************************************* **********************************************
*Function name: getFileName
*Function description: get the file name of the incoming path
*Function parameter: QString
*Function return value: QString
*Author: sakuya
*Function creation date: 2019/1/25
*Function modification date:
*Modified by:
*Modify reason :
*Version: 1.0
*Historical version: None
************************************************** *********************************************/
QString MainWindow::getFileName(QString m_filePath)
{
    QString temp;
    QString fileName;
    int count = -1;
    fileName = m_filePath;
    for(int i = 0; temp != "/"; i++)
    {
        temp = fileName.right(1);
        fileName.chop(1);
        count++;
    }
    fileName = m_filePath.right(count);

    return fileName;
}

/************************************************* **********************************************
*Function name: replyError
*Function description: An error occurs in the download process, and an error report is processed (overtime processing also throws out a timeout signal, and it is processed by this slot function)
*Function parameter: NetworkError
*Function return value: None
*Author: sakuya
*Function creation date: 2019/1/28
*Function modification date:
*Modified by:
*Modify reason :
*Version: 1.0
*Historical version: None
************************************************** *********************************************/
void MainWindow::replyError(QNetworkReply::NetworkError error)
{
    auto metaEnum = QMetaEnum::fromType<QNetworkReply::NetworkError>();
    //The enumeration value is converted to a string
    auto errStr = metaEnum.valueToKey(error);
    QMessageBox::critical(NULL, tr("Error"), QString(errStr));

    file->deleteLater();
    file = Q_NULLPTR;

    reply->deleteLater();
}

//Write data to a local file when downloading
void MainWindow::readContent()
{
    file->write(reply->readAll());
}

//upload files
void MainWindow::on_Btn_upload_clicked()
{
    if (initFTP()) {
        //Get the path of the selected file, save it in the string list
        QStringList string_list;
        string_list = QFileDialog::getOpenFileNames(this, tr("select file"), "", tr("Files (*)"));
        if (!string_list.isEmpty()) {
            for (int i = 0; i <string_list.count(); i++) {
                QString filePath;
                filePath = string_list.at(i);
                file = new QFile(filePath);
                file->open(QIODevice::ReadOnly);
                QByteArray byte_file = file->readAll();

                QString fileName;
                fileName = getFileName(filePath);
                QString m_ftpPath;
                m_ftpPath = ftpPath + "/" + fileName;

                //Upload the selected file to the server
                accessManager = new QNetworkAccessManager(this);
                accessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
                QUrl url(m_ftpPath);
                url.setPort(ui->port->text().toInt());
                url.setUserName(user);
                url.setPassword(password);

                QNetworkRequest request(url);
                reply = accessManager->put(request, byte_file);

                connect(accessManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(replyFinished(QNetworkReply*)));
                connect(reply, SIGNAL(uploadProgress(qint64 ,qint64)), this, SLOT(loadProgress(qint64 ,qint64)));
                connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
            }
        }
    }
}

//download file
void MainWindow::on_Btn_download_clicked()
{
    if (initFTP()) {
        QString folderPath;
        folderPath = QFileDialog::getSaveFileName(this,  tr("Save TX"), "",  tr("SaveRX/TX File (*.txt);;All Files (*)"));
            //    QFileDialog::getExistingDirectory(this, tr("select file"), "", QFileDialog::ShowDirsOnly);
        file = new QFile(folderPath.toLatin1());
        file->open(QIODevice::WriteOnly);

        //Download the file from the server to the selected folder
        QNetworkAccessManager *accessManager = new QNetworkAccessManager(this);
        accessManager->setNetworkAccessible(QNetworkAccessManager::Accessible);
        QUrl url( ui->lineEdit_ftp->text().toLatin1() );
        url.setPort(ui->port->text().toInt());
        url.setUserName(user);
        url.setPassword(password);

        QNetworkRequest request(url);
        reply = accessManager->get(request);

        connect((QObject *)reply, SIGNAL(readyRead()), this, SLOT(readContent()));
        connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
        connect(reply, SIGNAL(downloadProgress(qint64 ,qint64)), this, SLOT(loadProgress(qint64 ,qint64)));
        connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), SLOT(replyError(QNetworkReply::NetworkError)));
    }
}

void MainWindow::on_streambtn_clicked()
{
    //scan directory for new files then upload
    QString USER;
    QDate dNow(QDate::currentDate());

 //   QDir().mkdir(dNow.toString("dd.MM.yyyy"));



//    QDir().mkdir(dNow.toString("dd.MM.yyyy"));


  //  process2.start("ffmpeg", QStringList() << "-y -i /dev/video0 -c:v libvpx -r 30 -g 90 -s 1280x720 -quality realtime -speed 5 -threads 2 -crf 10 -b:v 1M -map 0 -segment_time 00:00:25 -f segment -reset_timestamps 1 -c:a libvorbis ./output%01d.webm");
//process2.start("ffmpeg", {"-y","-i","/dev/video0" ,"-c:v" ,"libvpx" ,"-r", "30", "-g", "90", "-s" ,"1280x720", "-quality", "realtime", "-speed", "5", "-threads", "2", "-crf", "10", "-b:v", "1M", "-map", "0", "-segment_time", "00:00:25", "-f", "segment", "-reset_timestamps", "1", "-c:a", "libvorbis", "./output%01d.webm"});
//    if (!process2.startDetached("/bin/sh", QStringList{"liveStream.sh"})){
//        qDebug() << "Failed to run";
//    }
    
    qint64 pid;
    QString localdir;
    QProcess::startDetached("/bin/sh", QStringList{"liveStream.sh"}, QDir::currentPath()  ,  &pid);
    streaming=1;
   // QProcess::startDetached("kill", {QString::number(pid)});

//    QString Filename;
//    QString OldFilename;
//    QStringList files;
//    while (streaming){

//        QDirIterator it("./"+dNow.toString("dd.MM.yyyy")+"/", QStringList() << "*.webm", QDir::Files, QDirIterator::Subdirectories);
//        while (it.hasNext()){
//          //  QFileInfo fileInfo(f.fileName());
//          //  ui->cmbTheme->addItem(it.next().toLatin1());
//         //   dNow.toString("dd.MM.yyyy");
//         //   "./stream/"
//            files << it.next().toLatin1();
//        }


//    qSort(files);

////    for (int i=0; i < files.count(); i++){
////                //if files.count() > cache rm files
////        if (i = files.count()-1){
////           Filename = files.at().toLatin1();
////        }
////    }

//     Filename = files.at(files.count()).toLatin1();

//    if (Filename != OldFilename){
//        OldFilename = Filename;
//        //QString URL,QString User,QString Password,QString port,QString filePath
//    //    manager.Upload("ftp://127.0.0.1","admin","qt","8258",Filename.toLatin1());
//        //"ftp://admin:qt@127.0.0.1:8258"

//    }

//    }


}

void MainWindow::on_stop_clicked()
{

        QProcess::startDetached("killall", {"ffmpeg","-9"});
        streaming = 0;
             //process2.kill();
}
