/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include "command.h"
#include <vector>
#include <QDebug>
#include <QApplication>

#define REFRESH_GUI 5


/****************************/
// CommandManager
class CommandManager
{
private:
    std::vector<Command *> commands;

public:
    CommandManager();
    ~CommandManager();

    void ExecuteCommand(Command *);
    void ClearCommands();
    void UndoCommands();
};

#endif // COMMANDMANAGER_H
