#ifndef BASIC_CLASSIF_H
#define BASIC_CLASSIF_H


#include "attribute.h"
#include "treebranch.h"
#include "file.h"
#include "../plugin_interface.h"

#include <vector>
#include <QString>
#include <QStringList>
#include <QPluginLoader>
#include <QDir>
#include <QApplication>
#include <QDebug>
#include <QObject>

#define MEGA 1024*1024


/****************************/
// BasicClassification
class BasicClassification
{
public:
    BasicClassification();
    ~BasicClassification();

	void LoadPlugins();
    QStringList GetLoadedPlugins();

    QStringList GetPropertiesForDistribution(QString);
    QStringList GetProperties(QString);
    QStringList GetFileFormats();
    QString GetGeneralFileFormat(QString);
    std::vector<int> GetPropertyOperations(QString,QString);
    int GetPropertyType(QString, QString);

    bool IsFileFullfilCondition(TreeBranch *, File *, QString);

    QString GetDistributionPath(QString,File*);
    QString GetNextFolderForDistribution(QString,File*);

private:
    void InitializePlugin(QObject *plugin);    
    // Array of objects representing each file format
    std::vector<PluginInterface *> PluginsFormats;

    bool IsBasicProperty(QString);
    bool IsGeneralFormat(QString);

    bool IsSizeConditionTrue(TreeBranch *, File *);
    bool IsCreationDateConditionTrue(TreeBranch *, File *);
    bool IsLastModificationDateConditionTrue(TreeBranch *, File *);
    bool IsNameConditionTrue(TreeBranch *, File *);
    bool IsExtensionConditionTrue(TreeBranch *, File *);

    std::vector<Attribute *> properties;

    const QString PROPERTY_SIZE = "Size (MB)";
    const QString PROPERTY_CREATION_DATE = "Creation date";
    const QString PROPERTY_LAST_MODIFICATION_DATE = "Last modif. date";
    const QString PROPERTY_NAME = "Name";
    const QString PROPERTY_EXTENSION = "Extension";

    const QString ALL_FORMATS = "All files";
    const QString PICTURES = "Pictures (JPG,PNG,etc)";
    const QString AUDIOS = "Audio (MP3,WMV,etc)";
    const QString VIDEOS = "Video (AVI,MKV,etc)";
    const QString DOCUMENTS = "Documents (PDF,DOC,etc)";
    const QString ARCHIVES = "Archives (ZIP,RAR,etc)";

    const QStringList BASIC_DISTRIBUTION_PROPERTIES = QStringList() << "Year" << "Month" << "Day";

    const QStringList GENERAL_FORMATS = QStringList() << ALL_FORMATS << DOCUMENTS << PICTURES << AUDIOS << VIDEOS << ARCHIVES;
    const QStringList SPECIFIC_FORMATS = QStringList() << "PDF" << "DOC" << "PPT"  << "JPG" << "PNG" << "GIF" << "TXT" << "HTML" << "XML" << "MP3" << "WMV" << "AVI" << "MP4" << "ZIP" << "RAR" << "TAR" << "GZ";

    const QStringList DOCUMENT_FORMATS = QStringList() << "PDF" << "DOC" << "DOCX" << "ODT" << "XLS" << "ODS" << "PPT" << "HTML" << "XML";
    const QStringList PICTURE_FORMATS = QStringList() << "JPG" << "PNG" << "GIF" << "BMP";
    const QStringList AUDIO_FORMATS = QStringList() << "MP3" << "WMA" << "WAV";
    const QStringList VIDEO_FORMATS = QStringList() << "AVI" << "MP4" << "MKV" << "FLV" << "WMV" << "MPG" << "3GP";
    const QStringList ARCHIVE_FORMATS = QStringList() << "ZIP" << "RAR" << "TAR" << "GZ" << "BZ" << "7Z";

};

#endif // BASIC_CLASSIF_H


