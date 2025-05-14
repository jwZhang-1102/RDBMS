#ifndef SECURITY_H
#define SECURITY_H

#include <QString>
#include <QMap>
#include <QSet>
#include <QFile>
#include <QTextStream>
#include <QList>

// 权限枚举
enum class Permission {
    CREATE_DB,
    DROP_DB,
    CREATE_TABLE,
    DROP_TABLE,
    ALTER_TABLE,
    INSERT_DATA,
    SELECT_DATA,
    UPDATE_DATA,
    DELETE_DATA,
    VIEW_DB,
    USE_DB
};

class SecurityManager {
public:
    static SecurityManager& instance();

    QList<QString> getAllDatabases() const;


    // 用户管理
    bool registerUser(const QString& username, const QString& password);
    bool authenticate(const QString& username, const QString& password);

    // 权限管理
    void grantPermission(const QString& owner, const QString& username, const QString& dbName, Permission permission);
    void revokePermission(const QString& owner, const QString& username, const QString& dbName, Permission permission);
    bool hasPermission(const QString& username, const QString& dbName, Permission permission) const;
    QSet<Permission> getUserPermissions(const QString& username, const QString& dbName) const;

    // 所有者管理
    void setDbOwner(const QString& dbName, const QString& username);
    bool isDbOwner(const QString& username, const QString& dbName) const;

private:
    SecurityManager();
    void loadPermissions();
    void savePermissions();

    QString usersFilePath = "D:\\数据库实训\\DBMS\\user.txt";
    QString permissionsFilePath = "D:\\数据库实训\\DBMS\\permissions.txt";

    QMap<QString, QString> users;  // 用户名 -> 密码
    QMap<QString, QString> dbOwners; // 数据库名 -> 所有者
    QMap<QString, QMap<QString, QSet<Permission>>> userPermissions; // 用户名 -> 数据库 -> 权限集合
};

QString permissionToString(Permission perm);
Permission stringToPermission(const QString& str);

#endif // SECURITY_H
