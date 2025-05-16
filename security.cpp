#include "security.h"
#include <QCryptographicHash>
#include <QDebug>

// 单例实现
SecurityManager& SecurityManager::instance() {
    static SecurityManager instance;
    return instance;
}

// 构造函数加载权限
SecurityManager::SecurityManager() {
    loadPermissions();
}

QList<QString> SecurityManager::getAllDatabases() const {
    return dbOwners.keys(); // 数据库名列表（所有者记录）
}


// 注册用户
bool SecurityManager::registerUser(const QString& username, const QString& password) {
    QFile file(usersFilePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.startsWith(username + ":")) {
                return false; // 用户已存在
            }
        }
        file.close();
    }

    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << username << ":" << password << "\n";
        file.close();
        return true;
    }

    return false;
}

// 验证用户
bool SecurityManager::authenticate(const QString& username, const QString& password) {
    QFile file(usersFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(":");
        if (parts.size() == 2 && parts[0] == username && parts[1] == password) {
            return true;
        }
    }

    return false;
}

// 授予权限
void SecurityManager::grantPermission(const QString& owner, const QString& username, const QString& dbName, Permission permission) {
    if (!isDbOwner(owner, dbName)) {
        qDebug() << "[Security] 非所有者无法授权";
        return;
    }

    userPermissions[username][dbName].insert(permission);
    savePermissions();
}


// 撤销权限
void SecurityManager::revokePermission(const QString& owner, const QString& username, const QString& dbName, Permission permission) {
    if (!isDbOwner(owner, dbName)) return;

    userPermissions[username][dbName].remove(permission);
    savePermissions();
}

// 权限判断
bool SecurityManager::hasPermission(const QString& username, const QString& dbName, Permission permission) const {
    if (permission == Permission::CREATE_DB)
        return true; // 所有用户默认可创建数据库

    if (isDbOwner(username, dbName))
        return true; // 所有者拥有所有权限

    return userPermissions.value(username).value(dbName).contains(permission);
}


// 返回全部权限集合
QSet<Permission> SecurityManager::getUserPermissions(const QString& username, const QString& dbName) const {
    return userPermissions.value(username).value(dbName);
}

// 设置数据库所有者
void SecurityManager::setDbOwner(const QString& dbName, const QString& username) {
    dbOwners[dbName] = username;
    savePermissions();
}

// 判断是否是所有者
bool SecurityManager::isDbOwner(const QString& username, const QString& dbName) const {
    return dbOwners.value(dbName) == username;
}

// 保存权限文件（字符格式）
void SecurityManager::savePermissions() {
    QFile file(permissionsFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return;

    QTextStream out(&file);
    // 保存权限
    for (const QString& user : userPermissions.keys()) {
        for (const QString& db : userPermissions[user].keys()) {
            QSet<Permission> perms = userPermissions[user][db];
            if (perms.isEmpty()) continue;

            QStringList permList;
            for (const Permission& p : perms) {
                permList << permissionToString(p);
            }

            out << user << ":" << db << ":" << permList.join(",") << "\n";
        }
    }

    // 保存数据库所有者
    for (const QString& db : dbOwners.keys()) {
        out << "__OWNER__:" << db << ":" << dbOwners[db] << "\n";
    }

    file.close();
}

// 加载权限文件
void SecurityManager::loadPermissions() {
    userPermissions.clear();
    dbOwners.clear();

    QFile file(permissionsFilePath);
    if (!file.exists() || !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "[Security] permissions.txt 不存在或无法打开，跳过加载";
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(":");
        if (parts.size() != 3) {
            qWarning() << "[Security] 跳过格式错误的行:" << line;
            continue;
        }

        QString user = parts[0];
        QString db = parts[1];
        QString data = parts[2];

        if (user == "__OWNER__") {
            dbOwners[db] = data;
        } else {
            QStringList permList = data.split(",", Qt::SkipEmptyParts);
            for (const QString& pStr : permList) {
                try {
                    Permission p = stringToPermission(pStr.trimmed());
                    userPermissions[user][db].insert(p);
                } catch (const std::invalid_argument&) {
                    qWarning() << "[Security] 跳过无效权限字符串:" << pStr << " 来自行:" << line;
                }
            }
        }
    }

    file.close();
}


// 将权限转为字符串
QString permissionToString(Permission perm) {
    switch (perm) {
    case Permission::CREATE_DB: return "CREATE_DB";
    case Permission::DROP_DB: return "DROP_DB";
    case Permission::CREATE_TABLE: return "CREATE_TABLE";
    case Permission::DROP_TABLE: return "DROP_TABLE";
    case Permission::ALTER_TABLE: return "ALTER_TABLE";
    case Permission::INSERT_DATA: return "INSERT_DATA";
    case Permission::SELECT_DATA: return "SELECT_DATA";
    case Permission::UPDATE_DATA: return "UPDATE_DATA";
    case Permission::DELETE_DATA: return "DELETE_DATA";
    case Permission::VIEW_DB: return "VIEW_DB";
    case Permission::USE_DB: return "USE_DB";
    default: return "UNKNOWN";
    }
}

// 将字符串解析为权限
Permission stringToPermission(const QString& str) {
    if (str == "CREATE_DB") return Permission::CREATE_DB;
    if (str == "DROP_DB") return Permission::DROP_DB;
    if (str == "CREATE_TABLE") return Permission::CREATE_TABLE;
    if (str == "DROP_TABLE") return Permission::DROP_TABLE;
    if (str == "ALTER_TABLE") return Permission::ALTER_TABLE;
    if (str == "INSERT_DATA") return Permission::INSERT_DATA;
    if (str == "SELECT_DATA") return Permission::SELECT_DATA;
    if (str == "UPDATE_DATA") return Permission::UPDATE_DATA;
    if (str == "DELETE_DATA") return Permission::DELETE_DATA;
    if (str == "VIEW_DB") return Permission::VIEW_DB;
    if (str == "USE_DB") return Permission::USE_DB;

    throw std::invalid_argument("Invalid permission: " + str.toStdString());
}
