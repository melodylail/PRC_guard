package com.king.server.util;

import java.util.Locale;

import org.apache.commons.codec.binary.Hex;

public class HexUtil {
	public static byte[] hexStringToByteArray(String s) {
		if(s == null) {
			return null;
		}
		try {
			int len = s.length();
			if (len % 2 == 0) {
				byte[] data = new byte[len / 2];
				for (int i = 0; i < len; i += 2) {
					data[i / 2] = (byte) ((Character.digit(s.charAt(i), 16) << 4) + Character
							.digit(s.charAt(i + 1), 16));
				}
				return data;
			} else {
				return null;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return null;
	}

	public static boolean isHexString(String s) {
		int length = s.length();
		if (length == 0 || length % 2 == 1)
			return false;
		s = s.toLowerCase();
		for (int i = 0; i < length; ++i) {
			char c = s.charAt(i);
			if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')) {
				continue;
			} else {
				return false;
			}
		}

		return true;
	}
	
	public static boolean validSha1(String s){
		if(40!=s.length()){
			return false;
		}
		return isHexString(s);
	}

	public static String byteArraytoHexString(byte[] data) {
		return Hex.encodeHexString(data).toUpperCase(Locale.ENGLISH);
	}

	public static void main(String[] args) {
		byte[] data = { (byte) 0xF4, 0x38 };
		// System.out.println(byteArraytoHexString(data));
		System.out.println(Hex.encodeHexString(data));
	}
}
