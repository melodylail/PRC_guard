package com.king.server.resource;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.HashMap;
import java.util.Map;

import javax.ws.rs.Consumes;
import javax.ws.rs.POST;
import javax.ws.rs.Path;
import javax.ws.rs.Produces;
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
import com.king.server.data.ErrorCode;
import com.king.server.response.CommonResponse;
import com.king.server.response.LoginResponse;
import com.king.server.util.FileUtil;
import com.sun.jersey.core.header.FormDataContentDisposition;
import com.sun.jersey.multipart.FormDataParam;


@Path("/upload.bin")
public class FileUploadResource {
	private final static  Log logger = LogFactory.getLog(FileUploadResource.class);
	private static String uploadPicDir = null;
	
	@POST
	@Path("/")
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
