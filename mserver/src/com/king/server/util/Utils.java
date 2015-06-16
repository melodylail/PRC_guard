package com.king.server.util;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

public class Utils {
	public static boolean isEmpty(String str) {
		return str == null || str.isEmpty();
	}
	
	public static String dateToString(Date date){		
		DateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		return df.format(date);		
	}
}
