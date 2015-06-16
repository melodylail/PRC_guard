package com.king.server.util;

import java.io.UnsupportedEncodingException;

public class XorMethod {	
	public static byte[] calcXor(String msg,String key){
		try {
			return calcXor(msg.getBytes("US-ASCII"),key.getBytes("US-ASCII"));
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
			return null;
		}
	}
	
	public static byte[] calcXor(byte[] msg, String key){
		try {
			return calcXor(msg, key.getBytes("US-ASCII"));
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
			return null;
		}
	}
	
	public static byte[] calcXor(byte[] msg, byte[] key){
		if (msg == null || key == null || msg.length == 0 || key.length == 0){
			return null;
		}
		int keyIndex = 0;
		byte[] output = new byte[msg.length];
		for (int i=0; i<msg.length; i++, keyIndex++){
			if (keyIndex >= key.length){
				keyIndex = 0;
			}
			output[i] = (byte) (key[keyIndex] ^ msg[i]);
		}
		return output;
	}
}
