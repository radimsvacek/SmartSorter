#include "command.h"


void Command::Initialize(QString fileName, QString filePath, QString destFolder,QTextBrowser * logs){
    destination = destFolder;
    file = fileName;
    path = filePath;
    logsOut = logs;
}

void MoveCommand::Execute(){
    QFile::rename(path,destination+file);
    logsOut->insertHtml("File " + file +" was moved to " + destination +"<br>");
    ScrollDown();
}

void MoveCommand::Undo(){
    QFile::rename(destination+file,path);
    logsOut->insertHtml("File " + file +" was moved back <br>");
}

void CopyCommand::Execute(){
    QFile::copy(path,destination+file);
    logsOut->insertHtml("File " + file +" was copied to " + destination +"<br>");
    ScrollDown();
}

void CopyCommand::Undo(){
    QFile::remove(destination+file);
    logsOut->insertHtml("Copied file " + file +" was removed from " + destination +"<br>");
}

// Only move to trash folder
void DeleteCommand::Execute(){
    QFile::rename(path,destination+file);
    logsOut->insertHtml("File " + file +" was moved to trash <br>");
    ScrollDown();
}

void DeleteCommand::Undo(){
    QFile::rename(destination+file,path);
    logsOut->insertHtml("File " + file +" was renewed from trash <br>");
}


void Command::ScrollDown(){
    if(logsOut!= NULL){
        QScrollBar *sb = logsOut->verticalScrollBar();
        sb->setValue(sb->maximum());
    }
}
