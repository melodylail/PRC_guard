package com.king.server.db;

import java.util.List;

import com.king.server.bean.DeviceInfo;
import com.king.server.bean.DeviceStatus;
import com.king.server.response.ReportBean;

public interface DeviceDao {
	public boolean updateDevice(DeviceStatus device_status);
	public List<DeviceInfo> queryDevices();
}
