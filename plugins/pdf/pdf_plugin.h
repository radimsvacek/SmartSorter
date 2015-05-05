#ifndef PDF_PLUGIN_H
#define PDF_PLUGIN_H

class PDF_plugin;

#include <QObject>
#include <QString>
#include <QStringList>
#include <QRectF>

#include "plugin_interface.h"
#include "code/attribute.h"
#include "code/file.h"
#include "code/treebranch.h"

#include "poppler/qt4/poppler-qt4.h"


class PDF_plugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
public:
    ~PDF_plugin();

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

    bool IsContentConditionTrue(TreeBranch *, Poppler::Document*, bool);
    bool IsPagesConditionTrue(TreeBranch *, int);
    bool IsStringConditionTrue(TreeBranch *, QString);

    const QString PROPERTY_CONTENT = "Content";
    const QString PROPERTY_FIRST_PAGE = "First page";
    const QString PROPERTY_META_AUTHOR = "Meta author";
    const QString PROPERTY_META_TITLE = "Meta title";
    const QString PROPERTY_META_SUBJECT = "Meta subject";
    const QString PROPERTY_PAGES = "Pages";


    const QStringList AVIABLE_EXTENSIONS = QStringList() << "PDF";
    const QStringList PROPERTIES_FOR_DISTRIBUTION = QStringList() << PROPERTY_META_AUTHOR;

};



#endif // PDF_PLUGIN_H
