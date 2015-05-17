/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#include "html_plugin.h"

/// Plugin initialization, Create properities which can be compared
void HTML_plugin::Initialize(){
    Attribute *a;
    std::vector<int> ops;

    ops.push_back(EQUAL);
    ops.push_back(CONTAINS);
    ops.push_back(NOT_CONTAINS);

    a = new Attribute(PROPERTY_TITLE,ops,TEXT_TYPE);
    properties.push_back(a);

    a = new Attribute(PROPERTY_ROBOTS,ops,TEXT_TYPE);
    properties.push_back(a);

    a = new Attribute(PROPERTY_AUTHOR,ops,TEXT_TYPE);
    properties.push_back(a);

    a = new Attribute(PROPERTY_DESCRIPTION,ops,TEXT_TYPE);
    properties.push_back(a);

    a = new Attribute(PROPERTY_CHARSET,ops,TEXT_TYPE);
    properties.push_back(a);

    a = new Attribute(PROPERTY_KEYWORDS,ops,TEXT_TYPE);
    properties.push_back(a);

    ops.clear();
}

HTML_plugin::~HTML_plugin(){
    for(auto propertyIterator = properties.begin(); propertyIterator != properties.end(); ++propertyIterator) {
        delete (*propertyIterator);
    }
    properties.clear();
}

// HTML PLUGIN
QString HTML_plugin::GetName()
{
    return "HTML";
}

/// Supported extensions
QStringList HTML_plugin::GetExtensions()
{
    return AVIABLE_EXTENSIONS;
}

/// Properties supported by this plugin
QStringList HTML_plugin::GetProperties()
{
    QStringList list;

    // Choose basic properties
    for(auto propertyIterator = properties.begin(); propertyIterator != properties.end(); ++propertyIterator) {
        list << (*propertyIterator)->GetName();
    }

    return list;
}

/// Returns properties which can be used for distribution
QStringList HTML_plugin::GetPropertiesForDistribution(){
    return PROPERTIES_FOR_DISTRIBUTION;
}

/// Returns vector of property operations
std::vector<int> HTML_plugin::GetPropertyOperations(QString property){
    for(auto propertyIterator = properties.begin(); propertyIterator != properties.end(); ++propertyIterator){
        if(((*propertyIterator)->GetName())==property)
            return (*propertyIterator)->GetPropertyOperations();
    }
    return std::vector<int>();
}

/// Returns type of specific property
int HTML_plugin::GetPropertyType(QString arg){
    for(auto propertyIterator = properties.begin(); propertyIterator != properties.end(); ++propertyIterator){
        if(((*propertyIterator)->GetName())==arg)
            return (*propertyIterator)->GetPropertyType();
    }
    return 0;
}

/// Returns if fileFormat is supported by this plugin
bool HTML_plugin::IsSupportedFormat(QString fileFormat){
    foreach(QString str,AVIABLE_EXTENSIONS){
        if(str.compare(fileFormat,Qt::CaseInsensitive) == 0)
            return true;
    }
    return false;
}

/// Returns values for file distribution
QString HTML_plugin::GetPropertyValueForDistribution(File *file, QString property){
   if(!PROPERTIES_FOR_DISTRIBUTION.contains(property))
       return "";
   QString meta = GetMetaData(file->GetPath(),property);
   if(meta.isEmpty())
       return "Default";
   else
       return meta;
}

/// Resolver for properties in this plugin
bool HTML_plugin::IsFileFullfillCondition(TreeBranch *branch, File *file, QString property){
    QString fileProperty = GetMetaData(file->GetPath(),property);
    QString branchValue = branch->GetStringValue();

    switch(branch->GetOperand()){
        case EQUAL:
            return (fileProperty.compare(branchValue,Qt::CaseInsensitive) == 0);
        case CONTAINS:
            return fileProperty.contains(branchValue,Qt::CaseInsensitive);
        case NOT_CONTAINS:
            return !fileProperty.contains(branchValue,Qt::CaseInsensitive);
    }
    return false;
}

/// Open file and load it as qwebview, return required data
QString HTML_plugin::GetMetaData(QString filePath, QString what){
    QFile F(filePath);

    if(!F.open(QIODevice::ReadOnly | QIODevice::Text))
        return "";

    QString content = F.readAll();
    QWebView *web = new QWebView();
    web->setHtml(content);

    if(what.compare(PROPERTY_TITLE,Qt::CaseInsensitive) == 0)
        return web->title();

    QWebFrame *pageContent = web->page()->mainFrame();
    QMultiMap<QString,QString> meta = pageContent->metaData();

    delete web;

    F.close();

    return GetSpecicifMeta(what,meta);
}

/// Returns string, which contains required metadata of html file
QString HTML_plugin::GetSpecicifMeta(QString which, QMultiMap<QString,QString> meta){
    foreach(QString key,meta.keys()){
        if(key.compare(which,Qt::CaseInsensitive) == 0){
            foreach(QString value, meta.values(key)){
                return value;
            }
        }
    }
    return "";
}
QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(html_plugin,HTML_plugin)
QT_END_NAMESPACE
