package com.king.server;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.BeansException;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;

import com.king.server.db.DeviceDao;
import com.king.server.db.ReportDao;
import com.king.server.db.UserManager;
import com.king.server.task.ReportCache;
import com.king.server.task.ReportWorker;

public class Factory implements ApplicationContextAware {
	private static final Log log = LogFactory.getLog(Factory.class);
	private static Factory instance = null;

	private static ApplicationContext context;

	private UserManager userManager;
	private StartEntry startEntry;
	private ReportDao reportDao;
	private DeviceDao deviceDao;
	private ReportWorker reportWorker;
	private ReportCache reportCache;

	/**
	 * constructor.
	 */
	public Factory() {
		if (instance != null) {
			throw new RuntimeException("the Factory must be singleton!");
		}
		log.info("new Factory");

		instance = this;
	}

	public static Factory getInstance() {
		return instance;
	}

	public Object getBean(String name) {
		return instance.getContext().getBean(name);
	}

	public ApplicationContext getContext() {
		return context;
	}

	// public static void initConfiguration() {
	// context = new FileSystemXmlApplicationContext(
	// new String[] { "classpath:commonContext.xml", });
	// }

	@Override
	public void setApplicationContext(ApplicationContext applicationContext)
			throws BeansException {
		context = applicationContext;
	}

	public void setUserManager(UserManager userManager) {
		this.userManager = userManager;
	}

	public UserManager getUserManager() {
		return userManager;
	}
	
	public ReportDao getReportDao() {
		return reportDao;
	}

	public void setReportDao(ReportDao reportDao) {
		this.reportDao = reportDao;
	}

	public DeviceDao getDeviceDao() {
		return deviceDao;
	}

	public void setDeviceDao(DeviceDao deviceDao) {
		this.deviceDao = deviceDao;
	}

	public ReportWorker getReportWorker() {
		return reportWorker;
	}

	public void setReportWorker(ReportWorker reportWorker) {
		this.reportWorker = reportWorker;
	}

	public ReportCache getReportCache() {
		return reportCache;
	}

	public void setReportCache(ReportCache reportCache) {
		this.reportCache = reportCache;
	}

	public StartEntry getStartEntry() {
		return startEntry;
	}

	public void setStartEntry(StartEntry startEntry) {
		this.startEntry = startEntry;
	}
}
