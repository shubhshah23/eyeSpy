/**
 * @author Tabish Jabir, Ian Guenther Green
 * @brief header file for the analytics class that include functions and variables
 * Description: Analytics Singleton class that stores security system information in
 * database and retrieves it
 * Date: Nov 29, 2022
 */

#ifndef ANALYTICS_H
#define ANALYTICS_H

// ensures file system library is included properly
#ifndef __has_include
  static_assert(false, "__has_include not supported");
#else
#  if __cplusplus >= 201703L && __has_include(<filesystem>)
#    include <filesystem>
     namespace fs = std::filesystem;
#  elif __has_include(<experimental/filesystem>)
#    include <experimental/filesystem>
     namespace fs = std::experimental::filesystem;
#  elif __has_include(<boost/filesystem.hpp>)
#    include <boost/filesystem.hpp>
     namespace fs = boost::filesystem;
#  endif
#endif

#include <QDebug>
#include <iostream>
#include <cstring>
#include <string>
#include <array>
#include <regex>

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQueryModel>

#include "notifyuser.h"

class Analytics
{
public:

    // function that initializs security system database
    static void Initialize();
    // function to store a record
    static void storeRecord(std::string starttime, std::string endtime, std::string description, std::string type);
    // functions to retrieve records in different ways
    static std::array<std::array<std::string,10>, 10> RetrieveTable(std::string query1);
    static std::string RetrieveSingle(std::string query);
    static std::string RetrieveUnformattedTable(std::string query1);

private:

    // Qt sql database pointer
    QSqlDatabase m_db;

    Analytics();
    ~Analytics() {}

    // singleton functionality
    static Analytics* ptrInstance;
    static Analytics& instance(){

        if(!ptrInstance){
            ptrInstance = new Analytics();
        }
        return *ptrInstance;
    }

    // private versions of above functions
    void privateInitialize();
    void privateStoreRecord(std::string starttime, std::string endtime, std::string description, std::string type);
    std::array<std::array<std::string,10>, 10> privateRetrieveTable(std::string query1);
    std::string privateRetrieveUnformattedTable(std::string query1);
    std::string privateRetrieveSingle(std::string query1);

};

#endif // ANALYTICS_H
