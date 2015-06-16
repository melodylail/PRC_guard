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
package com.king.server.util;

import java.io.UnsupportedEncodingException;
import java.nio.ByteBuffer;
import java.security.SignatureException;
import javax.crypto.Mac;
import javax.crypto.spec.SecretKeySpec;
import org.apache.commons.codec.binary.Base64;

/**
 * This is an utility class for generating RFC 2104-compliant HMAC signature.
 * @author herry_tang
 * 
 */
public class HMACSignatureGen {
	public static final String HMAC_SHA1_ALGORITHM = "HmacSHA1";
	
	/**
	 * Computes RFC 2104-compliant HMAC signature.
	 * 
	 * @param data
	 *            The data to be signed.
	 * @param key
	 *            The signing key.
	 * @return The base64-encoded RFC 2104-compliant HMAC signature.
	 * @throws java.security.SignatureException
	 *             when signature generation fails
	 */
	public static String calculateRFC2104HMAC(byte[] data, String key)
			throws java.security.SignatureException {
		String result;

		try {
			// get an hmac_sha1 key from the raw key bytes
			SecretKeySpec signingKey = new SecretKeySpec(key.getBytes(),
					HMAC_SHA1_ALGORITHM);
			// get an hmac_sha1 Mac instance
			// and initialize with the signing key
			Mac mac = Mac.getInstance(HMAC_SHA1_ALGORITHM);
			mac.init(signingKey);
			// compute the hmac on input data bytes
			byte[] rawHmac = mac.doFinal(data);
			// base64-encode the hmac
			// byte[] resultBytes = (new BASE64Encoder()).encode(rawHmac);
			result = encodeBase64(rawHmac);
		} catch (Exception e) {
			throw new SignatureException("Failed to generate HMAC : "
					+ e.getMessage());
		}
		return result;
	}
	
	private static String encodeBase64(byte[] source){
		return new Base64(Integer.MAX_VALUE,null).encodeToString(source);
	}
	/**
	 * 
	 * @param data
	 *            The string data to be signed.
	 * @param key
	 *           The signing key.
	 * @return
	 * @throws java.security.SignatureException
	 * @throws UnsupportedEncodingException 
	 */
	public static String calculateRFC2104HMAC(String data, String key)
			throws java.security.SignatureException, UnsupportedEncodingException {
		return calculateRFC2104HMAC(data.getBytes("UTF-8"), key);
	}
	/**
	 * 
	 * @param url 
	 *           original request URL, dose not include user's name and security code 
	 * @param userName 
	 * @param request request data
	 * @return
	 * @throws UnsupportedEncodingException 
	 */
	public static byte[] combinate(String url, String userName, byte[] request) throws UnsupportedEncodingException {
		int len1, len2, len3;
		len1 = (url == null) ? 0 : url.getBytes("UTF-8").length;
		len2 = (userName == null) ? 0 : userName.getBytes("UTF-8").length;
		len3 = (request == null) ? 0 : request.length;

		int total = len1 + len2 + len3;
		ByteBuffer bb = ByteBuffer.allocate(total);
		if (url != null) {
			bb.put(url.getBytes("UTF-8"));
		}
		if (userName != null) {
			bb.put(userName.getBytes("UTF-8"));
		}
		if (request != null) {
			bb.put(request);
		}
		return bb.array();
	}
}
