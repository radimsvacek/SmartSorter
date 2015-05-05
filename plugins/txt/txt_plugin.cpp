#include "txt_plugin.h"

// Plugin initialization
// Create properities which can be compared
void TXT_plugin::Initialize(){
    Attribute *a;
    std::vector<int> ops;

    ops.push_back(LESS);
    ops.push_back(LESS_EQUAL);
    ops.push_back(MORE);
    ops.push_back(MORE_EQUAL);
    ops.push_back(EQUAL);

    a = new Attribute(PROPERTY_ROWS,ops,NUM_TYPE);
    properties.push_back(a);

    a = new Attribute(PROPERTY_WORDS,ops,NUM_TYPE);
    properties.push_back(a);

    a = new Attribute(PROPERTY_CHARS,ops,NUM_TYPE);
    properties.push_back(a);

    ops.clear();
    ops.push_back(CONTAINS);
    ops.push_back(NOT_CONTAINS);

    a = new Attribute(PROPERTY_CONTENT,ops,TEXT_TYPE);
    properties.push_back(a);

    ops.clear();
}

TXT_plugin::~TXT_plugin(){
    for(auto propertyIterator = properties.begin(); propertyIterator != properties.end(); ++propertyIterator) {
        delete (*propertyIterator);
    }
    properties.clear();
}

// Returns file format of plugin
QString TXT_plugin::GetName()
{
    return "TXT";
}

// Returns all possible extensions
QStringList TXT_plugin::GetExtensions()
{
    return AVIABLE_EXTENSIONS;
}

// Returns all possible properities which can be compared
QStringList TXT_plugin::GetProperties()
{
    QStringList list;

    // Choose basic properties
    for(auto propertyIterator = properties.begin(); propertyIterator != properties.end(); ++propertyIterator) {
        list << (*propertyIterator)->GetName();
    }

    return list;
}

// Returns all possible properities which can be used for file distribution
QStringList TXT_plugin::GetPropertiesForDistribution(){
    return PROPERTIES_FOR_DISTRIBUTION;
}

// Returns all possible operations which can be done which specific property
std::vector<int> TXT_plugin::GetPropertyOperations(QString property){
    for(auto propertyIterator = properties.begin(); propertyIterator != properties.end(); ++propertyIterator){
        if(((*propertyIterator)->GetName())==property)
            return (*propertyIterator)->GetPropertyOperations();
    }
    return std::vector<int>();
}

// Returns type of specicif property
int TXT_plugin::GetPropertyType(QString arg)
{
    for(auto propertyIterator = properties.begin(); propertyIterator != properties.end(); ++propertyIterator){
        if(((*propertyIterator)->GetName())==arg)
            return (*propertyIterator)->GetPropertyType();
    }
    return 0;
}

// Check if file format is supported by this plugin
bool TXT_plugin::IsSupportedFormat(QString fileFormat){
    foreach(QString str,AVIABLE_EXTENSIONS){
        if(str.compare(fileFormat,Qt::CaseInsensitive) == 0)
            return true;
    }
    return false;
}

// Get value of file for distribution
QString TXT_plugin::GetPropertyValueForDistribution(File *, QString property){
    if(!PROPERTIES_FOR_DISTRIBUTION.contains(property))
        return "";

    return "Default";
}

// Check if file fullfill specific condition saved in branch
bool TXT_plugin::IsFileFullfillCondition(TreeBranch *branch, File *file, QString property){

    if(property == PROPERTY_CONTENT){
        return IsContentConditionTrue(branch,file);
    }
    else if(property == PROPERTY_ROWS){
        return IsRowsConditionTrue(branch,file);
    }
    else if(property == PROPERTY_WORDS){
        return IsWordsConditionTrue(branch,file);
    }
    else if(property == PROPERTY_CHARS){
        return IsCharsConditionTrue(branch,file);
    }
    else
        return false;

}

// Comparing content
bool TXT_plugin::IsContentConditionTrue(TreeBranch * branch, File * file){
    QFile F(file->GetPath());
    if(!F.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QString content = F.readAll();
    bool isInside = content.contains(branch->GetStringValue(),Qt::CaseInsensitive);
    F.close();

    switch(branch->GetOperand()){
        case CONTAINS:
            return isInside;
        case NOT_CONTAINS:
            return !isInside;
    }
    return false;
}

// Comparing row in file
bool TXT_plugin::IsRowsConditionTrue(TreeBranch * branch, File * file){
    // Get number of lines in file
    int rows = GetLinesCount(file);

    switch(branch->GetOperand()){
        case LESS:
            return (rows < branch->GetNumericValue());
        case LESS_EQUAL:
            return (rows <= branch->GetNumericValue());
        case EQUAL:
            return (rows == branch->GetNumericValue());
        case MORE:
            return (rows > branch->GetNumericValue());
        case MORE_EQUAL:
            return (rows >= branch->GetNumericValue());
    }
    return false;
}

// Comparing words count in file
bool TXT_plugin::IsWordsConditionTrue(TreeBranch * branch, File * file){
    int words = GetWordsCount(file);

    switch(branch->GetOperand()){
        case LESS:
            return (words < branch->GetNumericValue());
        case LESS_EQUAL:
            return (words <= branch->GetNumericValue());
        case EQUAL:
            return (words == branch->GetNumericValue());
        case MORE:
            return (words > branch->GetNumericValue());
        case MORE_EQUAL:
            return (words >= branch->GetNumericValue());
    }
    return false;
}

// Comparing chars count in file
bool TXT_plugin::IsCharsConditionTrue(TreeBranch * branch, File * file){
    int chars = GetCharsCount(file);

    switch(branch->GetOperand()){
        case LESS:
            return (chars < branch->GetNumericValue());
        case LESS_EQUAL:
            return (chars <= branch->GetNumericValue());
        case EQUAL:
            return (chars == branch->GetNumericValue());
        case MORE:
            return (chars > branch->GetNumericValue());
        case MORE_EQUAL:
            return (chars >= branch->GetNumericValue());
    }
    return false;
}

// Get rows count of file in argument
int TXT_plugin::GetLinesCount(File *file){
    int rows;
    QFile F(file->GetPath());
    if(!F.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    while(!F.atEnd()){
        F.readLine();
        rows++;
    }
    F.close();
    return rows;
}

// Get words count of file in argument
int TXT_plugin::GetWordsCount(File *file){

    QFile F(file->GetPath());
    if(!F.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QString content = F.readAll();
    content=content.simplified();
    QStringList list = content.split(" ");
    F.close();
    return list.size();
}

// Get chars count of file in argument
int TXT_plugin::GetCharsCount(File *file){
    QFile F(file->GetPath());
    if(!F.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QString content = F.readAll();
    content=content.simplified();
    content.replace(" ","");
    F.close();
    return content.size();
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(txt_plugin,TXT_plugin)
QT_END_NAMESPACE
