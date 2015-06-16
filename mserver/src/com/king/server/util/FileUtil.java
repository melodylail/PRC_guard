package com.king.server.util;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.Reader;
import java.io.StringWriter;
import java.io.Writer;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Enumeration;
import java.util.zip.ZipEntry;
import java.util.zip.ZipFile;

import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;

public class FileUtil {
	private static Log logger = LogFactory.getLog(FileUtil.class);
	public static boolean mkdir(String folder) {

		File file = new File(folder);
		try {
			if (!file.exists()) {
				file.mkdir();
			}
		} catch (Exception e) {
			logger.error("Failed to mkdir [" + folder + "]", e);
			return false;
		}

		return true;
	}
	
	public static String convertStreamToString(InputStream is, String enc)
			throws IOException {
		if (null == enc) {
			enc = "utf-8";
		}

		if (is != null) {
			Writer writer = new StringWriter();

			char[] buffer = new char[4096];
			try {
				Reader reader = new BufferedReader(new InputStreamReader(is,
						enc));
				int n;
				while ((n = reader.read(buffer)) != -1) {
					writer.write(buffer, 0, n);
				}
			} finally {
				is.close();
			}
			return writer.toString();
		} else {
			return "";
		}
	}

	public static void delFolder(String filePath) {
		File f = new File(filePath);
		if (f != null && f.exists()) {
			if (f.isDirectory()) {
				if (f.listFiles().length == 0) {
					f.delete();
				} else {
					File delFile[] = f.listFiles();
					int i = f.listFiles().length;
					for (int j = 0; j < i; j++) {
						if (delFile[j].isDirectory()) {
							delFolder(delFile[j].getAbsolutePath());
						}
						delFile[j].delete();
					}
				}
			}
			f.delete();
		}
	}
	
	public static ArrayList<String> getFileList(String strPath) { 
		ArrayList<String> fileList = new ArrayList<String>();
        File dir = new File(strPath); 
        File[] files = dir.listFiles(); 
        
        if (files == null) 
            return fileList; 
        for (int i = 0; i < files.length; i++) { 
        	 String strFileName = files[i].getAbsolutePath().toLowerCase();
            if (files[i].isDirectory()) { 
            	if(strFileName.indexOf("SC_Info".toLowerCase()) == -1) {
            		fileList.addAll(getFileList(files[i].getAbsolutePath())); 
            	} else {
//            		logger.debug("get SC_Info folder, dismiss: " + strFileName);
            	}
            } else {                
            	if (strFileName.indexOf("iTunesMetadata.plist".toLowerCase()) == -1){
//	                logger.debug("---"+strFileName);
	                fileList.add(files[i].getAbsolutePath());                    
            	} else {
//            		logger.debug("get iTunesMetadata.plist file, dismiss: " + strFileName);
            	}
            } 
        } 
        return fileList;
    }
	
	public static void unzip(File file, String outputDirectory)
			throws IOException {
		File outFile = new File(outputDirectory);
		if (!outFile.exists()) {
			outFile.mkdirs();
		}
		ZipFile zipFile = new ZipFile(file);
		Enumeration<? extends ZipEntry> en = zipFile.entries();
		ZipEntry zipEntry = null;
		while (en.hasMoreElements()) {
			zipEntry = (ZipEntry) en.nextElement();
			if (zipEntry.isDirectory()) {
				// mkdir directory
				String dirName = zipEntry.getName();
				dirName = dirName.substring(0, dirName.length() - 1);
				File f = new File(outFile.getPath() + File.separator + dirName);
				f.mkdirs();
			} else {
				String strFilePath = outFile.getPath() + File.separator
						+ zipEntry.getName();
				File f = new File(strFilePath);
				
				if (!f.exists()) {
					String[] arrFolderName = zipEntry.getName().split("/");
					String strRealFolder = "";
					for (int i = 0; i < (arrFolderName.length - 1); i++) {
						strRealFolder += arrFolderName[i] + File.separator;
					}
					strRealFolder = outFile.getPath() + File.separator
							+ strRealFolder;
					File tempDir = new File(strRealFolder);
					tempDir.mkdirs();
				}
				
				f.createNewFile();
				InputStream in = zipFile.getInputStream(zipEntry);
				FileOutputStream out = new FileOutputStream(f);
				try {
					int c;
					byte[] by = new byte[BUFFEREDSIZE];
					while ((c = in.read(by)) != -1) {
						out.write(by, 0, c);
					}
					// out.flush();
				} catch (IOException e) {
					throw e;
				} finally {
					out.close();
					in.close();
				}
			}
		}
		zipFile.close();
	}

	private static final int BUFFEREDSIZE = 1024;

	public static byte[] fileSha1(File f) {
		byte[] mdbytes = null;
		FileInputStream fis = null;
		try {
			fis = new FileInputStream(f);
			mdbytes = fileSha1(fis);

		} catch (FileNotFoundException e) {
			e.printStackTrace();

		} finally {
			try {
				if (fis != null)
					fis.close();
			} catch (IOException e) {
			}
		}
		return mdbytes;
	}

	public static byte[] fileSha1(InputStream fis) {
		byte[] mdbytes = null;
		if (fis != null) {
			try {
				MessageDigest md = MessageDigest.getInstance("SHA1");
				byte[] dataBytes = new byte[1024];

				int nread = 0;
				while ((nread = fis.read(dataBytes)) != -1) {
					md.update(dataBytes, 0, nread);
				}
				mdbytes = md.digest();

			} catch (NoSuchAlgorithmException e) {
				e.printStackTrace();
			} catch (IOException e) {

				e.printStackTrace();
			}
		}
		return mdbytes;
	}
}
