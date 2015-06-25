package com.king.server.util;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

public class Utils {
	public static boolean isEmpty(String str) {
		return str == null || str.isEmpty();
	}
	
	public static String dateToString(Date date){		
		DateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		return df.format (date);		
	}
	
	public static String md5(String s){
		MessageDigest md5;
		try {
			md5 = MessageDigest.getInstance("MD5");
			return HexUtil.byteArraytoHexString(md5.digest(s.getBytes()));
		} catch (NoSuchAlgorithmException e) {
			return null;
		}
	}
}
