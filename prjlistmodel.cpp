#include "prjlistmodel.h"
#include <QDomDocument>
#include <QDomNode>
#include <QDomElement>
#include <QFile>

prjListModel::prjListModel(QObject *parent)
    :QAbstractTableModel(parent)
{
    //m_impModule = 0;
}

prjListModel::~prjListModel()
{
    //qDebug() << "Destroy: inputsTableModel";
}

int prjListModel::rowCount(const QModelIndex &) const
{
    return m_items.count();
}

int prjListModel::columnCount(const QModelIndex &) const
{
    return 3;
}

QVariant prjListModel::data(const QModelIndex &index, int role) const
{
    if ((role == Qt::DisplayRole) || (role == Qt::EditRole))
    {
        if (index.column() == 0)
        {
            if (m_items[index.row()].active == true)
                return "Yes";
            else
                return "No";
        }
        if (index.column() == 1)
            return m_items[index.row()].name;
        if (index.column() == 2)
            return m_items[index.row()].url;
    }
    return QVariant();
}

bool prjListModel::setData(const QModelIndex &/*index*/,const QVariant &/*value*/,int /*role*/)
{
    return false;
}

Qt::ItemFlags prjListModel::flags(const QModelIndex &) const
{
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant prjListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if (section == 0)
                return "Active";
            if (section == 1)
                return "Name";
            if (section == 2)
                return "URL";
        }
    }
    return QVariant();
}

void prjListModel::loadProjects(QString projectsFile,QString activeProject)
{
    this->beginResetModel();
    m_items.clear();
    QDomDocument projectDoc("projectsFile");
    QFile prjFile(projectsFile);
    if (!prjFile.open(QIODevice::ReadOnly))
    {
        emit this->logError("Cannot open projects file");
        return;
    }
    if (!projectDoc.setContent(&prjFile))
    {
        emit this->logError("Cannot parse document for projects file");
        prjFile.close();
        return;
    }

    prjFile.close();
    QDomNodeList prjs = projectDoc.elementsByTagName("project");
    for (int pos = 0; pos <= prjs.count()-1;pos++)
    {
        TprojectItem item;
        item.code = prjs.item(pos).toElement().attribute("id","None");
        item.name = prjs.item(pos).toElement().attribute("name","None");
        item.url = prjs.item(pos).toElement().attribute("url","None");
       if (item.code == activeProject)
           item.active = true;
       else
           item.active = false;
       m_items.append(item);
    }
    this->endResetModel();
}

void prjListModel::setActiveProject(QModelIndex index)
{
    for (int pos = 0; pos <= m_items.count()-1;pos++)
    {
        m_items[pos].active = false;
    }

    m_items[index.row()].active = true;

    QModelIndex begin;
    begin = this->index(0,0);
    QModelIndex end;
    end = this->index(this->rowCount()-1,0);
    emit dataChanged(begin,end);
}

QString prjListModel::getProjectCode(QModelIndex index)
{
    return  m_items[index.row()].code;
}

QString prjListModel::getProjectUrl(QModelIndex index)
{
    return m_items[index.row()].url;
}

QString prjListModel::getProjectName(QModelIndex index)
{
    return m_items[index.row()].name;
}
