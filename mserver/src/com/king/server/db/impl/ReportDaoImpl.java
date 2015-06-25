package com.king.server.db.impl;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import javax.sql.DataSource;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.king.server.db.ReportDao;
import com.king.server.response.ReportBean;
import com.king.server.util.HexUtil;
import com.king.server.util.Utils;


public class ReportDaoImpl implements ReportDao{

	private DataSource ds = null;
	private final static  Log logger = LogFactory.getLog(ReportDaoImpl.class);
	
	public static String uploadApkDir = null;
	
	private Connection connect() throws SQLException {
		Connection conn = ds.getConnection();
		return conn;
	}
	
	public ReportDaoImpl(DataSource ds){
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
	
	public int queryReportsCount(){
		Connection conn = null;
		ResultSet rs = null;
		PreparedStatement st = null;
		String query = "SELECT COUNT(1) FROM Report";
		int count = 0;
		
		try{
			conn = connect();
			st = conn.prepareStatement(query);
			rs = st.executeQuery();
			
			if(rs.next()){
				count = rs.getInt(1);
			}
		}catch (SQLException e) {
			logger.error("queryReportsCount error", e);
		} finally {
			cleanup(rs, st, conn);
		}
		
		return count;
	}
	
	@Override
	public List<ReportBean> queryReports(){
		Connection conn = null;
		ResultSet rs = null;
		PreparedStatement st = null;
		List<ReportBean> reports = new ArrayList<ReportBean>();
		String query = "SELECT User, Name, Room, Msg, Time FROM Report ORDER BY Time DESC LIMIT 100";
		
		try{
			conn = connect();
			st = conn.prepareStatement(query);
			rs = st.executeQuery();
			
			while(rs.next()){
				ReportBean rep = new ReportBean();
				rep.setUser(rs.getString("User"));
				rep.setName(rs.getString("Name"));
				rep.setRoom(rs.getString("Room"));
				rep.setMsg(rs.getString("Msg"));
				rep.setTime(Utils.dateToString(rs.getDate("Time")));
				
				reports.add(rep);
			}
		}catch (SQLException e) {
			logger.error("queryReports error", e);
		} finally {
			cleanup(rs, st, conn);
		}
		return reports;
	}
	
	@Override
	public List<ReportBean> queryReports(int pos, int count){
		Connection conn = null;
		ResultSet rs = null;
		PreparedStatement st = null;
		List<ReportBean> reports = new ArrayList<ReportBean>();
		String query = String.format("SELECT User, Name, Room, Msg, Time FROM Report ORDER BY Time DESC LIMIT %d,%d"
				                     , pos, count);
		
		logger.debug(query);
		
		try{
			conn = connect();
			st = conn.prepareStatement(query);
			rs = st.executeQuery();
			
			while(rs.next()){
				ReportBean rep = new ReportBean();
				rep.setUser(rs.getString("User"));
				rep.setName(rs.getString("Name"));
				rep.setRoom(rs.getString("Room"));
				rep.setMsg(rs.getString("Msg"));
				rep.setTime(Utils.dateToString(rs.getDate("Time")));
				
				reports.add(rep);
			}
		}catch (SQLException e) {
			logger.error("queryReports error", e);
		} finally {
			cleanup(rs, st, conn);
		}
		return reports;
	}
	
	@Override
	public boolean addReport(String uuid,
							String user,
							String name,
							String room,
							String msg){
		Connection conn = null;
		ResultSet rs = null;
		PreparedStatement st = null;
		
		String sql = "Insert IGNORE INTO Report (Uuid, User, Name, Room, Msg, Time) VALUES(?,?,?,?,?,now())";
		
		logger.debug("addReport-> " + sql);
		
		try{
			conn = connect();
			st = conn.prepareStatement(sql);
			st.setBytes(1, HexUtil.hexStringToByteArray(uuid));
			st.setString(2, user);
			st.setString(3, name);
			st.setString(4, room);
			st.setString(5, msg);
			
			if(0 != st.executeUpdate()){
				logger.info("addReport Success"); 
				return true;
			}
		}catch (SQLException e) {
			logger.error("addReport error", e);
		} finally {
			cleanup(rs, st, conn);
		}
		
		return false;
	}
	
	public int queryReportsCountByUser(String user){
		Connection conn = null;
		ResultSet rs = null;
		PreparedStatement st = null;
		String query = "SELECT COUNT(1) FROM Report WHERE User=?";
		int count = 0;
		
		try{
			conn = connect();
			st = conn.prepareStatement(query);
			st.setString(1, user);
			rs = st.executeQuery();
			
			if(rs.next()){
				count = rs.getInt(1);
			}
		}catch (SQLException e) {
			logger.error("queryReportsCountByUser error", e);
		} finally {
			cleanup(rs, st, conn);
		}
		
		return count;
	}
	
	@Override
	public List<ReportBean> queryReportsByUser(String user){
		Connection conn = null;
		ResultSet rs = null;
		PreparedStatement st = null;
		List<ReportBean> reports = new ArrayList<ReportBean>();
		String query = "SELECT User, Name, Room, Msg, Time FROM Report WHERE User=? ORDER BY Time DESC  LIMIT 100";
		
		try{
			conn = connect();
			st = conn.prepareStatement(query);
			st.setString(1, user);
			rs = st.executeQuery();
			
			while(rs.next()){
				ReportBean rep = new ReportBean();
				rep.setUser(rs.getString("User"));
				rep.setName(rs.getString("Name"));
				rep.setRoom(rs.getString("Room"));
				rep.setMsg(rs.getString("Msg"));
				rep.setTime(Utils.dateToString(rs.getDate("Time")));
				
				reports.add(rep);
			}
		}catch (SQLException e) {
			logger.error("queryReports error", e);
		} finally {
			cleanup(rs, st, conn);
		}
		return reports;
	}
	
	@Override
	public List<ReportBean> queryReportsByUser(String user, int pos, int count){
		Connection conn = null;
		ResultSet rs = null;
		PreparedStatement st = null;
		List<ReportBean> reports = new ArrayList<ReportBean>();
		String query = "SELECT User, Name, Room, Msg, Time FROM Report WHERE User=? ORDER BY Time DESC  LIMIT ?,?";
		logger.debug(query);
		
		try{
			conn = connect();
			st = conn.prepareStatement(query);
			st.setString(1, user);
			st.setInt(2, pos);
			st.setInt(3, count);
			rs = st.executeQuery();
			
			while(rs.next()){
				ReportBean rep = new ReportBean();
				rep.setUser(rs.getString("User"));
				rep.setName(rs.getString("Name"));
				rep.setRoom(rs.getString("Room"));
				rep.setMsg(rs.getString("Msg"));
				rep.setTime(Utils.dateToString(rs.getDate("Time")));
				
				reports.add(rep);
			}
		}catch (SQLException e) {
			logger.error("queryReports error", e);
		} finally {
			cleanup(rs, st, conn);
		}
		return reports;
	}

}
