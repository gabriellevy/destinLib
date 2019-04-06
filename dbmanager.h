#ifndef DBMANAGER_H
#define DBMANAGER_H
#include <QString>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

class DbManager
{
public:
    DbManager();

    bool Initialisation(const QString& path);

private:
    QSqlDatabase m_db;
};

#endif // DBMANAGER_H
