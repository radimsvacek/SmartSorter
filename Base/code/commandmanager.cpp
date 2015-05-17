/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#include "commandmanager.h"

CommandManager::CommandManager()
{
}

/// Just in case of not cleaned vector
CommandManager::~CommandManager(){
    for(auto commandsIterator = commands.begin(); commandsIterator != commands.end(); ++commandsIterator) {
        delete (*commandsIterator);
    }
    commands.clear();
}

/// Execute commands and save it
void CommandManager::ExecuteCommand(Command *command){
    commands.push_back(command);
    command->Execute();

}

/// Clean memory
void CommandManager::ClearCommands(){
    for(auto commandsIterator = commands.begin(); commandsIterator != commands.end(); ++commandsIterator) {
        delete (*commandsIterator);
    }
    commands.clear();
}

/// Undo all saved commands in for cycle
void CommandManager::UndoCommands(){
    int counter = 0;

    for(auto commandsIterator = commands.begin(); commandsIterator != commands.end();++commandsIterator) {
        if(counter++ > REFRESH_GUI){
            qApp->processEvents();
            counter=0;
            (*commandsIterator)->ScrollDown();
        }
        (*commandsIterator)->Undo();
    }
}
