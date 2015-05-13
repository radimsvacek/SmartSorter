/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#ifndef JPEG_PLUGIN_H
#define JPEG_PLUGIN_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QProcess>
#include <QImage>

#include <libexif/exif-data.h>

#include "plugin_interface.h"
#include "code/attribute.h"
#include "code/file.h"
#include "code/treebranch.h"

#define MEGA 1000000

class JPEG_plugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
public:
    ~JPEG_plugin();

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
    std::vector<Attribute *> Properties;
    bool IsSizeConditionTrue(TreeBranch *branch, File *file, QString property);
    bool IsExifConditionTrue(TreeBranch *branch, File *file, QString property);
    bool IsFaceConditionTrue(int, File *file);
    QString GetCamera(File *);
    QDate GetTakenDate(File *);

    int timeForScript = 10000;

    const QString FACEDETECT_SCRIPT_PATH = "/usr/local/bin/tool/facedetect";
    const QString FACEDETECT_SCRIPT_PATH2 = "tool/facedetect/facedetect";

    const QString PROPERTY_RESOLUTION = "Resolution (MPx)";
    const QString PROPERTY_CAMERA = "Camera";
    const QString PROPERTY_TAKEN_DATE = "Date taken";
    const QString PROPERTY_WIDTH = "Width (px)";
    const QString PROPERTY_HEIGHT = "Height (px)";

    const QString PROPERTY_FACE = "Face in picture";

    const QString PROPERTY_YEAR = "EXIF Year";
    const QString PROPERTY_MONTH = "EXIF Month";
    const QString PROPERTY_DAY = "EXIF Day";
    const QStringList DATE_PROPERTIES = QStringList() << PROPERTY_YEAR << PROPERTY_MONTH << PROPERTY_DAY;

    const QStringList AVIABLE_EXTENSIONS = QStringList() << "JPEG" << "JPG" << "JPE" << "JIF";
    const QStringList PROPERTIES_FOR_DISTRIBUTION = QStringList() << PROPERTY_RESOLUTION << PROPERTY_CAMERA << DATE_PROPERTIES << PROPERTY_FACE;// << PropertyDark << PropertyBright;

};



#endif // JPEG_PLUGIN_H
