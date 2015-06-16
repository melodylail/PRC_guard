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

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

/**
 * An Exception class for customized message.
 * @author mike
 *
 */
public class RestException extends WebApplicationException {
	private static final long serialVersionUID = -8475094565916175367L;

	// below status code from RFC 2616 
	public static final int REQUEST_TOO_LARGE = 413;
	
	/**
	 * Constructor.
	 * @param statusCode the status code
	 * @param message the error message
	 */
	public RestException(int statusCode, String message) {
		this(Response.status(statusCode), message);
    }
	
	/**
	 * Constructor.
	 * @param status the status object
	 * @param message the error message
	 */
	public RestException(Status status, String message) {
    	this(Response.status(status), message);
    }
	
	public RestException(String userName, byte[] sha1, int fileType, Status status, int errorCode, String message) {
    	this(Response.status(status), message);	
    }
	
	private RestException(Response.ResponseBuilder builder, String message) {
		super(builder.entity(message)
    			.type("text/plain").build());
	}
	
	
}
