package cnd.conflict.util;

import java.sql.*;

public class DataBaseConn {

	private static DataBaseConn dataBaseConn = null;
	public Connection conn = null;
	private String url_database = "fileservices";
	
	private DataBaseConn(){
		
	}
	
	public String getUrl_database() {
		return url_database;
	}

	public void setUrl_database(String urlDatabase) {
		url_database = urlDatabase;
	}
	
	public void createConn() {
		String DRIVER = "com.mysql.jdbc.Driver";
		
		String url = "jdbc:mysql://localhost:3308/"+this.getUrl_database()+"?user=root&password=123456&useUnicode=true&characterEncoding=gb2312";
		if (conn == null) {
			try {
				Class.forName(DRIVER).newInstance();
				conn = DriverManager.getConnection(url);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	public Connection getConn() {
		if (conn == null) {
			createConn();
		}
		return conn;
	}

	public void closeConn() {
		if (conn != null) {
			try {
				conn.close();
				conn = null;
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}

	public static DataBaseConn getDataBaseConnInstance() {
		if(dataBaseConn == null){
			dataBaseConn = new DataBaseConn();
		}
		return dataBaseConn;
	}

}
