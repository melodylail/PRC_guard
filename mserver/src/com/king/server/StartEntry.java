package com.king.server;

import java.util.Properties;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.king.server.task.ReportWorker;

public class StartEntry {
	private static Log logger = LogFactory.getLog(StartEntry.class);
	
	public StartEntry() {
		loadConf();
		
		Thread reportWorkerThread = new Thread(new ReportWorker());
		reportWorkerThread.start();
	}
	
	private void loadConf() {
		Properties portalConf = PortalConfUtil.getPortalConf();		
	}

}
