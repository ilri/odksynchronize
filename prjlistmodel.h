#ifndef PRJLISTMODEL_H
#define PRJLISTMODEL_H

#include <QAbstractTableModel>

struct projectItem
{
  bool active;
  QString code;
  QString name;
  QString url;
};
typedef projectItem TprojectItem;

class prjListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    prjListModel(QObject *parent=0);
    ~prjListModel();

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex &index,const QVariant &value,int role = Qt::EditRole);
    Qt::ItemFlags flags (const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

    void loadProjects(QString projectsFile, QString activeProject);
    void setActiveProject(QModelIndex index);
    QString getProjectCode(QModelIndex index);
    QString getProjectUrl(QModelIndex index);
    QString getProjectName(QModelIndex index);

private:
    QList<TprojectItem> m_items;

signals:
    void logError(QString error);
};

#endif // PRJLISTMODEL_H
