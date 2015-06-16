package com.king.server.resource;

import java.util.ArrayList;
import java.util.List;

import javax.ws.rs.DefaultValue;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.Consumes;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;


import org.json.JSONArray;
import org.json.JSONObject;

import com.king.server.Factory;
import com.king.server.bean.LoginRequest;
import com.king.server.data.ErrorCode;
import com.king.server.data.User;
import com.king.server.response.*;
import com.king.server.util.Utils;

@Path("console/user")
public class UserResource {	
	private final Log logger = LogFactory.getLog(getClass());
	
	@GET
	@Path("list_all.json")
	public Response queryAllUser() {
		try {
			List<User> users = Factory.getInstance().getUserManager().getAllUser();
			JSONArray jsonUsers = new JSONArray();
			
			for (User usr : users){
				jsonUsers.put(usr.toJsonObj());
			}
			
			JSONObject jsonData = new JSONObject();
			jsonData.put("count", users.size());
			jsonData.put("users", jsonUsers);
			
			JSONObject json = new JSONObject();
			json.put("error_code", ErrorCode.ERR_SUCCESS);
			json.put("message", ErrorCode.getErrMsg(ErrorCode.ERR_SUCCESS));
			json.put("data", jsonData);
			
			logger.info("queryAllUser-> " + json.toString());
			return Response.ok(json.toString()).build();
			
		} catch (Exception e) {
			logger.error("queryAllUser->Error", e);
			e.printStackTrace();
		}
		
		logger.error("queryAllUser->404 NOT FOUND");
		
		return Response.status(Status.NOT_FOUND).build();
	}
	
	@GET
	@Path("show_all")
	@Produces(MediaType.APPLICATION_JSON)
	public Response listAllUser() {
		try {
			List<User> users = Factory.getInstance().getUserManager().getAllUser();
			UsersReponse res = new UsersReponse();
			res.setError_code(ErrorCode.ERR_SUCCESS);
			res.setMessage(ErrorCode.getErrMsg(ErrorCode.ERR_SUCCESS));
			
			ArrayList<UserBean> usrBeans = new ArrayList<UserBean>();			
			
			for (User usr : users){
				UserBean usrBean = new UserBean();
				usrBean.setUser_name(usr.getName());
				usrBean.setPassword(usr.getPassword());
				usrBean.setEmail(usr.getEmail());
				usrBean.setRole(usr.getRole());
				usrBean.setLast_modified_time(Utils.dateToString(usr.getLastModifyTime()));
				usrBeans.add(usrBean);
			}
			
			res.setData(usrBeans);
			
			return Response.ok(res).build();
			
		} catch (Exception e) {
			logger.error("queryAllUser->Error", e);
			e.printStackTrace();
		}
		
		logger.error("queryAllUser->404 NOT FOUND");
		
		return Response.status(Status.NOT_FOUND).build();
	}
	
	@GET
	@Path("show")
	@Produces(MediaType.APPLICATION_JSON)
	public Response getOneUser(@DefaultValue("") @QueryParam("account") String name) {
		try {
			
			logger.debug("Input account: " + name);
			
			if(name.isEmpty()){
				logger.error("Empty usr name.");
				return ErrorCode.genErrResponse(ErrorCode.ERR_INVALID_PARAMS);
			}
			
			User usr = Factory.getInstance().getUserManager().findUser(name);
			UserReponse res = new UserReponse();
			
			if(usr == null){
				logger.error("Not find user: " + name);
				return ErrorCode.genErrResponse(ErrorCode.ERR_USR_NO_USER);
			}			
			
			UserBean usrBean = new UserBean();	
			
			usrBean.setUser_name(usr.getName());
			usrBean.setPassword(usr.getPassword());
			usrBean.setEmail(usr.getEmail());
			usrBean.setRole(usr.getRole());
			usrBean.setLast_modified_time(Utils.dateToString(usr.getLastModifyTime()));
			
			res.setData(usrBean);
			
			return Response.ok(res).build();
			
		} catch (Exception e) {
			logger.error("getOneUser->Error", e);
			e.printStackTrace();
		}
		
		logger.error("getOneUser->404 NOT FOUND");
		
		return Response.status(Status.NOT_FOUND).build();
	}
	
	@POST
	@Path("add")
	@Produces(MediaType.APPLICATION_JSON)
	public Response addUser(@DefaultValue("") @QueryParam("account") String name) {
		try {
			
			logger.debug("Input account: " + name);
			
			if(name.isEmpty()){
				logger.error("Empty usr name.");
				return ErrorCode.genErrResponse(ErrorCode.ERR_INVALID_PARAMS);
			}
			
			User usr = Factory.getInstance().getUserManager().findUser(name);
			UserReponse res = new UserReponse();
			
			if(usr == null){
				logger.error("Not find user: " + name);
				return ErrorCode.genErrResponse(ErrorCode.ERR_USR_NO_USER);
			}			
			
			UserBean usrBean = new UserBean();	
			
			usrBean.setUser_name(usr.getName());
			usrBean.setPassword(usr.getPassword());
			usrBean.setEmail(usr.getEmail());
			usrBean.setRole(usr.getRole());
			usrBean.setLast_modified_time(Utils.dateToString(usr.getLastModifyTime()));
			
			res.setData(usrBean);
			
			return Response.ok(res).build();
			
		} catch (Exception e) {
			logger.error("getOneUser->Error", e);
			e.printStackTrace();
		}
		
		logger.error("getOneUser->404 NOT FOUND");
		
		return Response.status(Status.NOT_FOUND).build();
	}
	
	@POST
	@Path("login.json")
	@Produces(MediaType.APPLICATION_JSON)
	@Consumes(MediaType.APPLICATION_JSON)
	public Response login(LoginRequest req) {
		try {			
			logger.debug("request: " + req.toString());
			
			if(req.getUser_name().isEmpty() || req.getPassword().isEmpty()){
				logger.error("Empty usr name or password.");
				return ErrorCode.genErrResponse(ErrorCode.ERR_INVALID_PARAMS);
			}
			
			LoginResponse res = new LoginResponse();
			
			if(Factory.getInstance().getUserManager().verify(req.getUser_name(), req.getPassword())){
				User usr = Factory.getInstance().getUserManager().findUser(req.getUser_name());
				LoginResBean loginBean = new LoginResBean();
				loginBean.setRole(usr.getRole());
				res.setData(loginBean);
			}
			else{
				logger.error("Failed to authentication.");
				return ErrorCode.genErrResponse(ErrorCode.ERR_USR_AUTH_FAILED);
			}

			return Response.ok(res).build();
			
		} catch (Exception e) {
			logger.error("login->Error", e);
			e.printStackTrace();
		}
		
		logger.error("login->404 NOT FOUND");
		
		return Response.status(Status.NOT_FOUND).build();
	}
	
}
