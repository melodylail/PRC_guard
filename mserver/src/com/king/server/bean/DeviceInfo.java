package com.king.server.bean;

import java.util.Date;

public class DeviceInfo {
	private String id;
	private String name;
	private String type;
	private int status;
	private String channel;
	private String netinferface;
	private String lastlivetime;
	
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

	public String getLastlivetime() {
		return lastlivetime;
	}

	public void setLastlivetime(String lastlivetime) {
		this.lastlivetime = lastlivetime;
	}

	@Override
	public String toString() {
		return "DeviceInfo [id=" + id + ", name=" + name + ", type=" + type
				+ ", status=" + status + ", channel=" + channel
				+ ", netinferface=" + netinferface + ", lastlivetime="
				+ lastlivetime + "]";
	}

}
