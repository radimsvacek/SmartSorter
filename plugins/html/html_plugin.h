/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#ifndef HTML_PLUGIN_H
#define HTML_PLUGIN_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QtWebKit/QWebFrame>
#include <QtWebKit/QWebView>
#include <QMultiMap>
#include <QFile>

#include "plugin_interface.h"
#include "code/attribute.h"
#include "code/file.h"
#include "code/treebranch.h"

/*! \brief HTML plugin
 *
 * Can get info about meta in header, title, etc
 */
class HTML_plugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
public:
    ~HTML_plugin();

    QString GetName();
    void Initialize();

    QStringList GetExtensions();
    QStringList GetProperties();
    QStringList GetPropertiesForDistribution();
    std::vector<int> GetPropertyOperations(QString);
    int GetPropertyType(QString);

    bool IsFileFullfillCondition(TreeBranch *, File *, QString);

    bool IsSupportedFormat(QString);
    QString GetPropertyValueForDistribution(File *, QString);

private:
    std::vector<Attribute *> properties;

    QString GetMetaData(QString, QString);
    QString GetSpecicifMeta(QString, QMultiMap<QString,QString>);

};

const QString PROPERTY_TITLE = "Title";
const QString PROPERTY_AUTHOR = "Author";
const QString PROPERTY_KEYWORDS = "KeyWords";
const QString PROPERTY_DESCRIPTION = "Description";
const QString PROPERTY_CHARSET = "Charset";
const QString PROPERTY_ROBOTS = "Robots";

const QStringList AVIABLE_EXTENSIONS = QStringList() << "HTML" << "HTM" << "XHTML";
const QStringList PROPERTIES_FOR_DISTRIBUTION = QStringList() << PROPERTY_AUTHOR << PROPERTY_CHARSET << PROPERTY_ROBOTS;

#endif // HTML_PLUGIN_H
