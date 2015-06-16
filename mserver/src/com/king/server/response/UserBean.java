package com.king.server.response;

public class UserBean {
	private String user_name;
	private String email;
	private String password;
	private String last_modified_time;
	private int role;
	
	public String getUser_name() {
		return user_name;
	}
	
	public String getEmail() {
		return email;
	}
	
	public String getLast_modified_time() {
		return last_modified_time;
	}
	
	public int getRole() {
		return role;
	}
	
	public void setUser_name(String user_name) {
		this.user_name = user_name;
	}
	
	public void setEmail(String email) {
		this.email = email;
	}
	
	public void setLast_modified_time(String last_modified_time) {
		this.last_modified_time = last_modified_time;
	}
	
	public void setRole(int role) {
		this.role = role;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

}
