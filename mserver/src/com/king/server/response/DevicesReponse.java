package com.king.server.response;

import java.util.List;

import com.king.server.bean.DeviceInfo;
import com.king.server.bean.DeviceStatus;

public class DevicesReponse {
	private int error_code;
	List<DeviceInfo> data;
	
	public int getError_code() {
		return error_code;
	}
	public void setError_code(int error_code) {
		this.error_code = error_code;
	}
	public List<DeviceInfo> getData() {
		return data;
	}
	public void setData(List<DeviceInfo> data) {
		this.data = data;
	}
	
	
}
