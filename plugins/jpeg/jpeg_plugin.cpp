/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#include "jpeg_plugin.h"

// Plugin initialization
// Create properities which can be compared
void JPEG_plugin::Initialize(){
    Attribute *a;
    std::vector<int> ops;

    ops.push_back(LESS);
    ops.push_back(LESS_EQUAL);
    ops.push_back(MORE);
    ops.push_back(MORE_EQUAL);
    ops.push_back(EQUAL);
    a = new Attribute(PROPERTY_RESOLUTION,ops,NUM_TYPE);
    Properties.push_back(a);

    a = new Attribute(PROPERTY_WIDTH,ops,NUM_TYPE);
    Properties.push_back(a);

    a = new Attribute(PROPERTY_HEIGHT,ops,NUM_TYPE);
    Properties.push_back(a);

    a = new Attribute(PROPERTY_TAKEN_DATE,ops,DATE_TYPE);
    Properties.push_back(a);

    ops.clear();
    ops.push_back(CONTAINS);
    ops.push_back(NOT_CONTAINS);
    ops.push_back(EQUAL);

    a = new Attribute(PROPERTY_CAMERA,ops,TEXT_TYPE);
    Properties.push_back(a);

    ops.clear();
    ops.push_back(IS_TRUE);


    if(QFile(FACEDETECT_SCRIPT_PATH).exists() || QFile(FACEDETECT_SCRIPT_PATH2).exists()){
        a = new Attribute(PROPERTY_FACE,ops,TRUE_TYPE);
        Properties.push_back(a);
    }

/*
    a = new Attribute(PropertyDark,ops,True_type);
    Properties.push_back(a);

    a = new Attribute(PropertyBright,ops,True_type);
    Properties.push_back(a);*/

    ops.clear();
}



JPEG_plugin::~JPEG_plugin(){
    for(auto propertyIterator = Properties.begin(); propertyIterator != Properties.end(); ++propertyIterator) {
        delete (*propertyIterator);
    }
    Properties.clear();
}

// JPEG PLUGIN
QString JPEG_plugin::GetName()
{
    return "JPG";
}

// Supported extensions
QStringList JPEG_plugin::GetExtensions()
{
    return AVIABLE_EXTENSIONS;
}

// Properties supported by this plugin
QStringList JPEG_plugin::GetProperties()
{
    QStringList list;

    // Choose basic properties
    for(auto propertyIterator = Properties.begin(); propertyIterator != Properties.end(); ++propertyIterator) {
        list << (*propertyIterator)->GetName();
    }

    return list;
}

// Returns properties which can be used for distribution
QStringList JPEG_plugin::GetPropertiesForDistribution(){
    return PROPERTIES_FOR_DISTRIBUTION;
}

// Returns vector of property operations
std::vector<int> JPEG_plugin::GetPropertyOperations(QString property){
    for(auto propertyIterator = Properties.begin(); propertyIterator != Properties.end(); ++propertyIterator){
        if(((*propertyIterator)->GetName())==property)
            return (*propertyIterator)->GetPropertyOperations();
    }
    return std::vector<int>();
}

// Returns type of specific property
int JPEG_plugin::GetPropertyType(QString arg)
{
    for(auto propertyIterator = Properties.begin(); propertyIterator != Properties.end(); ++propertyIterator){
        if(((*propertyIterator)->GetName())==arg)
            return (*propertyIterator)->GetPropertyType();
    }
    return 0;
}

// Returns if fileFormat is supported by this plugin
bool JPEG_plugin::IsSupportedFormat(QString fileFormat){
    foreach(QString str,AVIABLE_EXTENSIONS){
        if(str.compare(fileFormat,Qt::CaseInsensitive) == 0)
            return true;
    }
    return false;
}

// Returns string containting value for distriribution .. specific information about file
QString JPEG_plugin::GetPropertyValueForDistribution(File *file, QString property){
    if(!PROPERTIES_FOR_DISTRIBUTION.contains(property))
        return "";

    if((property == PROPERTY_RESOLUTION)){
        // Load image
        QImage *maap = new QImage(file->GetPath());
        // check if was loaded successfuly
        if(maap->isNull())
            return "Default";
        // Get image size
        QSize siz = maap->size();
        return QString::number(siz.width()*siz.height()/MEGA)+"Mpx";
    }
    else if((property == PROPERTY_FACE)){
        if(IsFaceConditionTrue(1,file))
            return "FacesInPicture";
        else
            return "WithoutFaces";
    }
    else if(DATE_PROPERTIES.contains(property)){
        QDate date = GetTakenDate(file);
        if(date.isNull())
            return "Default";
        if(property == PROPERTY_YEAR)
            return QString::number(date.year());
        else if(property == PROPERTY_MONTH)
            return QString::number(date.month());
        else if(property == PROPERTY_DAY)
            return QString::number(date.day());

    }
    else if(property == PROPERTY_CAMERA){
        QString prop = GetCamera(file);
        if(prop.isEmpty())
            return "Default";
        return prop;
    }
    return "Default";
}

// Resolver for properties in this plugin
bool JPEG_plugin::IsFileFullfillCondition(TreeBranch *branch, File *file, QString property){
    // Choose right property to compare and call its method
    if((property == PROPERTY_RESOLUTION) || (property == PROPERTY_WIDTH) || (property == PROPERTY_HEIGHT)){
        return IsSizeConditionTrue(branch,file,property);
    }
    else if(property == PROPERTY_TAKEN_DATE){
        return IsExifConditionTrue(branch,file,PROPERTY_TAKEN_DATE);
    }
    else if(property == PROPERTY_CAMERA){
        return IsExifConditionTrue(branch,file,PROPERTY_CAMERA);
    }
    else if(property == PROPERTY_FACE){
        return IsFaceConditionTrue(branch->GetNumericValue(),file);
    }
    // Otherwise return false
    return false;
}

// Check & compare image size and resolution
bool JPEG_plugin::IsSizeConditionTrue(TreeBranch *branch, File *file, QString property){
    // Load image
    QImage *maap = new QImage(file->GetPath());
    // check if was loaded successfuly
    if(maap->isNull())
        return false;
    // Get image size
    QSize siz = maap->size();
    int valueToCompare = 0;

    if(property == PROPERTY_RESOLUTION)
        valueToCompare = siz.width()*siz.height()/MEGA;
    else if(property == PROPERTY_WIDTH)
        valueToCompare = siz.width();
    else if (property == PROPERTY_HEIGHT)
        valueToCompare = siz.height();

    // compare with choosen operand
    switch(branch->GetOperand()){
        case LESS:
            return (valueToCompare < branch->GetNumericValue());
        case LESS_EQUAL:
            return (valueToCompare <= branch->GetNumericValue());
        case EQUAL:
            return (valueToCompare == branch->GetNumericValue());
        case MORE:
            return (valueToCompare > branch->GetNumericValue());
        case MORE_EQUAL:
            return (valueToCompare >= branch->GetNumericValue());
    }
    return false;
}

// Returns date when was picture taken
QDate JPEG_plugin::GetTakenDate(File *file){
    // Open file
    ExifData *exifData = exif_data_new_from_file(file->GetPath().toStdString().c_str());
    if(!exifData){
        return QDate();
    }
    ExifEntry *entry;
    char buf[1024];

    // Load date and return
    entry = exif_content_get_entry(exifData->ifd[EXIF_IFD_EXIF], EXIF_TAG_DATE_TIME_ORIGINAL);
    exif_entry_get_value(entry, buf, sizeof(buf));
    QDateTime date = QDateTime::fromString(QString::fromStdString(buf),"yyyy:MM:dd hh:mm:ss");
    return date.date();

}

// Returns complete name of camera which take the picture
QString JPEG_plugin::GetCamera(File *file){
    // Open file
    ExifData *exifData = exif_data_new_from_file(file->GetPath().toStdString().c_str());
    if(!exifData){
        return QString();
    }
    ExifEntry *entry;
    char buFf[1024];
    // Load company that made camera
    entry = exif_content_get_entry(exifData->ifd[EXIF_IFD_0], EXIF_TAG_MAKE);
    exif_entry_get_value(entry, buFf, sizeof(buFf));

    char buFF[1024];
    // Load exact model of camera
    entry = exif_content_get_entry(exifData->ifd[EXIF_IFD_0], EXIF_TAG_MODEL);
    exif_entry_get_value(entry, buFF, sizeof(buFF));

    // Put it together and return
    QString prop = QString::fromStdString(buFf);
    prop.append(QString::fromStdString(buFF));
    prop = prop.simplified();
    return prop;

}

// Check for some EXIF properties
bool JPEG_plugin::IsExifConditionTrue(TreeBranch *branch, File *file, QString property){

    // If taken date saved in EXIF metadata is wanted to compare
    if(property == PROPERTY_TAKEN_DATE){
        QDate date = GetTakenDate(file);
        if(date.isNull())
            return false;
        switch(branch->GetOperand()){
            case LESS:
                return (date < branch->GetDateValue());
            case LESS_EQUAL:
                return (date <= branch->GetDateValue());
            case EQUAL:
                return (date == branch->GetDateValue());
            case MORE:
                return (date > branch->GetDateValue());
            case MORE_EQUAL:
                return (date >= branch->GetDateValue());
        }
    }
    // If camera model is wanted, then get it and compare with value in branch
    else if(property == PROPERTY_CAMERA){
        QString prop = GetCamera(file);
        if(prop.isNull() || prop.isEmpty())
            return false;

        switch(branch->GetOperand()){
            case EQUAL:
                return (prop.compare(branch->GetStringValue(),Qt::CaseInsensitive) == 0);
            case CONTAINS:
                return prop.contains(branch->GetStringValue(),Qt::CaseInsensitive);
            case NOT_CONTAINS:
                return !prop.contains(branch->GetStringValue(),Qt::CaseInsensitive);
        }
    }

    return false;
}

// Check if image contains face
bool JPEG_plugin::IsFaceConditionTrue(int faceIn, File *file){
    // Face in picture
    QProcess *faceDetect = new QProcess;
    QStringList args;
    QString scriptPath;

    if(QFile(FACEDETECT_SCRIPT_PATH).exists())
        scriptPath = FACEDETECT_SCRIPT_PATH;
    else if (QFile(FACEDETECT_SCRIPT_PATH2).exists())
        scriptPath = FACEDETECT_SCRIPT_PATH2;
    else
        return false;

    args << scriptPath << "-q" << file->GetPath();
    faceDetect->start("python",args);

    if(!faceDetect->waitForStarted(timeForScript)){
        return false;
    }

    faceDetect->waitForFinished(timeForScript);

    int ret = faceDetect->exitCode();


    if(faceIn == 1){
        if(ret == 0)
            return true;
        else
            return false;
        }
    else{
        if(ret == 0)
            return false;
        else
            return true;
    }

}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(jpeg_plugin,JPEG_plugin)
QT_END_NAMESPACE
