package com.king.server.response;

import java.util.List;

public class UsersReponse {
	private int error_code;
	private String message;
	List<UserBean> data;
	
	public int getError_code() {
		return error_code;
	}
	
	public String getMessage() {
		return message;
	}
	
	public List<UserBean> getData() {
		return data;
	}
	
	public void setError_code(int error_code) {
		this.error_code = error_code;
	}
	
	public void setMessage(String message) {
		this.message = message;
	}
	
	public void setData(List<UserBean> data) {
		this.data = data;
	}
}
