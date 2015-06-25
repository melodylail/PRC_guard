package com.king.server.bean;

import java.util.Date;

public class DeviceStatus {
	private String id;
	private String name;
	private String type;
	private int status;
	private String channel;
	private String netinferface;
	
	public String getId() {
		return id;
	}
	
	public void setId(String id) {
		this.id = id;
	}
	
	public String getName() {
		return name;
	}
	
	public void setName(String name) {
		this.name = name;
	}
	
	public String getType() {
		return type;
	}
	public void setType(String type) {
		this.type = type;
	}
	public int getStatus() {
		return status;
	}
	public void setStatus(int status) {
		this.status = status;
	}
	public String getChannel() {
		return channel;
	}
	public void setChannel(String channel) {
		this.channel = channel;
	}

	public String getNetinferface() {
		return netinferface;
	}

	public void setNetinferface(String netinferface) {
		this.netinferface = netinferface;
	}

	@Override
	public String toString() {
		return "DeviceStatus [id=" + id + ", name=" + name + ", type=" + type
				+ ", status=" + status + ", channel=" + channel + "]";
	}
	
	
	
	
	

}
