#include "basic_classif.h"

// Create some basic attributes with can be used for every file format
BasicClassification::BasicClassification(){
    Attribute *newAttribute;
    std::vector<int> attributeOperations;

    attributeOperations = {LESS,LESS_EQUAL,MORE,MORE_EQUAL,EQUAL};
    newAttribute = new Attribute(PROPERTY_SIZE,attributeOperations,NUM_TYPE);
    properties.push_back(newAttribute);

    //a = new Attribute(PropertyCreationDate,ops,Date_type);
    //Properties.push_back(a);

    newAttribute = new Attribute(PROPERTY_LAST_MODIFICATION_DATE,attributeOperations,DATE_TYPE);
    properties.push_back(newAttribute);

    attributeOperations = {EQUAL,CONTAINS,NOT_CONTAINS};
    newAttribute = new Attribute(PROPERTY_NAME,attributeOperations,TEXT_TYPE);
    properties.push_back(newAttribute);

    newAttribute = new Attribute(PROPERTY_EXTENSION,attributeOperations,TEXT_TYPE);
    properties.push_back(newAttribute);

    attributeOperations.clear();

	LoadPlugins();


}

// Clean memory
BasicClassification::~BasicClassification(){
    for(auto pluginsIterator = PluginsFormats.begin(); pluginsIterator != PluginsFormats.end(); ++pluginsIterator) {
        delete (*pluginsIterator);
    }
    PluginsFormats.clear();

    for(auto pluginsIterator = properties.begin(); pluginsIterator != properties.end(); ++pluginsIterator) {
        delete (*pluginsIterator);
    }
    properties.clear();
}

// Returns names of loaded plugins .. just for info
QStringList BasicClassification::GetLoadedPlugins(){
    QStringList namesList;
    for(auto pluginsIterator = PluginsFormats.begin(); pluginsIterator != PluginsFormats.end(); ++pluginsIterator) {
        namesList << (*pluginsIterator)->GetName();
    }
    return namesList;
}

// Find and load plugins in folder "plugins"
void BasicClassification::LoadPlugins(){
    QDir pluginsDir = QDir(qApp->applicationDirPath());
    pluginsDir.cd("plugins");

    foreach (QString fileName, pluginsDir.entryList(QDir::Files)){
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if(plugin){
            InitializePlugin(plugin);
        }

    }
}

// Initialize plugin and store pointer to that in vector
void BasicClassification::InitializePlugin(QObject *plugin){
    PluginInterface *importedPlugin = qobject_cast<PluginInterface *>(plugin);
    if(importedPlugin){
        // Plugin was sucessfully loaded !
        importedPlugin->Initialize();
        PluginsFormats.push_back(importedPlugin);
    }
}

// Load properties for GUI dialog
QStringList BasicClassification::GetProperties(QString format){
    QStringList listToReturn;
    // Format was not selected -> no properties to choose
    if(format=="")
        return listToReturn;
    // Add first row for select box
    listToReturn << "";

    // Choose basic properties
    for(auto propertiesIterator = properties.begin(); propertiesIterator != properties.end(); ++propertiesIterator) {
        // If is specific format, extension property will not be shown
        if((*propertiesIterator)->GetName() == PROPERTY_EXTENSION){
            if(IsGeneralFormat(format)){
                listToReturn << (*propertiesIterator)->GetName();
            }
        }
        else{
            listToReturn << (*propertiesIterator)->GetName();
        }
    }

    // If exists plugin for that format add some properties
    for(auto pluginsIterator = PluginsFormats.begin(); pluginsIterator != PluginsFormats.end(); ++pluginsIterator) {
        if(format == (*pluginsIterator)->GetName()){
            listToReturn << (*pluginsIterator)->GetProperties();
        }
    }
    return listToReturn;
}


// Load properties for distribution to DIALOG
QStringList BasicClassification::GetPropertiesForDistribution(QString format){
    QStringList listToReturn;
    listToReturn = BASIC_DISTRIBUTION_PROPERTIES;
    listToReturn << PROPERTY_EXTENSION;

    // If exists plugin for that format add some properties
    for(auto pluginsIterator = PluginsFormats.begin(); pluginsIterator != PluginsFormats.end(); ++pluginsIterator) {
        if(format == (*pluginsIterator)->GetName()){
            listToReturn << (*pluginsIterator)->GetPropertiesForDistribution();
        }
    }

    return listToReturn;
}


// Load file formats for GUI dialog
QStringList BasicClassification::GetFileFormats(){
    return QStringList() << GENERAL_FORMATS << SPECIFIC_FORMATS;
}

// Load file formats for GUI dialog
QString BasicClassification::GetGeneralFileFormat(QString format){

    foreach(QString str,PICTURE_FORMATS){
        if(QString::compare(format,str,Qt::CaseInsensitive) == 0){
            return PICTURES;
        }
    }

    foreach(QString str,ARCHIVE_FORMATS){
        if(QString::compare(format,str,Qt::CaseInsensitive) == 0){
            return ARCHIVES;
        }
    }

    foreach(QString str,DOCUMENT_FORMATS){
        if(QString::compare(format,str,Qt::CaseInsensitive) == 0){
            return DOCUMENTS;
        }
    }

    foreach(QString str,VIDEO_FORMATS){
        if(QString::compare(format,str,Qt::CaseInsensitive) == 0){
            return VIDEOS;
        }
    }

    foreach(QString str,AUDIO_FORMATS){
        if(QString::compare(format,str,Qt::CaseInsensitive) == 0){
            return AUDIOS;
        }
    }

    return QString();
}

// Load each property's possible operations for GUI dialog
std::vector<int> BasicClassification::GetPropertyOperations(QString property,QString fileFormat){
    for(auto propertiesIterator = properties.begin(); propertiesIterator != properties.end(); ++propertiesIterator){
        if(((*propertiesIterator)->GetName())==property)
            return (*propertiesIterator)->GetPropertyOperations();
    }

    // If exists plugin for that format try to find operations there
    for(auto pluginsIterator = PluginsFormats.begin(); pluginsIterator != PluginsFormats.end(); ++pluginsIterator) {
        if(fileFormat == (*pluginsIterator)->GetName()){
            return (*pluginsIterator)->GetPropertyOperations(property);
        }
    }

    return std::vector<int>();
}

// Load type of property for GUI dialog
int BasicClassification::GetPropertyType(QString arg,QString fileFormat){
    for(auto propertiesIterator = properties.begin(); propertiesIterator != properties.end(); ++propertiesIterator){
        if(((*propertiesIterator)->GetName())==arg)
            return (*propertiesIterator)->GetPropertyType();
    }

    // If exists plugin for that format try to find operations there
    for(auto pluginsIterator = PluginsFormats.begin(); pluginsIterator != PluginsFormats.end(); ++pluginsIterator) {
        if(fileFormat == (*pluginsIterator)->GetName()){
            return (*pluginsIterator)->GetPropertyType(arg);
        }
    }
    return 0;
}

// Returning true if format is basic format, otherwise false if attribute is general format (like Audio etc)
bool BasicClassification::IsGeneralFormat(QString attribute){

    foreach(QString str,GENERAL_FORMATS){
        if(str==attribute)
            return true;
    }
    return false;
}

// Returning true, if property is basic attribute that can be compared in this class
bool BasicClassification::IsBasicProperty(QString property){
    for(auto propertiesIterator = properties.begin(); propertiesIterator != properties.end(); ++propertiesIterator){
        if(((*propertiesIterator)->GetName())==property)
            return true;
    }
    return false;
}

// Check if file fullfilled condition in specicif branch of node
bool BasicClassification::IsFileFullfilCondition(TreeBranch * branch, File * file, QString property){

    // If is it only walkthrough branch, immediately return true
    if(branch->IsWalkThrough())
        return true;

    // If property to compare is one of basic properties, find proper method to compare
    // and there check if condition is fullfilled
    if(IsBasicProperty(property)){
        if(property == PROPERTY_SIZE){
            return IsSizeConditionTrue(branch,file);
        }
        else if(property == PROPERTY_CREATION_DATE){
            return IsCreationDateConditionTrue(branch,file);
        }
        else if(property == PROPERTY_LAST_MODIFICATION_DATE){
            return IsLastModificationDateConditionTrue(branch,file);
        }
        else if(property == PROPERTY_NAME){
            return IsNameConditionTrue(branch,file);
        }
        else if(property == PROPERTY_EXTENSION){
            return IsExtensionConditionTrue(branch,file);
        }
        else
            return false;
    }

    // In case of property from plugin it is necessary to find specific
    // method to compare this property in plugin which is destined for that
    // file format
    else{
        // If exists plugin for that format try to find truth there
        for(auto pluginsIterator = PluginsFormats.begin(); pluginsIterator != PluginsFormats.end(); ++pluginsIterator) {
            if((*pluginsIterator)->IsSupportedFormat(file->GetExtension())){
                return (*pluginsIterator)->IsFileFullfillCondition(branch,file,property);
            }
        }
        return false;
    }
}



// Comparing size
bool BasicClassification::IsSizeConditionTrue(TreeBranch * branch, File * file){
    switch(branch->GetOperand()){
        case LESS:
            return (file->GetSize() < branch->GetNumericValue()*MEGA);
        case LESS_EQUAL:
            return (file->GetSize() <= branch->GetNumericValue()*MEGA);
        case EQUAL:
            return (file->GetSize() == branch->GetNumericValue()*MEGA);
        case MORE:
            return (file->GetSize() > branch->GetNumericValue()*MEGA);
        case MORE_EQUAL:
            return (file->GetSize() >= branch->GetNumericValue()*MEGA);
    }
    return false;
}

// Comparing creation date
bool BasicClassification::IsCreationDateConditionTrue(TreeBranch * branch, File * file){
    switch(branch->GetOperand()){
        case LESS:
            return (file->GetCreationDate() < branch->GetDateValue());
        case LESS_EQUAL:
            return (file->GetCreationDate() <= branch->GetDateValue());
        case EQUAL:
            return (file->GetCreationDate() == branch->GetDateValue());
        case MORE:
            return (file->GetCreationDate() > branch->GetDateValue());
        case MORE_EQUAL:
            return (file->GetCreationDate() >= branch->GetDateValue());
    }
    return false;
}

// Comparing last modification date
bool BasicClassification::IsLastModificationDateConditionTrue(TreeBranch * branch, File * file){
    switch(branch->GetOperand()){
        case LESS:
            return (file->GetLastModificationDateDate() < branch->GetDateValue());
        case LESS_EQUAL:
            return (file->GetLastModificationDateDate() <= branch->GetDateValue());
        case EQUAL:
            return (file->GetLastModificationDateDate() == branch->GetDateValue());
        case MORE:
            return (file->GetLastModificationDateDate() > branch->GetDateValue());
        case MORE_EQUAL:
            return (file->GetLastModificationDateDate() >= branch->GetDateValue());
    }
    return false;
}

// Comparing name
bool BasicClassification::IsNameConditionTrue(TreeBranch * branch, File * file){
    switch(branch->GetOperand()){
        case EQUAL:
            return (file->GetBaseName() == branch->GetStringValue());
        case CONTAINS:
            return file->GetBaseName().contains(branch->GetStringValue(),Qt::CaseInsensitive);
        case NOT_CONTAINS:
            return !file->GetBaseName().contains(branch->GetStringValue(),Qt::CaseInsensitive);
    }
    return false;
}

// Comparing extension
bool BasicClassification::IsExtensionConditionTrue(TreeBranch * branch, File * file){
    switch(branch->GetOperand()){
        case EQUAL:
            return (file->GetExtension() == branch->GetStringValue());
        case CONTAINS:
            return file->GetExtension().contains(branch->GetStringValue(),Qt::CaseInsensitive);
        case NOT_CONTAINS:
            return !file->GetExtension().contains(branch->GetStringValue(),Qt::CaseInsensitive);
    }
    return false;
}

// Returning exact path for specific file based on file's properties
QString BasicClassification::GetDistributionPath(QString distribution,File *file){
    if(distribution.isEmpty())
        return "";

    QString result = "";
    QStringList properties = distribution.split("/");
    QString midResult;
    foreach(QString str,properties){
        midResult = GetNextFolderForDistribution(str,file);
        if(!midResult.isEmpty())
            result += midResult+"/";
    }
    return result;
}

// Returning property of specific file
QString BasicClassification::GetNextFolderForDistribution(QString property, File *file){
    if(property == "Year")
        return QString::number(file->GetCreationYear());
    else if(property == "Month")
        return QString::number(file->GetCreationMonth());
    else if(property == "Day")
        return QString::number(file->GetCreationDay());
    else if(property == "Extension")
        return file->GetExtension();
    else{
        // If exists plugin for that format try to find distribution path there
        for(auto pluginsIterator = PluginsFormats.begin(); pluginsIterator != PluginsFormats.end(); ++pluginsIterator) {
            if((*pluginsIterator)->IsSupportedFormat(file->GetExtension())){
                return (*pluginsIterator)->GetPropertyValueForDistribution(file,property);
            }
        }
    }
    return "Default";
}

