package com.king.server.resource;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.Context;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.ws.rs.core.UriInfo;

import org.apache.commons.io.FileUtils;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

import com.king.server.Factory;
import com.king.server.PortalConfUtil;
import com.king.server.bean.DeviceInfo;
import com.king.server.bean.DeviceStatus;
import com.king.server.data.ErrorCode;
import com.king.server.response.CommonResponse;
import com.king.server.response.DevicesReponse;
import com.king.server.response.LoginResponse;
import com.king.server.response.ReportBean;
import com.king.server.response.ReportsDataTableReponse;
import com.king.server.util.FileUtil;
import com.sun.jersey.core.header.FormDataContentDisposition;
import com.sun.jersey.multipart.FormDataParam;

@Path("/device")
public class DeviceResource {
	private final static  Log logger = LogFactory.getLog(DeviceResource.class);
	private static String uploadPicDir = null;
	
	@POST
	@Path("/upload_report.bin")
	@Consumes(MediaType.MULTIPART_FORM_DATA)
	@Produces(MediaType.APPLICATION_JSON)
	public Response uploadFile(@Context UriInfo info,
			@FormDataParam("file") InputStream uploadedInputStream,
			@FormDataParam("file") FormDataContentDisposition fileDisposition){
		
		logger.info("Upload file name:" + fileDisposition.getFileName());

		
		String fielname = fileDisposition.getFileName();
		uploadPicDir = PortalConfUtil.getPortalConf().getProperty("upload.report.dir");
		logger.info("upload.report.dir:" + uploadPicDir);		
		File tmpFile = new File(uploadPicDir, fielname + ".tmp");
	    File uploadedFile = null;
		
		try{
			if (!new File(uploadPicDir).isDirectory())
				new File(uploadPicDir).mkdir();
			
			writeToFile(uploadedInputStream, tmpFile);
			uploadedFile = new File(uploadPicDir, fielname);
			
			if(!uploadedFile.exists()) {
				FileUtils.moveFile(tmpFile, uploadedFile);
			} else {
				logger.debug(fielname + " already exist");
			}
			
			CommonResponse res = new CommonResponse();
			res.setError_code(ErrorCode.ERR_SUCCESS);
			res.setMessage(ErrorCode.getErrMsg(ErrorCode.ERR_SUCCESS));
			
			return Response.ok(res).build();
			
		}catch(Exception e){
			logger.error("Upload File Error: ", e);
						
		}finally{
			if (tmpFile.exists()){
				tmpFile.delete();
			}			
		}
		
		return Response.status(Status.INTERNAL_SERVER_ERROR).build();
	}
	
	@POST
	@Path("/upload_status.bin")
	@Consumes(MediaType.APPLICATION_JSON)
	@Produces(MediaType.APPLICATION_JSON)
	public Response uploadStatus(DeviceStatus device_status){
		
		logger.info("Device upload status:" + device_status.toString());
		
		try{
			CommonResponse res = new CommonResponse();
			
			if(Factory.getInstance().getDeviceDao().updateDevice(device_status)){
				res.setError_code(ErrorCode.ERR_SUCCESS);
			}
			else{
				res.setError_code(ErrorCode.ERR_FAILED);
			}
			
			return Response.ok(res).build();
			
		}catch(Exception e){
			logger.error("uploadStatus Error: ", e);
						
		}finally{		
		}
		
		return Response.status(Status.INTERNAL_SERVER_ERROR).build();
	}
	
	@GET
	@Path("/query_all.bin")
	@Produces(MediaType.APPLICATION_JSON)
	public Response qeuryAllDevices() {
		try {
			logger.info("Receive qeuryAllDevices request");
			
			DevicesReponse res = new DevicesReponse();
			List<DeviceInfo> devices = Factory.getInstance().getDeviceDao().queryDevices();
			
			res.setData(devices);
			res.setError_code(ErrorCode.ERR_SUCCESS);
			
			return Response.ok(res).build();
			
		} catch (Exception e) {
			logger.error("qeuryAllDevices->Error", e);
			e.printStackTrace();
		}
		
		logger.error("qeuryAllDevices->404 NOT FOUND");
		
		return Response.status(Status.NOT_FOUND).build();
	}
	
	private void writeToFile(InputStream uploadedInputStream, File fileUploaded){
		OutputStream out = null;
		try {
			out = new FileOutputStream(fileUploaded);
			int read = 0;
			byte[] bytes = new byte[1024];
			
			while ((read = uploadedInputStream.read(bytes)) != -1) {
				out.write(bytes, 0, read);
			}
			out.flush();

		} catch (IOException e) {
			e.printStackTrace();
		} finally {
			if (out != null)
				try {
					out.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
		}
	}	
}
