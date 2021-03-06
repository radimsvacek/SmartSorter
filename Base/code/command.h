/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#ifndef COMMAND_H
#define COMMAND_H

#include <QString>
#include <QTextBrowser>
#include <QFile>
#include <QScrollBar>
#include <QDebug>


/****************************/
// Command
/*! \brief Virtual class
 *
 * Specifies what must be implemented in child classes
 */
class Command
{
public:
    virtual ~Command() {}
    virtual void Execute() = 0;
    virtual void Undo() = 0;
    void Initialize(QString,QString,QString,QTextBrowser *);
    void ScrollDown();

    QString file;
    QString path;
    QString destination;
    QTextBrowser * logsOut;
};

/*! \brief Move operation
 *
 * Move file to new destination
 */
class MoveCommand : public Command{
public:
    void Execute();
    void Undo();
};

/*! \brief Copy operation
 *
 * Copy file to destination
 */
class CopyCommand : public Command{
public:
    void Execute();
    void Undo();
};

/*! \brief Remove operation
 *
 * Move file to trash folder
 */
class DeleteCommand : public Command{
public:
    void Execute();
    void Undo();
};

#endif // COMMAND_H
