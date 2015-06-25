package com.king.server.task;

import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.king.server.response.ReportBean;

public class ReportCache {
	private final Log logger = LogFactory.getLog(getClass());
    private static final int CACHE_MAX_COUNT = 100;
    
    private Queue<ReportBean> reports_queue = new LinkedList<ReportBean>();
    
    public void add(ReportBean data){
    	if(reports_queue.size() > CACHE_MAX_COUNT){
    		reports_queue.poll();
    	}
    	
    	reports_queue.offer(data);
    }
    
    public ReportBean get(){
    	if(reports_queue.size() > 0){
    		return reports_queue.poll();
    	}
    	
    	return null;
    }
    
    public List<ReportBean> get_many(){
    	int count = 0;
    	List<ReportBean> data = new ArrayList<ReportBean>();
    	
    	while(reports_queue.size() > 0 && count < 10){
    		data.add(reports_queue.poll());
    		count++;
    	}
    	
    	return data;
    }

}
