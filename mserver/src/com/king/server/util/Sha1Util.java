package com.king.server.util;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class Sha1Util {
	public static byte[] calcSha1(String s){
		MessageDigest sha1;
		try {
			sha1 = MessageDigest.getInstance("SHA1");
			return sha1.digest(s.getBytes());
		} catch (NoSuchAlgorithmException e) {
			return null;
		}
	}
}
