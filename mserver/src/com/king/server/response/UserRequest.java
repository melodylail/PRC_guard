package com.king.server.response;

public class UserRequest {
	private String user_name;
	private String email;
	private String password;
	private int role;
	
	public String getUser_name() {
		return user_name;
	}
	
	public String getEmail() {
		return email;
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
	
	public void setRole(int role) {
		this.role = role;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	@Override
	public String toString() {
		return "UserRequest [user_name=" + user_name + ", email=" + email
				+ ", password=" + password + ", role=" + role + "]";
	}
	
	

}
