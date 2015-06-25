package com.king.server.db.impl;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.List;

import javax.sql.DataSource;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.king.server.bean.DeviceInfo;
import com.king.server.bean.DeviceStatus;
import com.king.server.db.DeviceDao;
import com.king.server.db.ReportDao;
import com.king.server.response.ReportBean;
import com.king.server.util.HexUtil;
import com.king.server.util.Utils;


public class DeviceDaoImpl implements DeviceDao{

	private DataSource ds = null;
	private final static  Log logger = LogFactory.getLog(DeviceDaoImpl.class);
	
	public static String uploadApkDir = null;
	
	private Connection connect() throws SQLException {
		Connection conn = ds.getConnection();
		return conn;
	}
	
	public DeviceDaoImpl(DataSource ds){
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
	
	@Override
	public boolean updateDevice(DeviceStatus device_status){
		Connection conn = null;
		ResultSet rs = null;
		PreparedStatement st = null;
		
		String sql = "Insert INTO Device (Id, Name, Type, Status, Channel,NetInterface,LastLiveTime) VALUES(?,?,?,?,?,?,now()) ON DUPLICATE KEY UPDATE Status=?, LastLiveTime=now()";
		
		logger.debug("updateDevice-> " + sql);
		
		try{
			conn = connect();
			st = conn.prepareStatement(sql);
			st.setString(1, device_status.getId());
			st.setString(2, device_status.getName());
			st.setString(3, device_status.getType());
			st.setInt(4, device_status.getStatus());
			st.setString(5, device_status.getChannel());
			st.setString(6, device_status.getNetinferface());
			
			if(0 != st.executeUpdate()){
				logger.info("updateDevice Success"); 
				return true;
			}
		}catch (SQLException e) {
			logger.error("updateDevice error", e);
		} finally {
			cleanup(rs, st, conn);
		}
		
		return false;
	}

	
	@Override
	public List<DeviceInfo> queryDevices(){
		Connection conn = null;
		ResultSet rs = null;
		PreparedStatement st = null;
		List<DeviceInfo> devices = new ArrayList<DeviceInfo>();
		String query = "SELECT Id, Name, Type, Status, Channel, NetInterface, LastLiveTime FROM Device";
		logger.debug(query);
		
		try{
			conn = connect();
			st = conn.prepareStatement(query);
			rs = st.executeQuery();
			
			while(rs.next()){
				DeviceInfo device = new DeviceInfo();
				device.setId(rs.getString("Id"));
				device.setName(rs.getString("Name"));
				device.setType(rs.getString("Type"));
				device.setStatus(rs.getInt("Status"));
				device.setChannel(rs.getString("Channel"));
				device.setNetinferface(rs.getString("NetInterface"));
				Timestamp livetime = rs.getTimestamp("LastLiveTime");
				device.setLastlivetime(livetime.toString());
				
				devices.add(device);
			}
		}catch (SQLException e) {
			logger.error("queryDevices error", e);
		} finally {
			cleanup(rs, st, conn);
		}
		return devices;
	}
	
}
