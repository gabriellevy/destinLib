#include "dbmanager.h"
#include <QDebug>

DbManager::DbManager()
{
   m_db = QSqlDatabase::addDatabase("QSQLITE");
}

bool DbManager::Initialisation(const QString& path)
{
    m_db.setDatabaseName(path);

    if (!m_db.open())
    {
       qDebug() << "Error: connection with database failed";
       return false;
    }
    else
    {
       qDebug() << "Database: connection ok";
       return true;
    }
}
