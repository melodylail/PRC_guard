package com.king.server;

import java.io.FileInputStream;
import java.util.Properties;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class PortalConfUtil {
	
	private static final Log logger = LogFactory.getLog(PortalConfUtil.class);
	private static Properties portalConf;
	private static String PORTAL_CONF_PATH = "conf/mserver.conf";
	
	public static Properties getPortalConf() {
		portalConf = loadConf(portalConf, PORTAL_CONF_PATH);
		return portalConf;
	}
	
	private static Properties loadConf(Properties conf, String path) {
		Properties result = conf;
		if(result == null) {
			synchronized(PortalConfUtil.class) {
				try {
					if(result == null) {
						result = new Properties();
						FileInputStream fis = new FileInputStream(path);
						result.load(fis);
					    fis.close();
					}
				} catch(Exception ex) {
					logger.error(ex.getMessage());
				}
			}
		}
		return result;
	}
	
}
