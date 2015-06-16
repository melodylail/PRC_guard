package com.king.server.data;

import java.util.Date;

import org.json.JSONException;
import org.json.JSONObject;

import com.king.server.Factory;
import com.king.server.util.Utils;


public class User {
	private String name;
	private String password;
	private String email;
	private Date lastModifyTime;
	private int role;
	
	private long load_time = 0;
	private static final long reloadPeriod = 20 * 60 * 1000;//20min

	public boolean isValidate() {
		if (System.currentTimeMillis() - this.load_time >= reloadPeriod) {
			return false;
		}
		return true;
	}
	
	public User(String name){
		this.name  = name;
		this.load_time = System.currentTimeMillis();
	}
	
	public String getName() {
		return name;
	}
	
	public String getPassword() {
		return password;
	}
	
	public String getEmail() {
		return email;
	}
	
	public int getRole() {
		return role;
	}
	public void setName(String name) {
		this.name = name;
	}
	
	public void setPassword(String password) {
		this.password = password;
	}
	
	public void setEmail(String email) {
		this.email = email;
	}
	
	public void setRole(int role) {
		this.role = role;
	}
	
	public Date getLastModifyTime() {
		return lastModifyTime;
	}

	public void setLastModifyTime(Date lastModifyTime) {
		this.lastModifyTime = lastModifyTime;
	}
	
	public String toJsonString(){
		JSONObject json = new JSONObject();

		try {
			json.put("user_name", name);
			json.put("email", email);
			json.put("role", role);
			json.put("last_modified_time", Utils.dateToString(lastModifyTime));
			return json.toString();
		} catch (JSONException e) {			
			e.printStackTrace();
		}
		
		return "";
	}
	
	public JSONObject toJsonObj(){
		JSONObject json = new JSONObject();

		try {
			json.put("user_name", name);
			json.put("email", email);
			json.put("role", role);
			json.put("last_modified_time", Utils.dateToString(lastModifyTime));
			return json;
		} catch (JSONException e) {			
			e.printStackTrace();
		}
		
		return null;
	}

}
