package com.king.server.task;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.security.NoSuchAlgorithmException;
import java.text.SimpleDateFormat;
import java.util.Date;
import java.util.List;

import org.apache.commons.io.FileUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.json.JSONException;
import org.json.JSONObject;

import com.king.server.Factory;
import com.king.server.PortalConfUtil;
import com.king.server.response.ReportBean;


public class ReportWorker implements Runnable{

	private final Log logger = LogFactory.getLog(getClass());
    private static final int sleepTime = 100;
    
	@Override
	public void run() {
		logger.info("ReportWorker thread started...");
		
		String reportDir = PortalConfUtil.getPortalConf().getProperty("upload.report.dir");
		logger.info("upload.report.dir:" + reportDir);
		String backupdir = PortalConfUtil.getPortalConf().getProperty("upload.report.backupdir");
		logger.info("upload.report.backupdir:" + backupdir);
		
		File reportDirFile = new File(reportDir);
		
		while(true){
			
			try{
				for(File f : reportDirFile.listFiles()){
					
					if(f.isFile()){
						
						if(f.getName().endsWith("_101.rpt")){
							logger.info("Found report file: " + f.getAbsolutePath());
							BufferedReader br = new BufferedReader(new FileReader(f));
							String line;
							
							while((line = br.readLine()) != null){
								logger.info("report: " + line);
								handleReport(line);
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
				
			}
			
			try {
				Thread.sleep(sleepTime);
			} 
			catch (InterruptedException e) {
				e.printStackTrace();
			}
					
		} //while(true)

	} 
	
	/*
	 * {"mac":"aabbcc112233", "uuid":",?=T0_rS>a V:FH=","firechat":"slumdunking","name":"冷雨一刀","user":"slumdunking","msg":"Slumdunking room","t":1.433261092811E9,"st":1433261056}
	 * 
	 */
	
	private void handleReport(String report){
		try {
			JSONObject data = new JSONObject(report);
			
			String room = data.getString("firechat");
			String user = data.getString("user");
			String name = data.getString("name");
			String uuid = data.getString("uuid");
			String msg = data.getString("msg");
			
			Factory.getInstance().getReportDao().addReport(uuid, user, name, room, msg);
			
			ReportBean rpt = new ReportBean();
			rpt.setMsg(msg);
			rpt.setName(name);
			rpt.setUser(user);
			rpt.setRoom(room);
			rpt.setTime(getCurrentTimeStamp());
			
			Factory.getInstance().getReportCache().add(rpt);
			
			
		} catch (JSONException e) {
			logger.error("handleReport->Exception when handle " + report, e);
			e.printStackTrace();
		}
	}
	
	private static String getCurrentTimeStamp() {
	    SimpleDateFormat sdfDate = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
	    Date now = new Date();
	    String strDate = sdfDate.format(now);
	    return strDate;
	}
	

}
