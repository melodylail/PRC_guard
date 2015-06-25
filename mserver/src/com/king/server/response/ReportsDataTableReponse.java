package com.king.server.response;

import java.util.List;

public class ReportsDataTableReponse {
	private int draw;
	private int recordsTotal;
	private int recordsFiltered;
	List<ReportBean> data;
	
	public int getDraw() {
		return draw;
	}
	
	public void setDraw(int draw) {
		this.draw = draw;
	}
	
	public int getRecordsTotal() {
		return recordsTotal;
	}
	
	public void setRecordsTotal(int recordsTotal) {
		this.recordsTotal = recordsTotal;
	}
	
	public int getRecordsFiltered() {
		return recordsFiltered;
	}
	
	public void setRecordsFiltered(int recordsFiltered) {
		this.recordsFiltered = recordsFiltered;
	}
	
	public List<ReportBean> getData() {
		return data;
	}
	
	public void setData(List<ReportBean> data) {
		this.data = data;
	}
	
	
}
