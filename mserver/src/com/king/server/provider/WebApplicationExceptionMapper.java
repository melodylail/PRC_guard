package com.king.server.provider;

import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.Response.Status;
import javax.ws.rs.ext.ExceptionMapper;
import javax.ws.rs.ext.Provider;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

@Provider
public class WebApplicationExceptionMapper implements ExceptionMapper<WebApplicationException> {

	private final static Log logger = LogFactory.getLog(WebApplicationExceptionMapper.class);
    @Override
    public Response toResponse(WebApplicationException exception) {
    	Response r = exception.getResponse();
    	
    	logger.error("got WebApplicationException:"+r.getStatus() + ", msg: " + exception.getMessage());
        if(404==r.getStatus()||405==r.getStatus())
        {
        	return Response.status(Status.BAD_REQUEST).build(); 
        }
        return r;
    }

}