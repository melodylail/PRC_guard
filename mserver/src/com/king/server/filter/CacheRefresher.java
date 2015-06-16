package com.king.server.filter;

import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.king.server.PortalConfUtil;

public class CacheRefresher {
	private final Log logger = LogFactory.getLog(getClass());
	private static int DEFAULT_RELOAD_PERIOD = 20 * 60 * 1000; // 20
	public static int cache_reload_period = DEFAULT_RELOAD_PERIOD;

	private static CacheRefresher instance = new CacheRefresher();

	public static CacheRefresher getInstance() {
		return instance;
	}

	private Timer timer = null;

	private CacheRefresher() {
		timer = new Timer("CacheRefresher", true);
		try {
			cache_reload_period = Integer.parseInt(PortalConfUtil.getPortalConf()
					.getProperty("cache_timeout")) * 60 * 1000;
			logger.debug("cache_reload_period: " + cache_reload_period);
		} catch (Exception e) {
			logger.error("parase cache timeout error", e);
			cache_reload_period = DEFAULT_RELOAD_PERIOD;
		}
	}

	public void schedule(TimerTask task, long delay, long period) {
		this.timer.schedule(task, delay, period);
	}

	public void scheduleAtFixedRate(TimerTask task, Date firstTime, long period) {
		this.timer.scheduleAtFixedRate(task, firstTime, period);
	}

	public void schedule(TimerTask task, Date firstTime, long period) {
		this.timer.schedule(task, firstTime, period);
	}

	public void schedule(TimerTask task, long delay) {
		this.timer.schedule(task, delay);
	}
}
