/*
 ********************************************************************************
 TREND MICRO HIGHLY CONFIDENTIAL INFORMATION:
 THIS SOFTWARE CONTAINS CONFIDENTIAL INFORMATION AND TRADE SECRETS OF TREND MICRO
 INCORPORATED AND MAY BE PROTECTED BY ONE OR MORE PATENTS. USE, DISCLOSURE, OR
 REPRODUCTION OF ANY PORTION OF THIS SOFTWARE IS PROHIBITED WITHOUT THE PRIOR
 EXPRESS WRITTEN PERMISSION OF TREND MICRO INCORPORATED.
 Copyright 2009 Trend Micro Incorporated. All rights reserved as an unpublished
 work.
 ********************************************************************************
 */
package com.king.server.db.impl;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.Date;
import java.util.ArrayList;

import javax.sql.DataSource;

import org.apache.commons.codec.binary.Hex;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.king.server.data.User;
import com.king.server.db.UserManager;
import com.king.server.util.Sha1Util;


public class UserManagerImpl implements UserManager {
	private static Log logger = LogFactory.getLog(UserManagerImpl.class);
	private DataSource ds = null;

	private Connection connect() throws SQLException {
		Connection conn = ds.getConnection();
		return conn;
	}

	public UserManagerImpl(DataSource ds) {
		this.ds = ds;
	}
	
	private void cleanup(ResultSet rs, Statement st, Connection conn) {
		try {
			if (rs != null) {
				rs.close();
			}
			if (st != null) {
				st.close();
			}
			if (conn != null) {
				conn.close();
			}
		} catch (SQLException e) {
			logger.error("db error", e);
		}
	}
	
	/**
	 * @param st
	 * @param conn
	 */
	private void cleanup(PreparedStatement st, Connection conn) {
		try {
			if (st != null) {
				st.close();
			}
			if (conn != null) {
				conn.close();
			}
		} catch (SQLException e) {
			logger.error("db error", e);
		}
	}
	
	public boolean isUserExist(String userName) {
		boolean result = false;
		Connection conn = null;
		ResultSet rs = null;
		PreparedStatement st = null;
		
		try {
			conn = connect();
			st = conn.prepareStatement("SELECT Password, Email, Creater, CreateTime, Role FROM UserInfo WHERE UserName=?");
			st.setString(1, userName);
			rs = st.executeQuery();
			
			if (rs.next()) {
				result = true;
			}
		} catch (SQLException e) {
			logger.error("db error", e);
		} finally {
			cleanup(rs, st, conn);
		}
		
		return result;
	}

	public User findUser(String userName) {
		User result = null;
		Connection conn = null;
		ResultSet rs = null;
		PreparedStatement st = null;
		
		try {
			conn = connect();
			st = conn.prepareStatement("SELECT Password, Email, LastModifyTime, Role FROM UserInfo WHERE UserName=?");
			st.setString(1, userName);
			rs = st.executeQuery();
			
			if (rs.next()) {
				result = new User(userName);
				result.setPassword(rs.getString("Password"));
				result.setEmail(rs.getString("Email"));
				result.setLastModifyTime(rs.getDate("LastModifyTime"));
				result.setRole(rs.getInt("Role"));
			}
		} catch (SQLException e) {
			logger.error("db error", e);
		} finally {
			cleanup(rs, st, conn);
		}
		
		return result;
	}
	
	public ArrayList<User> getAllUser(){
		ArrayList<User> users = new ArrayList<User>();
		Connection conn = null;
		ResultSet rs = null;
		PreparedStatement st = null;
		
		try {
			conn = connect();
			st = conn.prepareStatement("SELECT UserName, Password, Role, Email, LastModifyTime FROM UserInfo");
			rs = st.executeQuery();
			
			while (rs.next()) {
				User usr = new User(rs.getString("UserName"));				
				usr.setPassword(rs.getString("Password"));
				usr.setEmail(rs.getString("Email"));
				usr.setLastModifyTime(rs.getDate("LastModifyTime"));
				usr.setRole(rs.getInt("Role"));
				users.add(usr);
			}
		} catch (SQLException e) {
			logger.error("db error", e);
		} finally {
			cleanup(rs, st, conn);
		}
		
		return users;
	}

	public boolean verify(String userName, String password) {
		boolean result = false;
		Connection conn = null;
		ResultSet rs = null;
		PreparedStatement st = null;
//		//String sha1Pwd = PwdToSha1(password).toUpperCase();
//		
//		if (null==sha1Pwd){
//			return result;
//		}

		try {
			conn = connect();
			st = conn.prepareStatement("SELECT * from UserInfo where UserName=? and Password=?");
			st.setString(1, userName);
			st.setString(2, password);
			rs = st.executeQuery();
			
			if (rs.next()) {
				result = true;
			}
		} catch (SQLException e) {
			logger.error("db error", e);
		} finally {
			cleanup(rs, st, conn);
		}
		
		return result;
	}

	public static String PwdToSha1(String s){
		byte[] sha1Byte = Sha1Util.calcSha1(s);
		if (null == sha1Byte)
			return null;
		else
			return Hex.encodeHexString(sha1Byte);		
	}	

	public boolean changeEmail(String userName, String email) {
		boolean result = false;
		Connection conn = null;
		PreparedStatement st = null;
		
		try {
			conn = connect();
			st = conn.prepareStatement("UPDATE UserInfo Set Email=? where UserName=?");
			st.setString(1, email);
			st.setString(2, userName);
			int count = st.executeUpdate();
			result = (count>0);
			
		} catch (SQLException e) {
			logger.error("db error", e);
		} finally {
			cleanup(st, conn);
		}
		
		return result;
	}

	@Override
	public boolean addUser(User usr) {
		boolean result = false;
		Connection conn = null;
		ResultSet rs = null;
		PreparedStatement st = null;
		
		if(isUserExist(usr.getName())){
			logger.error("User is existed: " + usr.getName());
			return false;
		}
		
		try {
			conn = connect();
			st = conn.prepareStatement("INSERT INTO UserInfo (UserName, Password, Email, Role, LastModifyTime) VALUES (?, ?, ?, ?, ?, now())");
			st.setString(1, usr.getName());
			st.setString(2,  usr.getPassword());
			st.setString(3, usr.getEmail());
			st.setInt(4, usr.getRole());
			
			result = st.executeUpdate() > 0;			
			
		} catch (SQLException e) {
			logger.error("db error", e);
		} finally {
			cleanup(rs, st, conn);
		}
		
		return result;
	}
	
}
