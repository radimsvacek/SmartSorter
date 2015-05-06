/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#ifndef TXT_PLUGIN_H
#define TXT_PLUGIN_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDebug>
#include "plugin_interface.h"


class TXT_plugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
public:
    ~TXT_plugin();

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
    bool IsContentConditionTrue(TreeBranch *, File *);
    bool IsRowsConditionTrue(TreeBranch *, File *);
    bool IsWordsConditionTrue(TreeBranch *, File *);
    bool IsCharsConditionTrue(TreeBranch *, File *);

    int GetLinesCount(File *);
    int GetWordsCount(File *);
    int GetCharsCount(File *);

    const QString PROPERTY_CONTENT = "Content";
    const QString PROPERTY_ROWS = "Rows";
    const QString PROPERTY_WORDS = "Words";
    const QString PROPERTY_CHARS = "Chars";


    const QStringList AVIABLE_EXTENSIONS = QStringList() << "TXT";
    const QStringList PROPERTIES_FOR_DISTRIBUTION = QStringList();
};



#endif // TXT_PLUGIN_H
