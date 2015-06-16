package com.king.server.bean;

public class ReportBean {
	String user_name;
	String content;
	String picture_url;
	String voice_url;
	String time;
	
	public String getUser_name() {
		return user_name;
	}
	
	public void setUser_name(String user_name) {
		this.user_name = user_name;
	}
	
	public String getContent() {
		return content;
	}
	
	public void setContent(String content) {
		this.content = content;
	}
	
	public String getPicture_url() {
		return picture_url;
	}
	
	public void setPicture_url(String picture_url) {
		this.picture_url = picture_url;
	}
	
	public String getTime() {
		return time;
	}
	
	public void setTime(String time) {
		this.time = time;
	}

	public String getVoice_url() {
		return voice_url;
	}

	public void setVoice_url(String voice_url) {
		this.voice_url = voice_url;
	}

}
