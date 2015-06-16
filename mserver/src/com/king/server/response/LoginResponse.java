package com.king.server.response;

import com.king.server.data.ErrorCode;

public class LoginResponse {
	private int error_code;
	private String message;
	LoginResBean data;
	
	public LoginResponse(){
		data = null;
		error_code = ErrorCode.ERR_SUCCESS;
		message = ErrorCode.getErrMsg(ErrorCode.ERR_SUCCESS);
	}
	
	public int getError_code() {
		return error_code;
	}
	
	public String getMessage() {
		return message;
	}
	
	public LoginResBean getData() {
		return data;
	}
	
	public void setError_code(int error_code) {
		this.error_code = error_code;
	}
	
	public void setMessage(String message) {
		this.message = message;
	}
	
	public void setData(LoginResBean data) {
		this.data = data;
	}
}
