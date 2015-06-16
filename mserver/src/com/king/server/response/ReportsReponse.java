package com.king.server.response;

import java.util.List;

import com.king.server.bean.ReportBean;

public class ReportsReponse {
	private int error_code;
	private String message;
	List<String> data;
	
	public int getError_code() {
		return error_code;
	}
	
	public String getMessage() {
		return message;
	}
	
	public List<String> getData() {
		return data;
	}
	
	public void setError_code(int error_code) {
		this.error_code = error_code;
	}
	
	public void setMessage(String message) {
		this.message = message;
	}
	
	public void setData(List<String> data) {
		this.data = data;
	}
}
