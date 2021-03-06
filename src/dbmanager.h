#ifndef DBMANAGER_H
#define DBMANAGER_H

#include "db/connection.h"
#include "plugins/sqlwrapper.h"

#include <QList>
#include <QSqlDatabase>
#include <QSqlError>
#include <QStack>
#include <QStandardItemModel>
#include <QString>
#include <QThread>

/**
 * Manages the connexion pool
 *
 * @author manudwarf
 */
class DbManager : public QObject {
Q_OBJECT
public:
  enum ItemTypes {
    DbItem,
    DisplayItem,
    FieldItem,
    SchemaItem,
    SysTableItem,
    TableItem,
    ViewItem
  };

  DbManager();

  int                     addDatabase(QString driver, QString host,
                                      QString user, QString pswd, QString dbnm,
                                      QString alias, bool usesOdbc,
                                      bool save =true);
  int                     addDatabase(QString driver, QString host,
                                      QString user, QString pswd, QString dbnm,
                                      QString alias, QString wrapperName,
                                      bool save =true);
  void                    closeAll();
  QList<Connection*> connections();
  QStandardItemModel     *driverModel();
  QString                 genConnectionName();
  QStringList             getDbNames(bool);
  QString                 lastError();
  QStandardItemModel*     model();
  void                    openList();
  void                    removeDatabase(int);
  void removeDatabase(Connection* connection);
  void                    saveList();
  SqlSchema               schema(QSqlDatabase *db, QString schemaName);
  SqlTable                table(QSqlDatabase *db, QString tbl);
  void                    update(Connection* connection, QString alias);

  int lastUsedDbIndex;

  static QString          dbTitle(QSqlDatabase *db);
  static void             init();
  static DbManager       *instance;

public slots:
  void                    refreshModel();
  void                    refreshModelIndex(QModelIndex index);
  void                    refreshModelItem(QSqlDatabase *db);
  void refreshModelItem(Connection* connection);

private:
  QStandardItem*          columnsItem(QList<SqlColumn> columns);
  QString                 dbToolTip(QSqlDatabase *db);
  QSqlDatabase*           parentDb(QModelIndex index);
  void                    setupConnections();
  void                    setupModels();
  QStandardItem*          schemaItem(SqlSchema schema);
  QStandardItem*          tablesItem(QList<SqlTable> tables,
                                     QString schema ="");
  QStandardItem*          viewsItem(QList<SqlTable> tables,
                                    QString schema ="");

  QMap<QString, QString>  driverAlias;
  QMap<QString, QIcon>    driverIcon;

  bool                    closingAll;
  QList<Connection*> m_connections;
  QStandardItemModel     *m_driverModel;
  QMap<QSqlDatabase*, QStandardItem*> dbMap;
  QMap<QSqlDatabase*, SqlWrapper*> dbWrappers;
  QStandardItemModel     *m_model;
  int                     nconn;
  QString                 lastErr;

  QStack<QSqlDatabase*>   closeStack;
  QStack<QSqlDatabase*>   openStack;

private slots:
  void refreshModelItem();
  void updateLastDbIndex();

};

#endif // DBMANAGER_H
