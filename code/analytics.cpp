/**
 * @author Tabish Jabir, Ian Guenther Green
 * @brief Implementation file for the analytics class that include functions and variables
 * Description: Analytics class that stores security system information in
 * database and retrieves it in different ways
 * Date: Nov 29, 2022
 */

#include "analytics.h"

// initialize singleton pointer
Analytics* Analytics::ptrInstance = nullptr;

/**
 * Create a new Analytics Object
 * @brief Default constructor.
 */
Analytics::Analytics() {}


/**
 * Private function to initiate the analytics system, including creating a new database, and table if one does not exist
 * @author Tabish Jabir, Ian Guenther Green
 * @brief initiate the analytics system
 */
void Analytics::privateInitialize(){

    fs::path cwd = fs::current_path();
    std::string cwdpath = cwd.generic_string() + "/eyespy.db";
    std::cout << cwdpath << std::endl;

    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(QString::fromStdString(cwdpath));

    if (!m_db.open())
       {
          qDebug() << "Error: connection with database failed";
       }
       else
       {
          qDebug() << "Database: connection ok";
       }

    QSqlQuery query;
    QString create("create table if not exists AlarmRecords (starttime DATETIME, endtime DATETIME, description varchar(255));");

    if(!query.exec(create)){
        qDebug() << "error executing statement: " << query.lastError().databaseText();
    }
}


/**
 * Store a new record, or update an existing record in the database
 * @author Tabish Jabir, Ian Guenther Green
 * @brief store new information in the database
 */
void Analytics::privateStoreRecord(std::string starttime, std::string endtime, std::string description, std::string type){

    QSqlQuery query(m_db);

    QString Qstarttime = QString::fromStdString(starttime);
    QString Qendtime = QString::fromStdString(endtime);
    QString Qdesc = QString::fromStdString(description);


    if(type == "new"){

        query.prepare("insert into AlarmRecords (starttime, description) values (:stime, :desc)");
        query.bindValue(":stime",Qstarttime);
        query.bindValue(":etime",Qendtime);
        query.bindValue(":desc",Qdesc);
    }
    else if (type == "stop"){
        query.prepare("update AlarmRecords set endtime = :etime where starttime = :stime");
        query.bindValue(":stime",Qstarttime);
        query.bindValue(":etime",Qendtime);
        query.bindValue(":desc",Qdesc);
    }

    if(!query.exec())
    {
        qDebug() << "insert error:"
                 << query.lastError();
    }

}
/**
 * Query the database and return the results in the form of a 2-dimensional array
 * @author Tabish Jabir, Ian Guenther Green
 * @brief retrieve a structured query from the database
 * @param string consisting of a SQL query
 * @return 2-Dimensional Array of string representations of queried data
 */
std::array<std::array<std::string,10>, 10> Analytics::privateRetrieveTable(std::string query1){

//    cout << "retrieve query engaged" << endl;

    QSqlQuery query(m_db);

    query.prepare(QString::fromStdString(query1));

    if(!query.exec())
    {
        qDebug() << "select error:"
                 << query.lastError();
    }

//    cout << "query prepared" << endl;

    std::string starttime;
    std::string endtime;
    std::string description;

//    cout << "query size" + to_string(query.size()) << endl;

    static std::array<std::array<std::string,10>, 10> result;

    int startID = query.record().indexOf("starttime");
    int endID = query.record().indexOf("endtime");
    int descID = query.record().indexOf("description");

//    int index[10];
//    int fields_size = fields.size();


    int i = 0;
    while (query.next())

    {
//       cout << "i: " + to_string(i) << endl;
//       cout << "startID: " + to_string(startID) << endl;
       starttime = query.value(startID).toString().toStdString();
       endtime = query.value(endID).toString().toStdString();
       description = query.value(descID).toString().toStdString();

       std::string result_j;

           std::cout << "start time: " + starttime << std::endl;
           std::cout << "end time: " + endtime << std::endl;
           std::cout << "desc: " + description << std::endl;

       result[i][0] = starttime;
       result[i][1] = endtime;
       result[i][2] = description;

       i += 1;
       if (i==10){break;}

//           qDebug() << name;
    }



    return result;

}

/**
 * Reformat a string representation of a datetime from the format "DD-MM-YYYY HH:MM:SS" to "DD-MM-YYYY HH h MM m SS s"
 * @author Tabish Jabir, Ian Guenther Green
 * @brief Reformat a datetime string
 * @param string representation of a datetime value
 * @return reformatted string representation of a date and time
 */
std::string DecomposeData(std::string dateTime) {
	
    std::string date = dateTime.substr(0, dateTime.find(" "));
    std::string time = dateTime.substr(dateTime.find(" ") + 1, -1);
			
	size_t pos = 0;
	std::string token;
    std::string delimiter = ":";
		
		
	pos = time.find(delimiter);
    std::string hours = time.substr(0,pos);
	time = time.erase(0, pos + delimiter.length());
	
	pos = time.find(delimiter);
    std::string minutes = time.substr(0,pos);
	time = time.erase(0, pos + delimiter.length());
	
    std::string seconds = time;
	time = time.erase(0, pos + delimiter.length());
		
	return date + " " + hours + " h " + minutes + " m " + seconds + " s ";
		
	// cout << date + " " + hours + " h " + minutes + " m " + seconds + " s " << endl;


}
/**
 * Query the database and return the results in the form of a string
 * @author Tabish Jabir, Ian Guenther Green
 * @brief retrieve an unstructured query from the database
 * @param string consisting of a SQL query
 * @return string consisting of query results
 */
std::string Analytics::privateRetrieveUnformattedTable(std::string query1) {
	
	QSqlQuery query(m_db);

    query.prepare(QString::fromStdString(query1));

    if(!query.exec())
    {
        qDebug() << "select error:"
                 << query.lastError();
    }

    std::string table;

    static std::array<std::array<std::string,10>, 10> result;

    int startID = query.record().indexOf("starttime");
    int endID = query.record().indexOf("endtime");
    int descID = query.record().indexOf("description");

    int i = 0;
    while (query.next()) {
       table = table + "start time - " +  DecomposeData(query.value(startID).toString().toStdString()) + " end time - " + DecomposeData(query.value(endID).toString().toStdString()) + " description - " + query.value(descID).toString().toStdString() + "\n";
       i += 1;
             
       if (i==5){break;}

    }

    NotifyUser notif;

    return table;
	
}

/**
 * Query the database for a single value, and return the result in the form of a string
 * @author Tabish Jabir, Ian Guenther Green
 * @brief retrieve a single value from the database
 * @param string consisting of a SQL query for a single value
 * @return string consisting of query result
 */
std::string Analytics::privateRetrieveSingle(std::string query1){

    //    cout << "retrieve query engaged" << endl;

        QSqlQuery query(m_db);

        query.prepare(QString::fromStdString(query1));

        if(!query.exec())
        {
            qDebug() << "select error:"
                     << query.lastError();
        }

        if(!query.next()){
                qDebug() << query.lastError();
        }

        return query.value(0).toString().toStdString();

}

/**
 * Public function to initalize the Analytics system by calling privateIntialize()
 * @author Tabish Jabir, Ian Guenther Green
 * @brief initalize the Analytics system
 */
void Analytics::Initialize(){
    instance().privateInitialize();
}

/**
 * Public function to initalize the store new information in the database by calling privateStoreRecord()
 * @author Tabish Jabir, Ian Guenther Green
 * @brief store new information in the database
 */
void Analytics::storeRecord(std::string starttime, std::string endtime, std::string description, std::string type){
    instance().privateStoreRecord(starttime, endtime, description, type);
}

/**
 * Public function to retrieve a 2-D Aarray containing query results by calling privateRetrieveTable()
 * @author Tabish Jabir, Ian Guenther Green
 * @brief retrieve a structured query from the database
 * @param string consisting of a SQL query
 * @return 2-Dimensional Array of string representations of queried data
 */
std::array<std::array<std::string,10>, 10> Analytics::RetrieveTable(std::string query1){
    return instance().privateRetrieveTable(query1);
}

/**
 * Public function to retrieve a string containing query results by calling privateRetrieveUnformattedTable()
 * @author Tabish Jabir, Ian Guenther Green
 * @brief retrieve an unstructured query from the database
 * @param string consisting of a SQL query
 * @return string consisting of query results
 */
std::string Analytics::RetrieveUnformattedTable(std::string query1) {
	return instance().privateRetrieveUnformattedTable(query1);
}

/**
 * Public function to retrieve a single value from a query by calling privateRetrieveSingle()
 * @author Tabish Jabir, Ian Guenther Green
 * @brief retrieve a single value from the database
 * @param string consisting of a SQL query for a single value
 * @return string consisting of query result
 */
std::string Analytics::RetrieveSingle(std::string query1){
    return instance().privateRetrieveSingle(query1);
}
