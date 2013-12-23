#include "outputtemplate.h"

OutputTemplate::OutputTemplate(QObject *parent) :
    QObject(parent)
{
    this->setParent(parent);

}
void OutputTemplate::getTemplate (QSqlQuery *query, int division, int type){
    query->exec("SELECT templates.template FROM public.templates"
                "WHERE templates.div_id = " + QString(division) + " AND templates.type =" + QString(type)+ ";");
    if (query->next())
    {
        header = query->value(0).toString();
    }
    else
    {
        header = "";
    }


}

QString OutputTemplate::getHeader(){
    return this->header;
}
