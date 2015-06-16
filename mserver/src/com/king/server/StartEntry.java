package com.king.server;

import java.util.Properties;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class StartEntry {
	private static Log logger = LogFactory.getLog(StartEntry.class);
	
	public StartEntry() {
		loadConf();
	}
	
	private void loadConf() {
		Properties portalConf = PortalConfUtil.getPortalConf();		
	}

}
