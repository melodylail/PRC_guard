package com.king.server.user;

import java.util.HashMap;
import java.util.Map;

import com.king.server.Factory;
import com.king.server.data.User;

public class UserCacheManager {
	private static Map<String, User> mUserMap = new HashMap<String, User>();
	
	
	public static synchronized User getUserUser(String userName) {
		User obj = null;

		if (userName != null) {
			obj = mUserMap.get(userName);
			if (obj == null) {
				obj = Factory.getInstance().getUserManager().findUser(userName);
				if(obj != null) {
					mUserMap.put(userName, obj);
				}
				
			} else if (!obj.isValidate()) {
				mUserMap.remove(userName);
				obj = Factory.getInstance().getUserManager().findUser(userName);
				if(obj != null) {
					mUserMap.put(userName, obj);
				}
			}
		}
		return obj;
	}
}
