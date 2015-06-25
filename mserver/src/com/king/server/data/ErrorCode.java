package com.king.server.data;

import javax.ws.rs.core.Response;

import com.king.server.response.CommonResponse;

public class ErrorCode {
	public static final int ERR_SUCCESS              = 1;
	public static final int ERR_FAILED               = 2;
	
	public static final int ERR_INVALID_PARAMS       = 100;
	
	public static final int ERR_USR_NO_USER          = 200;
	public static final int ERR_USR_AUTH_FAILED      = 201;
	public static final int ERR_USR_EXSITED          = 204;
	
	
	public static String getErrMsg(int error) {
		String msg = "Unknown error";
		
		switch (error) {
		case ERR_SUCCESS:
			msg = "Success";
			break;
		case ERR_FAILED:
			msg = "Error";
			break;
		case ERR_INVALID_PARAMS:
			msg = "Invalid params";
			break;
		case ERR_USR_NO_USER:
			msg = "Not find the user.";
			break;
		case ERR_USR_AUTH_FAILED:
			msg = "Failed to authentication";
			break;
		case ERR_USR_EXSITED:
			msg = "User existed";
			break;
		default:
			break;
		}
		
		return msg;
	}
	
	public static Response genErrResponse(int error_code){
		CommonResponse res = new CommonResponse();
		res.setError_code(error_code);
		res.setMessage(getErrMsg(error_code));
		
		return Response.ok(res).build();
	}
	

}
