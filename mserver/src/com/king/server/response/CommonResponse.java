package com.king.server.response;

public class CommonResponse {
	private int error_code;
	private String message;
	
	public int getError_code() {
		return error_code;
	}
	
	public String getMessage() {
		return message;
	}
	
	public void setError_code(int error_code) {
		this.error_code = error_code;
	}
	
	public void setMessage(String message) {
		this.message = message;
	}
}
