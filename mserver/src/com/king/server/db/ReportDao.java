package com.king.server.db;

import java.util.List;

import com.king.server.response.ReportBean;

public interface ReportDao {
	public boolean addReport(String uuid, 
							String user,
							String name,
							String room,
							String msg);
	
	public List<ReportBean> queryReports();
	public List<ReportBean> queryReports(int pos, int count);
	public int queryReportsCount();
	public int queryReportsCountByUser(String user);
	public List<ReportBean> queryReportsByUser(String user);
	public List<ReportBean> queryReportsByUser(String user, int pos, int count);
}
