/**********************************/
// This file is part of SmartSorter
// Author: Radim Svacek
// Created: 2015
/**********************************/

#include "pdf_plugin.h"

// Plugin initialization
// Create properities which can be compared
void PDF_plugin::Initialize(){
    Attribute *a;
    std::vector<int> ops;

    ops.push_back(LESS);
    ops.push_back(LESS_EQUAL);
    ops.push_back(MORE);
    ops.push_back(MORE_EQUAL);
    ops.push_back(EQUAL);
    a = new Attribute(PROPERTY_PAGES,ops,NUM_TYPE);
    properties.push_back(a);


    ops.clear();
    ops.push_back(CONTAINS);
    ops.push_back(NOT_CONTAINS);

    a = new Attribute(PROPERTY_CONTENT,ops,TEXT_TYPE);
    properties.push_back(a);

    a = new Attribute(PROPERTY_FIRST_PAGE,ops,TEXT_TYPE);
    properties.push_back(a);

    ops.push_back(EQUAL);

    a = new Attribute(PROPERTY_META_AUTHOR,ops,TEXT_TYPE);
    properties.push_back(a);

    a = new Attribute(PROPERTY_META_TITLE,ops,TEXT_TYPE);
    properties.push_back(a);

    a = new Attribute(PROPERTY_META_SUBJECT,ops,TEXT_TYPE);
    properties.push_back(a);

    ops.clear();

}

PDF_plugin::~PDF_plugin(){
    for(auto propertyIterator = properties.begin(); propertyIterator != properties.end(); ++propertyIterator) {
        delete (*propertyIterator);
    }
    properties.clear();
}

// PDF PLUGIN
QString PDF_plugin::GetName()
{
    return "PDF";
}

// Supported extensions
QStringList PDF_plugin::GetExtensions()
{
    return AVIABLE_EXTENSIONS;
}

// Properties supported by this plugin
QStringList PDF_plugin::GetProperties()
{
    QStringList list;

    // Choose basic properties
    for(auto propertyIterator = properties.begin(); propertyIterator != properties.end(); ++propertyIterator) {
        list << (*propertyIterator)->GetName();
    }

    return list;
}

// Returns properties which can be used for distribution
QStringList PDF_plugin::GetPropertiesForDistribution(){
    return PROPERTIES_FOR_DISTRIBUTION;
}

// Returns vector of property operations
std::vector<int> PDF_plugin::GetPropertyOperations(QString property){
    for(auto propertyIterator = properties.begin(); propertyIterator != properties.end(); ++propertyIterator){
        if(((*propertyIterator)->GetName())==property)
            return (*propertyIterator)->GetPropertyOperations();
    }
    return std::vector<int>();
}

// Returns type of specific property
int PDF_plugin::GetPropertyType(QString arg)
{
    for(auto propertyIterator = properties.begin(); propertyIterator != properties.end(); ++propertyIterator){
        if(((*propertyIterator)->GetName())==arg)
            return (*propertyIterator)->GetPropertyType();
    }
    return 0;
}

// Returns if fileFormat is supported by this plugin
bool PDF_plugin::IsSupportedFormat(QString fileFormat){
    foreach(QString str,AVIABLE_EXTENSIONS){
        if(str.compare(fileFormat,Qt::CaseInsensitive) == 0)
            return true;
    }
    return false;
}

// Returns values for file distribution
QString PDF_plugin::GetPropertyValueForDistribution(File *file, QString property){
    if(!PROPERTIES_FOR_DISTRIBUTION.contains(property))
        return "";

    if(property == PROPERTY_META_AUTHOR){
        // Load PDF document
        Poppler::Document* document = Poppler::Document::load(file->GetPath());
        if (!document || document->isLocked()) {
            delete document;
            return "Default";
        }
        if (document == 0) {
            return "Default";
        }
        return document->info("Author");
    }
    return "";
}

// Resolver for properties in this plugin
bool PDF_plugin::IsFileFullfillCondition(TreeBranch *branch, File *file, QString property){
    bool returnValue = false;
    // Load PDF document
    Poppler::Document* document = Poppler::Document::load(file->GetPath());
    if (!document || document->isLocked()) {
        delete document;
        return false;
    }
    if (document == 0) {
        return false;
    }

    // Choose right property to compare and call its method
    if(property == PROPERTY_META_AUTHOR){
        returnValue = IsStringConditionTrue(branch,document->info("Author"));
    }
    else if(property == PROPERTY_META_TITLE){
        returnValue = IsStringConditionTrue(branch,document->info("Title"));
    }
    else if(property == PROPERTY_META_SUBJECT){
        returnValue = IsStringConditionTrue(branch,document->info("Subject"));
    }
    else if(property == PROPERTY_PAGES){
        returnValue = IsPagesConditionTrue(branch,document->numPages());
    }
    else if(property == PROPERTY_CONTENT){
        returnValue = IsContentConditionTrue(branch,document,false);
    }
    else if(property == PROPERTY_FIRST_PAGE){
        returnValue = IsContentConditionTrue(branch,document,true);
    }

    delete document;

    // Otherwise return false, never should gets there
    return returnValue;
}

// Compare string value
bool PDF_plugin::IsStringConditionTrue(TreeBranch *branch, QString propertyValue){
    switch(branch->GetOperand()){
        case EQUAL:
            return (propertyValue.compare(branch->GetStringValue(),Qt::CaseInsensitive) == 0);
        case CONTAINS:
            return propertyValue.contains(branch->GetStringValue(),Qt::CaseInsensitive);
        case NOT_CONTAINS:
            return !propertyValue.contains(branch->GetStringValue(),Qt::CaseInsensitive);
    }
    return false;
}

// Check if pages count is acceptable
bool PDF_plugin::IsPagesConditionTrue(TreeBranch *branch, int pages){
    switch(branch->GetOperand()){
        case LESS:
            return (pages < branch->GetNumericValue());
        case LESS_EQUAL:
            return (pages <= branch->GetNumericValue());
        case EQUAL:
            return (pages == branch->GetNumericValue());
        case MORE:
            return (pages > branch->GetNumericValue());
        case MORE_EQUAL:
            return (pages >= branch->GetNumericValue());
    }
    return false;
}

// Check if PDF document contains some text
bool PDF_plugin::IsContentConditionTrue(TreeBranch *branch, Poppler::Document* document, bool onlyFirstPage){
    // Pages to search in
    int pagesCount = document->numPages();
    if(onlyFirstPage)
        pagesCount=1;

    // Some variables which will be needed
    Poppler::Page* pdfPage;
    QRectF rec = QRectF();
    bool found = false;
    int i = 0;
    // Access page of the PDF file
    // Document starts at page 0

    // Loop to find text in PDF document
    while(!found && i<pagesCount){
        pdfPage = document->page(i);
        if (pdfPage == 0) {
            return false;
        }
        const QString what = branch->GetStringValue();

        found = pdfPage->search(what,rec,Poppler::Page::NextResult,Poppler::Page::CaseInsensitive);
        i++;
        delete pdfPage;
    }

    // Return value based on property
    switch(branch->GetOperand()){
        case CONTAINS:
            return found;
        case NOT_CONTAINS:
            return !found;
    }

    // Otherwise return false, but should never gets there
    return false;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(pdf_plugin,PDF_plugin)
QT_END_NAMESPACE
