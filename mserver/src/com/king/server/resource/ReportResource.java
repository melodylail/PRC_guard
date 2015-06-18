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
	@Path("/query_all.bin")
	@Produces(MediaType.APPLICATION_JSON)
	public Response qeuryReports() {
		try {
			logger.info("Receive qeuryAllReports request");
			
			List<String> reports = getReports();
			ReportsReponse res = new ReportsReponse();
			res.setError_code(ErrorCode.ERR_SUCCESS);
			res.setMessage(ErrorCode.getErrMsg(ErrorCode.ERR_SUCCESS));	
			res.setData(reports);
			
			return Response.ok(res).build();
			
		} catch (Exception e) {
			logger.error("qeuryReports->Error", e);
			e.printStackTrace();
		}
		
		logger.error("qeuryAllReports->404 NOT FOUND");
		
		return Response.status(Status.NOT_FOUND).build();
	}
	
	private List<String> getReports(){
		List<String> reports = new ArrayList<String>();
		
		try{
			String reportDir = PortalConfUtil.getPortalConf().getProperty("upload.report.dir");
			logger.info("upload.report.dir:" + reportDir);
			String backupdir = PortalConfUtil.getPortalConf().getProperty("upload.report.backupdir");
			logger.info("upload.report.backupdir:" + backupdir);		
			
			File reportDirFile = new File(reportDir);
			
			for(File f : reportDirFile.listFiles()){
				if(f.isFile()){
					if(f.getName().endsWith("_101.rpt")){
						logger.info("Found report file: " + f.getAbsolutePath());
						BufferedReader br = new BufferedReader(new FileReader(f));
						String line;
						
						while((line = br.readLine()) != null){
							logger.info("report: " + line);
							reports.add(new String(line));
						}
						
						br.close();
						
						File dstFile = new File(backupdir, f.getName());
						
						try{
							FileUtils.moveFile(f.getAbsoluteFile(), dstFile);
 							logger.info("Success to move " + f.getAbsolutePath() 
 									+ " to " + dstFile.getAbsolutePath());
						}
						catch(Exception e){
 							logger.error("Failed to move " + f.getAbsolutePath() 
 									+ " to " + dstFile.getAbsolutePath(), e);
 							FileUtils.forceDelete(f.getAbsoluteFile());
 							e.printStackTrace();
						}
					}
				}
			}
		}
		catch(Exception e){
			logger.error("getReports catch exception: " + e.getMessage(), e);
		}
		
		return reports;
	}
	
}
