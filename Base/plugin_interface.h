/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#ifndef PLUGIN_INTERFACE_H
#define PLUGIN_INTERFACE_H


#include <QString>
#include <QtPlugin>

#include "code/attribute.h"
#include "code/file.h"
#include "code/treebranch.h"


/****************************/
// PluginInterface
class PluginInterface
{
public:
    virtual ~PluginInterface() {}
    virtual void Initialize() = 0;
    virtual QString GetName() = 0;
    virtual QStringList GetExtensions() = 0;
    virtual QStringList GetProperties() = 0;
    virtual QStringList GetPropertiesForDistribution() = 0;
    virtual std::vector<int> GetPropertyOperations(QString) = 0;
    virtual int GetPropertyType(QString) = 0;
    virtual bool IsFileFullfillCondition(TreeBranch *, File *, QString) = 0;
    virtual bool IsSupportedFormat(QString) = 0;
    virtual QString GetPropertyValueForDistribution(File *, QString) = 0;

};


Q_DECLARE_INTERFACE(PluginInterface, "cz.webrs.BP.SmartSorter/1.0")

#endif // PLUGIN_INTERFACE_H
