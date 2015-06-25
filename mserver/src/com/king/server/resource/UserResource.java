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
	@Path("list_all.bin")
	@Produces(MediaType.APPLICATION_JSON)
	public Response listAllUser() {
		try {
			List<User> users = Factory.getInstance().getUserManager().getAllUser();
			UsersReponse res = new UsersReponse();
			res.setError_code(ErrorCode.ERR_SUCCESS);
			
			ArrayList<UserBean> usrBeans = new ArrayList<UserBean>();			
			
			for (User usr : users){
				UserBean usrBean = new UserBean();
				usrBean.setUser_name(usr.getName());
				usrBean.setPassword(Utils.md5(usr.getPassword()));
				usrBean.setEmail(usr.getEmail());
				usrBean.setRole(usr.getRole());
				usrBean.setLast_modified_time(Utils.dateToString(usr.getLastModifyTime()));
				usrBeans.add(usrBean);
			}
			
			res.setData(usrBeans);
			
			return Response.ok(res).build();
			
		} catch (Exception e) {
			logger.error("listAllUser->Error", e);
			e.printStackTrace();
		}
		
		logger.error("listAllUser->404 NOT FOUND");
		
		return Response.status(Status.NOT_FOUND).build();
	}
	
	@GET
	@Path("get_one.bin")
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
			usrBean.setPassword(Utils.md5(usr.getPassword()));
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
	@Path("add.bin")
	@Consumes(MediaType.APPLICATION_JSON)
	@Produces(MediaType.APPLICATION_JSON)
	public Response addUser(UserRequest request) {
		try {
			
			logger.info("addUser: " + request.toString());
			
			if(request.getUser_name().isEmpty() || request.getPassword().isEmpty()){
				logger.error("addUser->Empty usr name or password.");
				return ErrorCode.genErrResponse(ErrorCode.ERR_INVALID_PARAMS);
			}
			
			CommonResponse res = new CommonResponse();
			
			if(Factory.getInstance().getUserManager().isUserExist(request.getUser_name())){
				logger.error("addUser->user has existed: " + request.getUser_name());
				return ErrorCode.genErrResponse(ErrorCode.ERR_USR_EXSITED);
			}
			
			User usr = new User(request.getUser_name());
			usr.setPassword(request.getPassword());
			usr.setRole(request.getRole());
			usr.setEmail(request.getEmail());
			
			if(Factory.getInstance().getUserManager().addUser(usr)){
				res.setError_code(ErrorCode.ERR_SUCCESS);
			}
			else{
				res.setError_code(ErrorCode.ERR_FAILED);
			}
			
			return Response.ok(res).build();
			
		} catch (Exception e) {
			logger.error("addUser->Error", e);
			e.printStackTrace();
		}
		
		logger.error("addUser->404 NOT FOUND");
		
		return Response.status(Status.NOT_FOUND).build();
	}
	
	@POST
	@Path("edit.bin")
	@Consumes(MediaType.APPLICATION_JSON)
	@Produces(MediaType.APPLICATION_JSON)
	public Response editUser(UserRequest request) {
		try {
			
			logger.info("editUser: " + request.toString());
			
			if(request.getUser_name().isEmpty() || request.getPassword().isEmpty()){
				logger.error("editUser->Empty usr name or password.");
				return ErrorCode.genErrResponse(ErrorCode.ERR_INVALID_PARAMS);
			}
			
			CommonResponse res = new CommonResponse();
			
			if(!Factory.getInstance().getUserManager().isUserExist(request.getUser_name())){
				logger.error("editUser->user not existed: " + request.getUser_name());
				return ErrorCode.genErrResponse(ErrorCode.ERR_USR_NO_USER);
			}
			
			User usr = new User(request.getUser_name());
			usr.setPassword(request.getPassword());
			usr.setRole(request.getRole());
			usr.setEmail(request.getEmail());
			
			if(Factory.getInstance().getUserManager().updateUser(usr)){
				res.setError_code(ErrorCode.ERR_SUCCESS);
			}
			else{
				res.setError_code(ErrorCode.ERR_FAILED);
			}
			
			return Response.ok(res).build();
			
		} catch (Exception e) {
			logger.error("addUser->Error", e);
			e.printStackTrace();
		}
		
		logger.error("addUser->404 NOT FOUND");
		
		return Response.status(Status.NOT_FOUND).build();
	}
	
	@POST
	@Path("delete.bin")
	@Produces(MediaType.APPLICATION_JSON)
	public Response deleteUser(@DefaultValue("") @QueryParam("account") String name) {
		try {
			
			logger.debug("Input account: " + name);
			
			if(name.isEmpty()){
				logger.error("Empty usr name.");
				return ErrorCode.genErrResponse(ErrorCode.ERR_INVALID_PARAMS);
			}
			
			CommonResponse res = new CommonResponse();
			boolean ret = Factory.getInstance().getUserManager().deleteUser(name);
			
			
			if(ret){
				res.setError_code(ErrorCode.ERR_SUCCESS);
			}
			else{
				res.setError_code(ErrorCode.ERR_FAILED);
			}
			
			return Response.ok(res).build();
			
		} catch (Exception e) {
			logger.error("deleteUser->Error", e);
			e.printStackTrace();
		}
		
		logger.error("deleteUser->404 NOT FOUND");
		
		return Response.status(Status.NOT_FOUND).build();
	}
	
	@POST
	@Path("login.bin")
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
