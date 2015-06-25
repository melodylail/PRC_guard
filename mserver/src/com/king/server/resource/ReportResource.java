package com.king.server.resource;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.ArrayList;
import java.util.List;

import javax.ws.rs.DefaultValue;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.Consumes;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.io.FileUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;


import org.json.JSONArray;
import org.json.JSONObject;

import com.king.server.Factory;
import com.king.server.PortalConfUtil;
import com.king.server.data.ErrorCode;
import com.king.server.data.User;
import com.king.server.response.ReportBean;
import com.king.server.response.ReportsDataTableReponse;
import com.king.server.response.ReportsReponse;


@Path("/")
public class ReportResource {	
	private final Log logger = LogFactory.getLog(getClass());
	private static int port = 10;
	
	@GET
	@Path("/demo.bin")
	public Response queryDemo() {
		try {
			
			JSONObject jsonData = new JSONObject();
			jsonData.put("datetime", "2015-6-11 18:00:00");
			jsonData.put("org", "ChinaNet Nanjing Network");
			jsonData.put("city", "Shantou, China");
			jsonData.put("md5", "10.0.0.8");
			jsonData.put("city2", "LA, USA");
			jsonData.put("service", "RYNC");
			jsonData.put("dport", port++);
			
			
			logger.info("queryDemo-> " + jsonData.toString());
			return Response.ok(jsonData.toString()).build();
			
		} catch (Exception e) {
			logger.error("queryDemo->Error", e);
			e.printStackTrace();
		}
		
		logger.error("queryDemo->404 NOT FOUND");
		
		return Response.status(Status.NOT_FOUND).build();
	}
	
	@GET
	@Path("/console/report/query_latest.bin")
	@Produces(MediaType.APPLICATION_JSON)
	public Response qeuryLatestReports() {
		try {
			logger.debug("Receive qeuryLatestReports request");
			
			List<ReportBean> reports = Factory.getInstance().getReportCache().get_many();
			ReportsReponse res = new ReportsReponse();
			res.setError_code(ErrorCode.ERR_SUCCESS);
			res.setData(reports);
			
			return Response.ok(res).build();
			
		} catch (Exception e) {
			logger.error("qeuryLatestReports->Error", e);
			e.printStackTrace();
		}
		
		logger.error("qeuryLatestReports->404 NOT FOUND");
		
		return Response.status(Status.NOT_FOUND).build();
	}
	
	@GET
	@Path("/console/report/query_all_count.bin")
	@Produces(MediaType.APPLICATION_JSON)
	public Response qeuryAllReportsCount() {
		try {
			logger.debug("Receive qeuryAllReportsCount request");
			
			int count = Factory.getInstance().getReportDao().queryReportsCount();
			JSONObject jsonData = new JSONObject();
			jsonData.put("error_code", ErrorCode.ERR_SUCCESS);
			jsonData.put("count", count);
			
			return Response.ok(jsonData).build();
			
		} catch (Exception e) {
			logger.error("qeuryLatestReports->Error", e);
			e.printStackTrace();
		}
		
		logger.error("qeuryLatestReports->404 NOT FOUND");
		
		return Response.status(Status.NOT_FOUND).build();
	}
	
	@GET
	@Path("/console/report/query_all.bin")
	@Produces(MediaType.APPLICATION_JSON)
	public Response qeuryAllReports(
			@QueryParam("draw") int draw,
			@QueryParam("start") int start,
			@QueryParam("length") int length
			) {
		try {
			logger.info("Receive qeuryAllReports request, draw=" + draw + ", start=" +start + ", length=" + length);
			
			ReportsDataTableReponse res = new ReportsDataTableReponse();
			int count = Factory.getInstance().getReportDao().queryReportsCount();
			
			List<ReportBean> reports = Factory.getInstance().getReportDao().queryReports(start, length);
			res.setDraw(draw);
			res.setRecordsTotal(count);
			res.setRecordsFiltered(count);
			res.setData(reports);
			
			
			return Response.ok(res).build();
			
		} catch (Exception e) {
			logger.error("qeuryAllReports->Error", e);
			e.printStackTrace();
		}
		
		logger.error("qeuryAllReports->404 NOT FOUND");
		
		return Response.status(Status.NOT_FOUND).build();
	}
	
	@GET
	@Path("/console/report/query_reports.bin")
	@Produces(MediaType.APPLICATION_JSON)
	public Response qeuryReportsByUser(
			@QueryParam("account") String account,
			@QueryParam("draw") int draw,
			@QueryParam("start") int start,
			@QueryParam("length") int length
			) {
		try {
			logger.info("Receive qeuryReportsByUser request, account=" + account + ", draw=" + draw + ", start=" +start + ", length=" + length);
			
			ReportsDataTableReponse res = new ReportsDataTableReponse();
			int count = 0;
			List<ReportBean> reports = new ArrayList<ReportBean>();
			
			if(account == null || account.isEmpty()){
				count = Factory.getInstance().getReportDao().queryReportsCount();
				reports = Factory.getInstance().getReportDao().queryReports(start, length);
			}
			else{
				count = Factory.getInstance().getReportDao().queryReportsCountByUser(account);
				reports = Factory.getInstance().getReportDao().queryReportsByUser(account, start, length);
			}
			res.setDraw(draw);
			res.setRecordsTotal(count);
			res.setRecordsFiltered(count);
			res.setData(reports);
			
			
			return Response.ok(res).build();
			
		} catch (Exception e) {
			logger.error("qeuryReportsByUser->Error", e);
			e.printStackTrace();
		}
		
		logger.error("qeuryReportsByUser->404 NOT FOUND");
		
		return Response.status(Status.NOT_FOUND).build();
	}
	
	
	
}
