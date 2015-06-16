/*
 ********************************************************************************
 TREND MICRO HIGHLY CONFIDENTIAL INFORMATION:
 THIS SOFTWARE CONTAINS CONFIDENTIAL INFORMATION AND TRADE SECRETS OF TREND MICRO
 INCORPORATED AND MAY BE PROTECTED BY ONE OR MORE PATENTS. USE, DISCLOSURE, OR
 REPRODUCTION OF ANY PORTION OF THIS SOFTWARE IS PROHIBITED WITHOUT THE PRIOR
 EXPRESS WRITTEN PERMISSION OF TREND MICRO INCORPORATED.
 Copyright 2009 Trend Micro Incorporated. All rights reserved as an unpublished
 work.
 ********************************************************************************
 */
package com.king.server;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.springframework.beans.BeansException;
import org.springframework.context.ApplicationContext;
import org.springframework.context.ApplicationContextAware;

import com.king.server.db.UserManager;

public class Factory implements ApplicationContextAware {
	private static final Log log = LogFactory.getLog(Factory.class);
	private static Factory instance = null;

	private static ApplicationContext context;

	private UserManager userManager;

	private StartEntry startEntry;

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
	

	
	public StartEntry getStartEntry() {
		return startEntry;
	}

	public void setStartEntry(StartEntry startEntry) {
		this.startEntry = startEntry;
	}
}
