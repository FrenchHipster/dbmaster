#include "sqlitewrapper.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QVariant>

SqliteWrapper::SqliteWrapper(QObject *parent)
  : QObject(parent) {
}

SqliteWrapper::SqliteWrapper(QSqlDatabase db)
  : QObject(NULL) {
  m_db = db;
}

QList<SqlColumn> SqliteWrapper::columns(QString table) {
  QList<SqlColumn> cols;

  QString sql;

  sql += "PRAGMA TABLE_INFO('" + table + "')";

  if (!m_db.isOpen()) {
    m_db.open();
  }

  QSqlQuery query(m_db);
  if (!query.exec(sql)) {
    qDebug() << query.lastError().text();
    return cols;
  }

  SqlColumn c;
  while (query.next()) {
    c.name = query.value(1).toString();
    c.type.name = query.value(2).toString();
    c.permitsNull = query.value(3).toBool();
    c.defaultValue = query.value(4);
    c.primaryKey = query.value(5).toBool();

    cols << c;
  }

  query.clear();
  m_db.close();

  return cols;
}

SqlWrapper::WrapperFeatures SqliteWrapper::features() {
  return BasicFeatures;
}

SqlWrapper* SqliteWrapper::newInstance(QSqlDatabase db) {
  return new SqliteWrapper(db);
}

SqlTable SqliteWrapper::table(QString t) {
  SqlTable table;
  table.name = t;

  QString sql;

  sql += "PRAGMA TABLE_INFO('" + t + "')";

  if (!m_db.isOpen()) {
    m_db.open();
  }

  QSqlQuery query(m_db);
  if (!query.exec(sql)) {
    qDebug() << query.lastError().text();
    return table;
  }

  SqlColumn c;
  while (query.next()) {
    c.name = query.value(1).toString();
    c.type.name = query.value(2).toString();
    c.permitsNull = query.value(3).toBool();
    c.defaultValue = query.value(4);
    c.primaryKey = query.value(5).toBool();

    table.columns << c;
  }

  query.clear();
  m_db.close();

  return table;
}

QList<SqlTable> SqliteWrapper::tables() {
  QList<SqlTable> tables;

  QString sql;

  sql += "SELECT name, type FROM sqlite_master ";
  sql += "WHERE type in ('table', 'view') ";
  sql += "ORDER BY name ";

  if (!m_db.isOpen()) {
    m_db.open();
  }

  QSqlQuery query(m_db);
  if (!query.exec(sql)) {
    qDebug() << query.lastError().text();
    m_db.close();
    return tables;
  }

  SqlTable t;
  while (query.next()) {
    t.name = query.value(0).toString();
    t.type = query.value(1).toString() == "table" ? Table : ViewTable;
    tables << t;
  }

  query.clear();
  m_db.close();

  return tables;
}

Q_EXPORT_PLUGIN2(dbm_sqlite_wrapper, SqliteWrapper)
