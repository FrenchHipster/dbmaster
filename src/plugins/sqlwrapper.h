/*
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 */


#ifndef WRAPPER_H
#define WRAPPER_H

#include "plugin.h"
#include "../db_enum.h"

#include <QList>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QString>
#include <QStringList>

class SqlWrapper : public Plugin {
public:
  enum WrapperFeature {
    BasicFeatures     = 0x0000,
    CustomTypes       = 0x0001,
    Dumps             = 0x0002,
    ForeignKeys       = 0x0004,
    Indexes           = 0x0008,
    Schemas           = 0x0010,
    StoredProcedures  = 0x0020,
    Triggers          = 0x0040,
    Views             = 0x0080
  };
  Q_DECLARE_FLAGS(WrapperFeatures, WrapperFeature)


  virtual SqlWrapper* newInstance(QSqlDatabase *db) =0;

  virtual QString driver() =0;

  virtual WrapperFeatures features() =0;

  /**
   * Retourne la liste des schémas disponibles sur cette connexion.
   *
   * @param fillTables
   *    Cette option permet à l'adaptateur de remplir la liste des tables pour
   *    chaque schéma. Voir la variable tables du type SqlSchema.
   *    Noter que cette option est par défaut à vrai.
   *
   * @return une liste vide si la fonctionnalité n'est pas supportée.
   */
  virtual QList<SqlSchema> schemas() { return QList<SqlSchema>(); };

  virtual QList<SqlTable> tables() { return QList<SqlTable>(); };

  virtual bool supportsOdbc() { return true; };

  QSqlDatabase* db() { return m_db; };

protected:
  QSqlDatabase *m_db;
};

Q_DECLARE_INTERFACE(SqlWrapper, "dbmaster.Wrapper")
Q_DECLARE_OPERATORS_FOR_FLAGS(SqlWrapper::WrapperFeatures)

#endif // WRAPPER_H